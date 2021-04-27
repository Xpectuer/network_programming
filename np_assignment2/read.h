#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif


char read_byte(char** s);
	
uint16_t read_uint16(char** s);



uint32_t read_uint32(char** s);


int32_t read_int32(char** s);


double read_lf(char** s);


#ifdef __cplusplus
}
#endif
