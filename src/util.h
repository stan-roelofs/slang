#ifndef SLANG_UTIL_H
#define SLANG_UTIL_H

#include <stdarg.h>

void slang_set_error(char **buffer, const char *message, ...);
void slang_set_errorv(char **buffer, const char *message, va_list valist);

#endif