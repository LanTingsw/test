#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define   N  128
typedef struct {
	char type;
	char name[32];
	char text[N];
}MSG;

#define  err_log(errlog)  do{perror(errlog); exit(1);}while(0)

int main(int argc, const char *argv[])
{
	int sockfd;	
	struct sockaddr_in serveraddr;
	MSG msg;
	pid_t pid;

	if(argc < 3)
	{
		fprintf(stderr, "Usage:%s serverip  port.\n", argv[0]);
		return -1;
	}
	if((sockfd = socket(AF_INET,SOCK_DGRAM, 0)) < 0)
	{
		err_log("fail to socket");
	}

	serveraddr.sin_family = AF_INET;
//	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
	
	serveraddr.sin_port = htons(atoi(argv[2]));

	printf("Input name> ");
	fgets(msg.name, 32, stdin);
	msg.name[strlen(msg.name)-1] = '\0';

	msg.type = 'L';
    sendto(sockfd, &msg, sizeof(msg), 0, (struct sockaddr *)&serveraddr, sizeof(serveraddr));

	if((pid = fork()) < 0)
	{
		err_log("fail to fork");
	}
	else if(pid == 0) //child
	{
		while(1)	
		{
			printf("input text >>>");
			fgets(msg.text, N, stdin);
			msg.text[strlen(msg.text)-1] = '\0';

			if(strncmp(msg.text, "quit", 4) == 0)
			{
				msg.type = 'Q';
				sendto(sockfd, &msg, sizeof(msg), 0, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
				break;
			}
			else 
			{
				msg.type = 'B';
				sendto(sockfd, &msg, sizeof(msg), 0, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
			}
		}
		kill(getppid(), SIGKILL);
		exit(0);
	}
	else // parent
	{
		while(1)
		{
			recvfrom(sockfd, &msg, sizeof(msg), 0, NULL, NULL);
			printf("%s\n", msg.text);
		}
	}

	
	close(sockfd);


	return 0;
}
