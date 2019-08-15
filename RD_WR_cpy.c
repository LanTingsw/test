#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, const char *argv[])
{
	int fd1, fd2;
	char buf[16];
	int count;
	fd1 = open("1.txt",O_RDONLY);
	fd2 = open("2.txt",O_WRONLY|O_CREAT|O_TRUNC,0666);
	while((count = read(fd1,buf,10)) > 0){
		write(fd2,buf,count);
	}
	close(fd1);
	close(fd2);
	return 0;
}
