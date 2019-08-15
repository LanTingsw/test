#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>

#define NU 100
void changestyle(char *aaa[], char buf[100])
{
	char *token;
	const char s[2] = " ";	
	token = strtok(buf,s);
	strcpy(aaa[0],token);
	int i = 1;
	 while( (token = strtok(NULL, s))!= NULL ) {
		strcpy(aaa[i],token);
		i++;
   }
   
   for(;i<10;i++){
		free(aaa[i]);
		aaa[i]=NULL;
   }


}

int main(){

    pid_t pid;
    char *p;
    int i=0,j=0;
    char buf[100]={' '};
    int len;
    char *aaa[10]={0};
    int ret;

	while(1){
        printf(">");
		
        gets(buf);


        pid = fork();
        if(pid==0){  //创建子进程执行第三方程序
			for(i=0;i<10;i++){
			   aaa[i] = malloc(32);
			}	
			changestyle(aaa, buf);	
			ret=execvp(aaa[0],aaa);
			//ret = system(buf);
			if(ret<0){
				perror("execvp");
				exit(1);		
			}
        }else if(pid>0){  //父进程等待第三方程序执行完毕回收子进程资源

            wait(NULL);
		
			
        }

    }

}
