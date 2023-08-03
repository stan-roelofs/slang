#include "memory.h"

void *xyz_reallocate(void *ptr, size_t new_size)
{
    ptr = realloc(ptr, new_size);
    if (!ptr)
        exit(1); // TODO handle this more gracefully

    return ptr;
}