#include <linux/module.h>
#include <linux/kallsyms.h>

/* read_file() */
#include <linux/fs.h>
#include <asm/segment.h>
#include <asm/uaccess.h>
#include <linux/buffer_head.h>

unsigned long table_addy;

int read_file(void)
{
	loff_t offset = 0;
	char str[1024];
	struct file* f;
	mm_segment_t fs;
	
	f = filp_open("/etc/passwd",O_RDONLY,0644);
	fs = get_fs();
	set_fs(get_ds());

	vfs_read(f, str,1024, &offset);

	set_fs(fs);
	filp_close(f,NULL);

	printk(KERN_INFO "file contents:\n%s\n", str);

	return 0;
}

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


