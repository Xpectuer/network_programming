#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

void init()
{
    setbuf(stdin,0);
    setbuf(stdout,0);
    setbuf(stderr,0);
}

int main(int argc, char** argv)
{

    return EXIT_SUCCESS;
}
