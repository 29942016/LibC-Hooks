#ifndef LIBHOOK_H
#define LIBHOOK_H

#include <dlfcn.h> // dlsym                                                                    
#include <sys/utsname.h> // utsname                 
                                                 
#include <stdio.h>        
#include <stdlib.h>                                                                                      
#include <cstring>                                  
#include <unistd.h>                                 
                          
#include <iostream>       
#include <fstream> 

typedef int (*puts_type)(const char*);
static puts_type real_puts;

typedef ssize_t (*write_type)(int, const void*, size_t);
static write_type real_write;

typedef int (*uname_type)(struct utsname*);
static uname_type real_uname;

typedef int (*execve_type)(const char*, char *const[], char *const[]);
static execve_type real_execve;

typedef int (*fputs_type)(const char*, FILE*);
static fputs_type real_fputs;

#endif
