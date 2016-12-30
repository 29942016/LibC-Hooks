
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/utsname.h>
#include <linux/cred.h>

asmlinkage int (*getuid_call)(void);
// uid_t uid = getuid_call() -- returns unsigned int of uid

asmlinkage int (*original_uname) (struct new_utsname *);
asmlinkage int overide_uname(struct new_utsname *buf)
{
	struct new_utsname our_uts =
   	{ 
		.sysname = "Looonix", 
		.nodename = "node", 
		.release = "release", 
		.version = "0.0.1-Alpha", 
		.machine = "el1",
	    .domainname = "dankmemes.net",
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

