#include <dlfcn.h>	     // dlsym
#include <sys/utsname.h> // utsname

#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <unistd.h>

#include <iostream>
#include <fstream>

typedef int (*puts_type)(const char*);
static puts_type real_puts = NULL;

typedef ssize_t (*write_type)(int, const void*, size_t);
static write_type real_write = NULL;

typedef int (*uname_type)(struct utsname*);
static uname_type real_uname = NULL;

typedef int (*execve_type)(const char*, char *const[], char *const[]);
static execve_type real_execve = NULL;

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
}

ssize_t write(int fd, const void *buf, size_t count)
{	
	printf("[WRITE HOOK TRIGGERED] %lu\n", count);

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
