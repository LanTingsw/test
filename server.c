#include <stdio.h>
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

typedef struct node {
	struct sockaddr_in clientaddr;
	struct node *next;
}linknode_t , *linklist;

#define  err_log(errlog)  do{perror(errlog); exit(1);}while(0)

linklist create_linklist(void)
{
	linklist h = (linklist)malloc(sizeof(linknode_t));
	h->next = NULL;

	return h;
}

int process_login(int sockfd, linklist h, struct sockaddr_in clientaddr , MSG msg)
{
	linklist p = h->next;
	linklist temp;

	sprintf(msg.text, "%s login.", msg.name);
	printf("%s \n", msg.text);

	while(p != NULL)
	{
		sendto(sockfd, &msg, sizeof(msg), 0, (struct sockaddr*)&p->clientaddr, sizeof(struct sockaddr));

		p = p->next;
	}

	temp = (linklist)malloc(sizeof(linknode_t));
	temp->clientaddr = clientaddr;

	temp->next = h->next;
	h->next = temp;

	return 0;
}

int process_broadcast(int sockfd, linklist h, struct sockaddr_in clientaddr, MSG msg)
{
	linklist p = h->next;
	char buf[N] = {};
	sprintf(buf, "%s said: %s", msg.name, msg.text);
	printf("%s\n", buf);
	strcpy(msg.text, buf);	

	while(p != NULL)
	{
		if(memcmp(&clientaddr, &p->clientaddr, sizeof(clientaddr)) == 0)
		{
			p = p->next;
		}
		else 
		{
			sendto(sockfd, &msg, sizeof(msg), 0, (struct sockaddr *)&p->clientaddr, sizeof(p->clientaddr));
			p = p->next;
		}
	}

	return 0;
}
int process_quit(int sockfd, linklist h, struct sockaddr_in clientaddr, MSG msg)
{
	sprintf(msg.text, "%s offline.", msg.name);
	printf("%s\n", msg.text);

	linklist p = h;
	linklist temp;

	while(p->next != NULL)
	{
		if(memcmp(&clientaddr, &p->next->clientaddr, sizeof(clientaddr)) == 0)
		{
			temp = p->next;
			p->next = temp->next;
			free(temp);
			temp = NULL;
		}
		else
		{
			sendto(sockfd, &msg, sizeof(msg), 0, (struct sockaddr*)&p->next->clientaddr, sizeof(clientaddr));
			p = p->next;
		}
	}

	return 0;
}

int main(int argc, const char *argv[])
{
	int sockfd;
	pid_t pid;
	linklist h;
	MSG msg;
	
	struct sockaddr_in serveraddr;
	struct sockaddr_in clientaddr;

	if(argc < 3)
	{
		fprintf(stderr, "Usage:%s serverip  port.\n", argv[0]);
		return -1;
	}
	if((sockfd = socket(AF_INET,SOCK_DGRAM, 0)) < 0)
	{
		err_log("fail to socket");
	}
	printf("socket ok.\n");

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(atoi(argv[2]));


	if(bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
	{
		err_log("fail to bind");
	}
	printf("bind ok. \n");

	socklen_t  addrlen = sizeof(struct sockaddr_in);

	if((pid = fork()) < 0)
	{
		err_log("fail to fork");
	}
	else if(pid > 0)
	{
		h = create_linklist();

		while(1)
		{
			recvfrom(sockfd, &msg, sizeof(msg), 0, (struct sockaddr*)&clientaddr, &addrlen);
			printf("connect ip is %s\n",(char *)inet_ntoa(clientaddr.sin_addr));
			switch(msg.type)
			{
			case 'L':
				process_login(sockfd, h, clientaddr, msg);
				break;
			case 'B':
				process_broadcast(sockfd, h, clientaddr, msg);
				break;
			case 'Q':
				process_quit(sockfd, h, clientaddr, msg);
				break;
			}

		}

	}
	else  // child
	{
		while(1)
		{
			msg.type = 'B';
			strcpy(msg.name, "server");
			fgets(msg.text, N, stdin);
			msg.text[strlen(msg.text)-1] = '\0';

			sendto(sockfd, &msg, sizeof(msg), 0, (struct sockaddr *)&serveraddr, sizeof(serveraddr));

		}
	}



	close(sockfd);


	return 0;
}
