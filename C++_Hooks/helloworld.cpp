#include "helloworld.h"

int main()
{
	tputs();
	twrite();
	tuname();
	texecve();

	printf("\n\n[END OF HOOK TEST]\n");

	for(;;)
		usleep(100);


	return 0;
}

void texecve()
{
	std::string cd = cwd();

	char *newargv[] = { (char*)"ls", (char*)"-al", &cd[0] , NULL };
	char *newenviron[] = { (char*)"PATH=/bin" }; // TODO get actual env var

	printf("\n[TEST] EXECVE \n");
	execve("/usr/bin/ls", newargv, newenviron);
}

void tputs()
{
	puts("[TEST] PUTS");
}

void twrite()
{
	write(0, "[TEST] WRITE\n", 13);
}

void tuname()
{
	struct utsname unameData;
	uname(&unameData);
	printf("[TEST] UNAME: %s", unameData.sysname);
}

std::string execpath()
{

	char result[ PATH_MAX ];
	ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
	return std::string(result, (count > 0) ? count : 0);

}

std::string cwd()
{
	char result[ PATH_MAX ];
	getcwd(result, PATH_MAX);
	return std::string(result);
}
