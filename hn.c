#include <stdio.h>
#include <stdlib.h>
//#include <sys/types.h>
#include <sys/socket.h>
//#include <unistd.h>
#include <sys/select.h>
//#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
//#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char *argv[]) {
int res;
struct sockaddr_in addr;
long arg;
fd_set myset;
struct timeval tv;
int valopt;
	socklen_t lon;
int soc,
	remotePort,
	status = 0;
//int errno = 0;
int timeout;
struct hostent *hostPtr = NULL;
int clientSocket;
char *remoteHost = NULL;


if (4 != argc)
{
	fprintf(stderr, "Usage: %s <serverHost> <serverPort> <timeout>\n",argv[0]);
	exit(1);
}

remoteHost = argv[1];
remotePort = atoi(argv[2]);
timeout = atoi(argv[3]);

fprintf(stdout,"%s:%i waiting %i ",remoteHost,remotePort,timeout);
/*
* need to resolve the remote server name or
* IP address */
hostPtr = gethostbyname(remoteHost);
if (NULL == hostPtr)
	{
	hostPtr = gethostbyaddr(remoteHost,strlen(remoteHost), AF_INET);
	if (NULL == hostPtr)
		{
		fprintf(stdout," unable to resolve %s\n", remoteHost);
		exit(1);
		}
	}

// Create socket
soc = socket(PF_INET, SOCK_STREAM,IPPROTO_TCP);

// Set non-blocking
arg = fcntl(soc, F_GETFL, NULL);
arg |= O_NONBLOCK;
fcntl(soc, F_SETFL, arg);

// Trying to connect with timeout
addr.sin_family = AF_INET;
addr.sin_port = htons(remotePort);

//addr.sin_addr.s_addr = (char) hostPtr->h_addr;
(void) memcpy(&addr.sin_addr,hostPtr->h_addr,hostPtr->h_length);
res = connect(soc, (struct sockaddr *)&addr, sizeof(addr));

if (res < 0) {
	tv.tv_sec = timeout;
	tv.tv_usec = 0;
	FD_ZERO(&myset);
	FD_SET(soc, &myset);
	if (select(soc+1, NULL, &myset, NULL, &tv) > 0) {
		lon = sizeof(int);
		getsockopt(soc, SOL_SOCKET, SO_ERROR, (void*)(&valopt), &lon);
		if (valopt) {
			fprintf(stdout," refused\n");
			strerror(valopt);
			exit(1);
		}
	}
	else {
		fprintf(stdout," timeout\n");
		strerror(valopt);
		exit(1);
	}
}

// Set to blocking mode again...
arg = fcntl(soc, F_GETFL, NULL);
arg &= (~O_NONBLOCK);
fcntl(soc, F_SETFL, arg);

// I hope that is all
fprintf(stdout," sucsess\n");
exit(0);
return 0;

}

