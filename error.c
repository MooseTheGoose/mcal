#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

void dieWhen(int condition, char *frmt, ...)
{
    va_list args;

    if(condition)
    {
        va_start(args, frmt);
        vfprintf(stderr, frmt, args);
        va_end(args);
        exit(-1);
    }
}
