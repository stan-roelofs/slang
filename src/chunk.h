#ifndef XYZ_CHUNK_H
#define XYZ_CHUNK_H

#include "common.h"
#include "value.h"

/// \brief A chunk of bytecode
typedef struct xyz_chunk
{
    uint8_t *code;
    size_t capacity;           // total capacity
    size_t size;               // actual size of code, should always be <= capacity
    xyz_value_array constants; // stores constants used in this chunk
} xyz_chunk;

void xyz_chunk_init(xyz_chunk *chunk);
void xyz_chunk_free(xyz_chunk *chunk);
void xyz_chunk_write(xyz_chunk *chunk, uint8_t value);

/**
 * \brief Adds a constant to the chunk
 * \param chunk The chunk to add the constant to
 * \param value The constant to add
 * \return The index of the constant in the chunk
 */
size_t xyz_chunk_write_constant(xyz_chunk *chunk, xyz_value value);

#endif