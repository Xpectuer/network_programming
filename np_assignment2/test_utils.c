#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include "common.h"
void init()
{
    setbuf(stdin,0);
    setbuf(stdout,0);
    setbuf(stderr,0);
}

int main(int argc, char** argv)
{
	initCalcLib();
	int iresult;
	double fresult;
	struct calcProtocol protocol;
	int ret=gen_puzzle(&protocol, &iresult, &fresult);
	printf("ret:%d\n",ret);
    return EXIT_SUCCESS;
}
