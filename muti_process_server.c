#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <linux/in.h>
#include <signal.h>
void handler(int signum){
		waitpid(-1,NULL,WNOHANG);
}
int main(int argc, const char *argv[])
{
	if(argc < 3){
		printf("please input ip and port");
		exit(-1);
	}
	int sockfd;

		printf("so OK\n");
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in serveraddr;
	struct sockaddr_in clientaddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = ntohs(atoi(argv[2]));
	serveraddr.sin_addr.s_addr = inet_addr(argv[1]);

	bind(sockfd,(struct sockaddr*)&serveraddr,sizeof(struct sockaddr));
	
		printf("bind OK\n");
	listen(sockfd,9);

		printf("listen OK\n");
	int recvbytes,acceptfd;
	char buf[128] = {0};
	socklen_t clientlen = sizeof(struct sockaddr);
	while(1){
		signal(SIGCHLD,handler);
		acceptfd = accept(sockfd,(struct sockaddr*)&clientaddr, &clientlen);
		printf("AC OK\n");
		printf("welcome: ip =%s\n",(char *)inet_ntoa(clientaddr.sin_addr.s_addr));
		pid_t pid;
		pid = fork();
		if(pid < 0){
			perror("fork failed");
			exit(-1);
		}
		if(pid == 0){
			close(sockfd);
			while(1){
				if((recvbytes = recv(acceptfd,(void *)buf,sizeof(buf),0)) < 0){
					perror("recv failed");
					exit(-1);
				}
				else if(recvbytes == 0){
					printf("client disconect");
					break;
				}
				else{
				printf("buf = %s\n",(char *)buf);
				}
			}
			close(acceptfd);
			exit(-1);
		}

		close(acceptfd);
	}

	return 0;
}
