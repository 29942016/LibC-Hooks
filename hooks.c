
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/utsname.h>

asmlinkage int (*original_uname) (struct new_utsname *);
asmlinkage int overide_uname(struct new_utsname *buf)
{
	const char* msg = "Uname hook triggered";
	int length = strlen(msg);

	original_uname(buf);
	printk(KERN_INFO "[Hook] Uname(%s)\n", buf->sysname);

	strncpy(buf->sysname, msg, length);

	return 0;
}

asmlinkage int (*original_open) (char* file, int flag, int mode);
asmlinkage int overide_open(char* file, int flags, int mode)
{
			printk(KERN_INFO "[Hook] Open(%s, %d, %d)\n", file, flags, mode);
				return original_open(file, flags, mode);
}

