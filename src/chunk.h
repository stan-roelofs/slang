#ifndef XYZ_CHUNK_H
#define XYZ_CHUNK_H

#include "common.h"
#include "value.h"

/// \brief A chunk of bytecode
typedef struct
{
    uint8_t *code;               // Array that stores bytecode
    unsigned *code_line_numbers; // For each instruction, this stores the line number of the source code. Shares the same size and capacity as code
    size_t capacity;             // total capacity of code
    size_t size;                 // actual size of code, should always be <= capacity
    xyz_value_array constants;   // stores constants used in this chunk
} xyz_chunk;

void xyz_chunk_init(xyz_chunk *chunk);
void xyz_chunk_free(xyz_chunk *chunk);
void xyz_chunk_write(xyz_chunk *chunk, uint8_t value, unsigned line_number);

/**
 * \brief Gets the line number of the source code that the instruction at the given index corresponds to
 * \param chunk The chunk to get the line number from
 * \param instruction_index The index of the instruction to get the line number of
 * \return The line number of the source code that the instruction at the given index corresponds to
 */
unsigned xyz_chunk_get_line_number(xyz_chunk *chunk, size_t instruction_index);

/**
 * \brief Adds a constant to the chunk
 * \param chunk The chunk to add the constant to
 * \param value The constant to add
 * \return The index of the constant in the chunk
 */
size_t xyz_chunk_write_constant(xyz_chunk *chunk, xyz_value value);

#endif