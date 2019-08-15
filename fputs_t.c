#include <stdio.h>

int main(int argc, const char *argv[])
{
	char buf[10] = "hello";
	FILE *fp;
	if((fp = fopen(argv[1],"a")) == NULL){
			perror("fopen");
			return 0;
	}
	fputs(buf,fp);

	return 0;
}
