#include <asm/unistd.h>
#include <asm/cacheflush.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <asm/pgtable_types.h>
#include <linux/highmem.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/moduleparam.h>
#include <linux/unistd.h>
#include <asm/cacheflush.h>
#include <linux/utsname.h>

// Syscall Table Reference
// /usr/src/kernels/4.8.15-300.fc25.x86_64/arch/x86/entry/syscalls/syscall_32.tbl
//
// Syscall Table Address
// cat /proc/kallsyms | grep sys_call_table

//Required for load_address()
MODULE_LICENSE("GPL"); 

// Pointer the Kernels system call table.
void **system_call_table_addr;
 
// Store the address of the original syscall functions so we can restore them later.
asmlinkage int (*original_uname) (struct new_utsname *);
asmlinkage int (*original_open) (char* file, int flag, int mode);
 
// Replaces the Uname syscall with `msg`.
asmlinkage int overide_uname(struct new_utsname *buf) 
{
	const char* msg = "Uname hook triggered";
	int length = strlen(msg);

	// Put the original uname values into our structure.
	original_uname(buf);

	// Print the true values to the kernel log.
    printk(KERN_INFO "[Hook] Uname(%s)\n", buf->sysname);

	// Override the output to the user.
	strncpy(buf->sysname, msg, length);

    return 0;
}

asmlinkage int overide_open(char* file, int flags, int mode)
{
	printk(KERN_INFO "[Hook] Open(%s, %d, %d)\n", file, flags, mode);
	return original_open(file, flags, mode);
}
 
// Make page writeable.
int make_rw(unsigned long address)
{
    unsigned int level;
    pte_t *pte = lookup_address(address, &level);
    if(pte->pte &~_PAGE_RW)
    {
    	printk(KERN_INFO "Address: 0x%lx set to ACCESS_RW\n", address);
        pte->pte |=_PAGE_RW;
    }
    return 0;
}
 
// Make the page write protected.
int make_ro(unsigned long address)
{
    unsigned int level;
    pte_t *pte = lookup_address(address, &level);
    pte->pte = pte->pte &~_PAGE_RW;
   	printk(KERN_INFO "Address: 0x%lx set to ACCESS_RO\n", address);
    return 0;
}
 
static int __init entry_point(void)
{
    printk(KERN_INFO "Module loaded into kernel space.\n");
    system_call_table_addr = (void*)0xffffffff83a001c0;
 
    // Replace custom syscall with the correct system call name (write,open,etc) to hook.
    original_uname = system_call_table_addr[__NR_uname];
	original_open  = system_call_table_addr[__NR_open];
 
    // Disable page protection*/
    make_rw((unsigned long)system_call_table_addr);
	
    // Change syscall to our syscall function.
    system_call_table_addr[__NR_uname] = overide_uname;
	system_call_table_addr[__NR_open] = overide_open;
	
    return 0;
}
 
static void __exit exit_point(void)
{
    printk(KERN_INFO "Module unloaded.\n");
 
    // Restore original system call.
    system_call_table_addr[__NR_uname] = original_uname;
    system_call_table_addr[__NR_open] = original_open;
 
    // Renable page protection.
    make_ro((unsigned long)system_call_table_addr);
}
 
module_init(entry_point);
module_exit(exit_point);
