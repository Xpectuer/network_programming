#include "common.h"
#include <stdarg.h>
#include <syslog.h>
/* error - print a diagnostic and optionally exit */
void error(int status, int err, char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    char *flag = status ? "[FATAL]" : "[WARNING]";
    fprintf(stderr, "%s:", flag);
    vfprintf(stderr, fmt, ap);

    va_end(ap);
    if (err)
        fprintf(stderr, ": %s (%d)\n", strerror(err), err);
    if (status)
        exit(status);
    fputs("\n", stderr);
}

void info(char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    fputs("[INFO] ", stdout);
    vfprintf(stdout, fmt, ap);
    fputs("\n", stdout);
}

void cmd_info(char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stdout, fmt, ap);
    fputs("\n> ", stdout);
    fflush(stdout);
}
