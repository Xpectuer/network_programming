#include "read.h"


// for testing
static void print_bytes(int n, char buf[n])
{
	puts("The byte array is: ");
	fputc('[',stdout);
	for(int i=0;i<n;i++)
	{
		fprintf(stdout,"\'%c\' ",buf[i]);
	}
	fputc(']',stdout);
	fputc('\n',stdout);
}

char read_byte(char** s)
{
	char c = **s;
	(*s)++;
	return c;

}

uint16_t read_uint16(char** s)
{
	int size = 2;
	char buf[size+1];
	memcpy(buf,*s,size);
	buf[size] = 0;
	*s+=size;
	return (uint16_t) atoll(buf);
}

uint32_t read_uint32(char** s)
{
	int size = 4;
	char buf[size+1];
	memcpy(buf, *s, size);
	buf[size] = 0;
	*s+=size;
	return (uint32_t) atoll(buf);
}

int32_t read_int32(char** s)
{
	int size = 4;
	char buf[size+1];
	memcpy(buf,*s,size);
	buf[size]=0;	
	print_bytes(size,buf);
	*s+=size;
	return (int32_t) atoll(buf);
}

double read_lf(char** s)
{
	int size = 8;
	char buf[size+1];
	memcpy(buf,*s,size);
	buf[size]=0;
	print_bytes(size+1, buf);
	printf("%s\n",buf);
	*s+=size;
	return (double) strtod(buf,NULL);
}


