#include <stdio.h>

int main(int argc, const char *argv[])
{
	FILE *fp1, *fp2;
	char buf[16];
	int count;
	fp1 = fopen("1.txt","r");
	fp2 = fopen("2.txt","w");
	while((count = fread(buf,1,10,fp1)) > 0){
		fwrite(buf,1,count,fp2);
	}
	fclose(fp1);
	fclose(fp2);
	return 0;
}
