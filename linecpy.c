#include <stdio.h>

int main(int argc, const char *argv[])
{
	FILE *fp1,*fp2;
	char buf[12];
	fp1 = fopen("1.txt","r");
	fp2 = fopen("2.txt","w");
	while(fgets(buf,10,fp1) != NULL){
		fputs(buf,fp2);
	}
	fclose(fp1);
	fclose(fp2);
	return 0;
}
