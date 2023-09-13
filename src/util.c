#include "util.h"

#include <string.h>

#include "memory.h"

void slang_set_error(char **buffer, const char *message)
{
    *buffer = slang_reallocate(*buffer, strlen(message) + 1);
    strcpy((char *)(*buffer), message);
}