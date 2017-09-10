#include "dropper.h"

std::string mystring("127.0.0.1");

void download();
bool fileExists(std::string);
void execute();

int pull()
{
	fprintf(stdout, "Dropper called");

	//	fileExists 
	//	no
	// 		download()
	// 		execute()
	//  yes	
	// 		execute()

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
	std::vector<std::string> webclients = 
	{
			"/usr/bin/wget",
		   	"/usr/bin/curl"
	};

	for(std::vector<std::string>::iterator it = webclients.begin() ; it != webclients.end(); it++)
	{
		if(fileExists(webclients[it - webclients.begin()]))
		{
				execute();
				break;
		}
	}
}

void execute()
{

}
