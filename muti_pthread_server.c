#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/in.h>
#include <pthread.h>

void *func(void * arg){
	int acceptfd = *(int *)arg;
	int recvbytes;
	char buf[128] = {0};
	while(1){
		if((recvbytes = recv(acceptfd,(void *)buf,sizeof(buf),0)) < 0){
			printf("sad");
		}

		else if(recvbytes == 0){
			printf("client disconect");
			break;
		}
		else{
			printf("buf = %s\n",(char *)buf);
		}
	}


}

int main(int argc, const char *argv[])
{

	int sockfd, acceptfd;

	int opt = 1;
	//setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

	struct sockaddr_in serveraddr;
	struct sockaddr_in clientaddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(atoi(argv[2]));
	//serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(sockfd,(struct sockaddr*)&serveraddr,sizeof(struct sockaddr));

	printf("bind OK\n");
	listen(sockfd,9);
	printf("listen OK\n");
	char buf[128] = {0};
	socklen_t clientlen;
	pthread_t tid;
	while(1){
		acceptfd = accept(sockfd, (struct sockaddr *)&clientaddr,&clientlen);
		pthread_create(&tid,NULL,func,(void *)acceptfd);
	}
	close(sockfd);
	close(acceptfd);
	return 0;
}
