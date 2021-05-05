#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdint.h>
#include<time.h>
void init()
{
    setbuf(stdin,0);
    setbuf(stdout,0);
    setbuf(stderr,0);
}

int main(int argc, char** argv)
{
	uint32_t i;
		
	printf("%d",sizeof(char));
	perror("...");
	clock_t t1= clock();
	//printf("time %ld\n",t1/CLOCKS_PER_SEC);
	//sleep(1000)
//	scanf("%d",&i);
//	if(i > 8)
//	{
//		puts("pwn");
//	}
	sleep(5);
	time_t t2 = clock();
	printf("time %ld\n",(t2-t1)/CLOCKS_PER_SEC);
    return EXIT_SUCCESS;
}
