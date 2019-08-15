#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
int carda = 20000;
int cardb = 20000;

pthread_mutex_t lock;

void * drawmoney(void *arg){
	while(1){
		pthread_mutex_lock(&lock);	
		carda -= 20;
		usleep(100000);
		cardb -= 20;
		if(carda != cardb){
			printf("%d  %d\n",carda,cardb);
		}
		pthread_mutex_unlock(&lock); 
		sleep(1);
	}
}

int main(int argc, const char *argv[])
{
	int re;
	pthread_mutex_init(&lock,NULL);
	pthread_t tid1,tid2;
	pthread_create(&tid1,NULL,drawmoney,NULL);
	pthread_create(&tid2,NULL,drawmoney,NULL);
	while(1){
		sleep(1);
	}
	return 0;
}
