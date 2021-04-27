#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include "read.h"

void init()
{
    setbuf(stdin,0);
    setbuf(stdout,0);
    setbuf(stderr,0);
}

int main(int argc, char** argv)
{
	char *str = "123412.121213";
	
	char *ptr = str; 	
	printf("uint32 is:%d\n", read_uint32(&str));
	printf("offset is:%p\n", str-ptr);	

	ptr = str;
	printf("double is:%lf\n", read_lf(&str));
	printf("offset is:%p\n", str-ptr);	

	char* str1 = "12a1231";
	ptr = str1;
	printf("uint16 is:%d\n", read_uint16(&str1));
	printf("offset is:%p\n", str1-ptr);	

	ptr = str1;
	printf("byte is: %c\n", read_byte(&str1));
	printf("offset is:%p\n", str1-ptr);


	ptr = str1;
	printf("int32 is: %d\n", read_int32(&str1));
	printf("offset is:%p\n", str1-ptr);
	

    return EXIT_SUCCESS;
}
