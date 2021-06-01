#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
	
void init()
{
	setbuf(stdin,0);
	setbuf(stdout,0);
	setbuf(stderr,0);
}

int main(int argc, char** argv)
{
    char buf[1024]="ERROR: max queue size reached!";
	for(int i=6;i<=10;i++)
	{
		char filename[1024];
		sprintf(filename, "%d.log",i);
		int fd = open(filename, O_WRONLY);
		write(fd,buf,1024);
		close(fd);
	}	
	return EXIT_SUCCESS;
}
