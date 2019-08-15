#include <stdio.h>

int main(int argc, const char *argv[])
{
	
	//char buf[10] = "hello";
	FILE *fp;
	if((fp = fopen("test.txt","r+")) == NULL){
			perror("fopen");
			return 0;
	}
	fseek(fp,0,SEEK_END);
	printf("lengthis:%d\n",ftell(fp));
	return 0;
}
