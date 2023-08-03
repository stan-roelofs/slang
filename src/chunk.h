#ifndef XYZ_CHUNK_H
#define XYZ_CHUNK_H

#include "common.h"

/// \brief A chunk of bytecode
typedef struct xyz_chunk
{
    uint8_t *code;
    size_t capacity; // total capacity
    size_t size;     // actual size of code, should always be <= capacity
} xyz_chunk;

void xyz_chunk_init(xyz_chunk *chunk);
void xyz_chunk_free(xyz_chunk *chunk);
void xyz_chunk_write(xyz_chunk *chunk, uint8_t value);

#endif