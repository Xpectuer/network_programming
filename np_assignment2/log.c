#include "common.h"

void fatal(char* message)
{
    fputs("[FATAL]",stderr);
    fputs(message,stderr);
    fputs("\n",stderr);
    exit(1);
}


