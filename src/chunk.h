#ifndef SLANG_CHUNK_H
#define SLANG_CHUNK_H

#include "common.h"
#include "value.h"

/// \brief A chunk of bytecode
typedef struct
{
    uint8_t *code;               // Array that stores bytecode
    unsigned *code_line_numbers; // For each instruction, this stores the line number of the source code. Shares the same size and capacity as code
    size_t capacity;             // total capacity of code
    size_t size;                 // actual size of code, should always be <= capacity
    slang_value_array constants;   // stores constants used in this chunk
} slang_chunk;

void slang_chunk_init(slang_chunk *chunk);
void slang_chunk_free(slang_chunk *chunk);
void slang_chunk_write(slang_chunk *chunk, uint8_t value, unsigned line_number);

/**
 * \brief Gets the line number of the source code that the instruction at the given index corresponds to
 * \param chunk The chunk to get the line number from
 * \param instruction_index The index of the instruction to get the line number of
 * \return The line number of the source code that the instruction at the given index corresponds to
 */
unsigned slang_chunk_get_line_number(slang_chunk *chunk, size_t instruction_index);

/**
 * \brief Adds a constant to the chunk
 * \param chunk The chunk to add the constant to
 * \param value The constant to add
 * \return The index of the constant in the chunk
 */
size_t slang_chunk_write_constant(slang_chunk *chunk, slang_value value);

#endif