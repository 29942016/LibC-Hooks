#include <dlfcn.h>	     // dlsym
#include <sys/utsname.h> // utsname

#include <stdio.h>
#include <stdlib.h>
#include <cstring>

typedef int (*puts_type)(const char*);
static puts_type real_puts = NULL;

typedef int (*uname_type)(struct utsname*);
static uname_type real_uname = NULL;

int uname(struct utsname* info)
{
	real_uname = (uname_type)dlsym(RTLD_NEXT, "uname");

	printf("[UNAME HOOK TRIGGERED]");
	printf("\n");

	real_uname(info);

	std::strcpy(info->sysname, "SYSNAME");
	std::strcpy(info->nodename, "NODE");
	std::strcpy(info->release, "RELEASE");
	std::strcpy(info->version, "VERSION");
	std::strcpy(info->machine, "MACHINE");
	std::strcpy(info->domainname, "DOMAINNAME");
}

/*
typedef ssize_t (*write_type)(int, const void*, size_t);
static write_type real_write = NULL;


ssize_t write(int fd, const void *buf, size_t count)
{	
	printf("writechars#:%lu\n", count);

	real_write = (write_type)dlsym(RTLD_NEXT, "write");
	real_write(fd, buf, count);	
}
*/

int puts(const char* str)
{
    //printf("puts:chars#:%lu\n", strlen(str));
    /* resolve the real puts function from glibc
     * and pass the arguments.
    */
//	printf("[PUTS HOOK TRIGGERED]");
//	printf("\n");
//	fflush(stdout);

    real_puts = (puts_type)dlsym(RTLD_NEXT, "puts");
    real_puts(str);
}
