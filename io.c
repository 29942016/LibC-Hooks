#include <linux/module.h>
#include <linux/kallsyms.h>

unsigned long table_addy;

int prsyms_print_symbol(void* data, const char* namebuf,
							   struct module* module, unsigned long address)
{
	char* table = "sys_call_table";

	if(strcmp(table, namebuf) == 0)
	{
		printk(KERN_INFO "[LIBC @]:\t0x%lx\n", address);
		table_addy = address;
	}

	return 0;
}

