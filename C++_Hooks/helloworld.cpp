#include "helloworld.h"

int main()
{
	tputs();
	twrite();
	tuname();

	printf("\n\n[END OF HOOK TEST]\n");
	return 0;
}

void tputs()
{
	puts("PUTS");
}

void twrite()
{
	write(0, "WRITE\n", 6);
}

void tuname()
{
	struct utsname unameData;
	uname(&unameData);
	printf("%s", unameData.sysname);
}

