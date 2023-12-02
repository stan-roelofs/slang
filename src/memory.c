#include "memory.h"

#include <stdio.h>
#include <stdlib.h>

void *slang_reallocate(void *ptr, size_t new_size)
{
    ptr = realloc(ptr, new_size);
    if (!ptr)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1); // TODO handle this more gracefully
    }

    return ptr;
}

void *slang_allocate(size_t size)
{
    return slang_reallocate(NULL, size);
}

void slang_free(void *ptr)
{
    free(ptr);
}