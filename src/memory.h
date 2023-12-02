#ifndef SLANG_MEMORY_H
#define SLANG_MEMORY_H

void *slang_reallocate(void *ptr, size_t new_size);
void *slang_allocate(size_t size);
void slang_free(void *ptr);

#endif