#include "memory.h"

#include <stdio.h>

void *xyz_reallocate(void *ptr, size_t new_size)
{
    ptr = realloc(ptr, new_size);
    if (!ptr)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1); // TODO handle this more gracefully
    }

    return ptr;
}

void *xyz_allocate(size_t size)
{
    return xyz_reallocate(NULL, size);
}

void xyz_free(void *ptr)
{
    free(ptr);
}