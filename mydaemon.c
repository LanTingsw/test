#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
int main(int argc, const char *argv[])
{
	pid_t pid;
	pid = fork();
	FILE *fp;
	time_t t;
	if(pid < 0){
		perror("fork");
	}else if(pid > 0){
		exit(0);
	}else{
		setsid();
		chdir("/tmp");
		umask(0);
		int i;
		for(i = 3 ; i < getdtablesize(); i++){
			printf("%d\n",i);
			close(i);
		}
		fp = fopen("timetest.log","w+");
		while(1){
		time(&t);
		fprintf(fp,"%s",ctime(&t));
		fflush(fp);
		sleep(1);
		}
	}

	return 0;
}
