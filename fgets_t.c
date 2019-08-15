#include <stdio.h>
#define N  10

int main(int argc, const char *argv[])
{
	char buf[N];
	fgets(buf,N,stdin);
	printf("%s\n",buf);
	return 0;
}
