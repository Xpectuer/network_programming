#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>	
#include <fcntl.h>

void init()
{
	setbuf(stdin,0);
	setbuf(stdout,0);
	setbuf(stderr,0);
}

void foo(char* s)
{
	printf("%s\n", s);

}
// test: passed
// *enhencement of read call
// auto set 0 for buffer
// check for error
// auto set the last byte to 0.
int read_plus(int fd, void* buf, size_t size)
{
		bzero(buf,sizeof(buf));
		int n = read(fd, buf, size);
		if(n == 0)
		{
			puts("client closed...");
			close(fd);
			return 0;
		} 
		if(n < 0)
		{
			fatal("read error... Exit");
		}
		((char*)buf)[n] = 0;
		return n;
}
int main(int argc, char** argv)
{
		
	char* s = "foo!!";
	foo(s);

	int fd;
	if((fd= open("./1.txt",O_RDONLY))<0)
		perror("open error!");	
	char buf[1024];
	printf("%lu\n",sizeof(buf));
	
	printf("%s\n",buf);	
	printf("%p\n", buf);	
	read_plus(fd, buf, sizeof(buf));
	printf("%s\n", buf);
	free(buf);
	return EXIT_SUCCESS;
}
