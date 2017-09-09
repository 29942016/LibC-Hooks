#include <stdio.h>
#include <unistd.h>
#include <sys/utsname.h>

int main()
{
	write(0, "WRITE Test\n", 11); 
	puts("PUTS test\n");


	struct utsname unameData;
	uname(&unameData);
	printf("%s", unameData.sysname);
	return 0;
}

