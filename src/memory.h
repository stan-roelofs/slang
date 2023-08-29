#ifndef XYZ_MEMORY_H
#define XYZ_MEMORY_H

#include "common.h"

void *xyz_reallocate(void *ptr, size_t new_size);
void *xyz_allocate(size_t size);
void xyz_free(void *ptr);

#endif