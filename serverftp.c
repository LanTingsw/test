/*************************************************************************
	#	 FileName	: server.c
	#	 Author		: gaomingxu 
	#	 Email		: gaomx_jt@hqyj.com 
	#	 Created	: Wed 07 Aug 2019 02:50:02 PM CST
 ************************************************************************/

#include<stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <linux/in.h>  //sockaddr_in
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>

#define N 128

void server_list(int acceptfd);
void server_put(int acceptfd,char *filename);
void server_get(int acceptfd,char *filename);

int main(int argc, const char *argv[])
{

	if(argc < 3)
	{
		printf("please use %s <IP> <PORT>.\n",argv[0]);
		exit(-1);
	}

	//1) socket(),创建套接字文件，用于连接
	int sockfd,acceptfd;

	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd < 0)
	{
		perror("socket failed.");
		exit(-1);
	}
	printf("socket ok.\n");

	//填充结构体sockaddr_in 
	struct sockaddr_in serveraddr;
	serveraddr.sin_family = AF_INET;            //填充协议 IPv4
	serveraddr.sin_port = htons(atoi(argv[2]));  // 端口号 ,atoi()字符串转整形
	serveraddr.sin_addr.s_addr = inet_addr(argv[1]);  //IP

	socklen_t addrlen = sizeof(serveraddr);

	//2) bind(),绑定，把socket()函数返回的文件描述符和IP、端口号进行绑定;
	if(bind(sockfd,(struct sockaddr *)&serveraddr,addrlen) < 0)
	{
		perror("bind failed.");
		exit(-1);
	}
	printf("bind ok.\n");
	
	//3) listen(), (监听)将socket()返回的文件描述符的属æ§，由主动变为被动;
	if(listen(sockfd,8) < 0)
	{
		perror("bind failed.");
		exit(-1);
	}
	printf("listen ok.\n");

	//4) accept(), 阻塞函数，阻塞等待å¢户端的连接请求，如果有客户端连接，
	//则accept()函数返回的文件描述符和数返回，返回一个用于通信的套接字文件;
	
	acceptfd = accept(sockfd,NULL,NULL);
	printf("acceptfd ok.\n");

	char buf[N] = {0};
	int recvbytes;
	while(1)
	{
		//5) recv(), 接收客户端发来的数据;   read()
		recvbytes = recv(acceptfd,(void *)buf,sizeof(buf),0);
		if(recvbytes < 0){
			printf("recv failed.\n");
			exit(-1);
		}else if(recvbytes == 0){
			//printf("client exit.\n");
			//break;
		}else{

			switch(buf[0])
			{
				case 'L':
					server_list(acceptfd);
					break;
				case 'P':
					server_put(acceptfd,buf + 2); //P 1.c
					break;
				case 'G':
					server_get(acceptfd,buf + 2);
					break;
			}
		}
	}

	//7) Clonese(), 关闭文件描述符;连接、通信

	close(sockfd);

    return 0;
}

void server_list(int acceptfd)
{
	DIR *dir;
	char buf[N] = {0};
	struct dirent *dirent;
	struct stat statbuf;
	dir = opendir(".");//打开目录，返回目录流
	
	//dirent = readdir(dir); //读目录 
	while((dirent = readdir(dir)) != NULL)
	{
		if(strncmp(dirent->d_name,".",1) == 0)//如果是以.开头的文件都屏蔽掉
			continue;
		
		stat(dirent->d_name,&statbuf);
		if(S_ISDIR(statbuf.st_mode))
			continue;

		//send(acceptfd,dirent->d_name,sizeof(dirent->d_name),0);
		strcpy(buf,dirent->d_name);
		send(acceptfd,buf,sizeof(buf),0);
		usleep(1000);
	}
	strcpy(buf,"quit");
	send(acceptfd,buf,sizeof(buf),0);

	printf("server list ok.\n");
}
void server_put(int acceptfd,char *filename)
{
	int fd;
	char buf[N] = {0};
	int recvbytes;
	fd = open(filename,O_WRONLY | O_CREAT | O_TRUNC, 0666);

	while(1)
	{
		recvbytes = recv(acceptfd,buf,sizeof(buf),0);
		if(strncmp(buf,"quit",4) == 0)
			break;
		write(fd,buf,recvbytes);
	}
	close(fd);
	printf("server put ok.\n");
}

void server_get(int acceptfd,char *filename)
{
	int fd;
	char buf[N] = {0};
	int readbytes;
/*	fd = open(filename,O_WRONLY | O_CREAT | O_TRUNC, 0666);

	while(1)
	{
		recvbytes = recv(acceptfd,buf,sizeof(buf),0);
		if(strncmp(buf,"quit",4) == 0)
			break;
		write(fd,buf,recvbytes);
	}
	close(fd);
	printf("server put ok.\n");*/

	
	fd = open(filename,O_RDONLY);

	while(1)
	{
		readbytes = read(fd,buf,sizeof(buf));
		if(readbytes == 0)
			break;
		send(acceptfd,buf,readbytes,0);
		usleep(1000);//加延时，防止沾包

	}
	strcpy(buf,"quit");
	send(acceptfd,buf,sizeof(buf),0);

	printf("server get ok.\n");
}

