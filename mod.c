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
 
// Syscall Table Reference
// /usr/src/kernels/4.8.15-300.fc25.x86_64/arch/x86/entry/syscalls/syscall_32.tbl
//
// Syscall Table Address
// cat /proc/kallsyms | grep sys_call_table

//Required for load_address()
MODULE_LICENSE("GPL"); 

// Pointer the Kernels system call table.
void **system_call_table_addr;
 
/*my custom syscall that takes process name*/
asmlinkage int (*original_syscall) (char* name);
 
// Our desired functionallity when the hook gets hit.
asmlinkage int overide_syscall(char* play_here) 
{
    printk(KERN_INFO "Hook triggered.\n");
    return original_syscall(play_here);
}
 
/*Make page writeable*/
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
 
/* Make the page write protected */
int make_ro(unsigned long address)
{
    unsigned int level;
    pte_t *pte = lookup_address(address, &level);
    pte->pte = pte->pte &~_PAGE_RW;
    return 0;
}
 
static int __init entry_point(void)
{
    printk(KERN_INFO "Module loaded into kernel space.\n");
    /*MY sys_call_table address*/
    system_call_table_addr = (void*)0xffffffff83a001c0;
 
    /* Replace custom syscall with the correct system call name (write,open,etc) to hook*/
    original_syscall = system_call_table_addr[__NR_uname];
 
    /*Disable page protection*/
    make_rw((unsigned long)system_call_table_addr);
	
    /*Change syscall to our syscall function*/
    system_call_table_addr[__NR_uname] = overide_syscall;
	
    return 0;
}
 
static void __exit exit_point(void)
{
    printk(KERN_INFO "Module unloaded.\n");
 
    /*Restore original system call */
    system_call_table_addr[__NR_uname] = original_syscall;
 
    /*Renable page protection*/
    make_ro((unsigned long)system_call_table_addr);
}
 
module_init(entry_point);
module_exit(exit_point);
