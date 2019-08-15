/*************************************************************************
	#	 FileName	: client.c
	#	 Author		: gaomingxu 
	#	 Email		: gaomx_jt@hqyj.com 
	#	 Created	: Wed 07 Aug 2019 02:50:09 PM CST
 ************************************************************************/

#include<stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <linux/in.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>


#define N 128

void client_list(int sockfd);
void client_put(int sockfd,char *filename);
void client_get(int sockfd,char *filename);
//void client_get();


int main(int argc, const char *argv[])
{
	int sockfd;
	//1.创建套接字，用于连接和通信的
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd < 0){
		perror("socket failed.");
		exit(-1);
	}
	printf("socket ok.\n");

	struct sockaddr_in serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(atoi(argv[2]));
	serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
	
	socklen_t addrlen = sizeof(serveraddr);

	//2.connect 连接 
	if(connect(sockfd,(struct sockaddr *)&serveraddr,addrlen) < 0)
	{
		perror("connect failed.");
		exit(-1);
	}
	printf("connect ok.\n");

	char buf[128] = {0};

	while(1)
	{
		printf("*********list*******\n");
		printf("*****put filename***\n");
		printf("*****get filename***\n");
		printf("*********quit*******\n");
		printf("********************\n");
		printf("please input>>");

		fgets(buf,sizeof(buf),stdin);
		buf[strlen(buf) - 1] = '\0';   // 吃\n

		switch(buf[0])
		{
			case 'l':
				client_list(sockfd);
				break;
			case 'p':
				client_put(sockfd,buf + 4);  //put 1.c
				break;
			case 'g':
				client_get(sockfd,buf + 4);
				break;
			case 'q':
				exit(0);
			default:
				printf("input error.\n");
				break;
		}
	}

	close(sockfd);
    return 0;
}

void client_list(int sockfd)
{
	char buf[N] = {0};
	sprintf(buf,"L");
	int i = 0;
	send(sockfd,buf,sizeof(buf),0);//发送L给服务器，让服务器知道现在做的是list

	while(1)
	{
		recv(sockfd,buf,sizeof(buf),0);
		if(strncmp(buf,"quit",4) == 0)
			break;
		printf("%d.%s\n",++i,buf);
	}

	printf("client list is ok.\n");
}

void client_put(int sockfd,char *filename)
{
	int fd;
	char buf[N] = {0};
	int readbytes;
	sprintf(buf,"P %s",filename);//P 1.c

	send(sockfd,buf,sizeof(buf),0);

	fd = open(filename,O_RDONLY);

	while(1)
	{
		readbytes = read(fd,buf,sizeof(buf));
		if(readbytes == 0)
			break;
		send(sockfd,buf,readbytes,0);
		usleep(1000);//加延时，防止沾包

	}
	strcpy(buf,"quit");
	send(sockfd,buf,sizeof(buf),0);

	printf("client put ok.\n");
}

void client_get(int sockfd,char *filename)
{
	int fd;
	char buf[N] = {0};
	int recvbytes;
	sprintf(buf,"G %s",filename);//P 1.c

	send(sockfd,buf,sizeof(buf),0);

	/*fd = open(filename,O_RDONLY);

	while(1)
	{
		readbytes = read(fd,buf,sizeof(buf));
		if(readbytes == 0)
			break;
		send(sockfd,buf,readbytes,0);
		usleep(1000);//加延时，防止沾包

	}
	strcpy(buf,"quit");
	send(sockfd,buf,sizeof(buf),0);

	printf("client put ok.\n");*/

	fd = open(filename,O_WRONLY | O_CREAT | O_TRUNC, 0666);

	while(1)
	{
		recvbytes = recv(sockfd,buf,sizeof(buf),0);
		if(strncmp(buf,"quit",4) == 0)
			break;
		write(fd,buf,recvbytes);
	}
	close(fd);
	printf("client get ok.\n");







	
}
