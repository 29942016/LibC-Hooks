
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/utsname.h>

asmlinkage int (*original_uname) (struct new_utsname *);
asmlinkage int overide_uname(struct new_utsname *buf)
{
	struct new_utsname our_uts =
   	{ 
		.sysname = "Loonix", 
		.nodename = "Nodename", 
		.release = "Release", 
		.version = "Version", 
		.machine = "Machine",
	    .domainname = "DomainName",
	};

	struct new_utsname *p_uts = &our_uts;

	*buf = *p_uts;

	return 0; //original_uname(buf);
}

asmlinkage int (*original_open) (char* file, int flag, int mode);
asmlinkage int overide_open(char* file, int flags, int mode)
{
	//printk(KERN_INFO "[Hook]:\tOpen(%s, %d, %d)\n", file, flags, mode);
	return original_open(file, flags, mode);
}

