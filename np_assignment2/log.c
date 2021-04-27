#include "common.h"

void fatal(char* message)
{
    fputs("[FATAL]",stderr);
    fputs(message,stderr);
    fputs("\n",stderr);
    exit(1);
}


// for testing
#ifdef __cplusplus
extern "C"{
#endif
void print_bytes(size_t size,char* buf)
{
	puts("the byte array is: ");
	fputc('[',stdout);
	for(int i=0;i < size;i++)
	{
		fprintf(stdout,"\'%x\' ",*buf);
	}
	fputc(']',stdout);
	fputc('\n',stdout);
}
#ifdef __cplusplus
}
#endif


