#ifndef SLANG_UTIL_H
#define SLANG_UTIL_H

#include <stdarg.h>

void slang_set_error(char **buffer, const char *message, ...);
void slang_set_errorv(char **buffer, const char *message, va_list valist);

#ifdef SLANG_DEBUG
#include <stdio.h>

#define slang_trace(...) printf(__VA_ARGS__)
#else
#define slang_trace(...) (void)0
#endif

#endif