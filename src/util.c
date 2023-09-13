#include "util.h"

#include <stdio.h>
#include <string.h>

#include "memory.h"

void slang_set_error(char **buffer, const char *message, ...)
{
    va_list valist;
    va_start(valist, message);

    slang_set_errorv(buffer, message, valist);
    va_end(valist);
}

void slang_set_errorv(char **buffer, const char *message, va_list valist)
{
    int size = vsnprintf((char *)NULL, 0, message, valist);
    *buffer = slang_reallocate(*buffer, size + 1);
    vsprintf(*buffer, message, valist);
}