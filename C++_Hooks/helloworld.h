#ifndef HELLOWORLD_H
#define HELLOWORLD_H
#endif

#include <stdio.h>
#include <unistd.h>
#include <string>
#include <limits.h>
#include <sys/utsname.h>

void tputs();
void twrite();
void tuname();
void texecve();
void tfputs();

std::string cwd();
std::string execpath();
