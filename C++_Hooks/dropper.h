#ifndef DROPPER_H
#define DROPPER_H


#include <array>
#include <cstdio>
#include <fcntl.h>
#include <iostream>
#include <memory>
#include <string>
#include <sys/stat.h>
#include <system_error>
#include <stdio.h>
#include <stdlib.h>
#include <stdexcept>
#include <sys/types.h>
#include <unistd.h>
#include <vector>


const std::string _IP("127.0.0.1");
const std::string _PayloadLocal("/tmp/image.jpg");
const std::string _PayloadRemote("https://www.cleverfiles.com/howto/wp-content/uploads/2016/08/mini.jpg");
const std::string  _ProcName("TestProcess");

bool fileExists(std::string);
void execute();
int  pull();
void download();
bool payloadRunning();

#endif


