#include "chunk.h"

#include <assert.h>

#include "memory.h"

#define MINIMUM_CAPACITY 32
#define GROW_CHUNK(current_capacity) (((size_t)((current_capacity) < MINIMUM_CAPACITY ? MINIMUM_CAPACITY : (current_capacity) * 1.5)))

void slang_chunk_init(slang_chunk *chunk)
{
    chunk->size = 0;
    chunk->capacity = 0;
    chunk->code = NULL;
    chunk->code_line_numbers = NULL;
    slang_value_array_init(&chunk->constants);
}

void slang_chunk_free(slang_chunk *chunk)
{
    if (!chunk)
        return;
    slang_free(chunk->code);
    slang_free(chunk->code_line_numbers);
    slang_value_array_free(&chunk->constants);
    slang_chunk_init(chunk);
}

void slang_chunk_write(slang_chunk *chunk, uint8_t value, unsigned line_number)
{
    if (chunk->size == chunk->capacity)
    {
        chunk->capacity = GROW_CHUNK(chunk->capacity);
        chunk->code = slang_reallocate(chunk->code, chunk->capacity);
        chunk->code_line_numbers = slang_reallocate(chunk->code_line_numbers, chunk->capacity);
    }

    assert(chunk->size < chunk->capacity);

    chunk->code[chunk->size] = value;
    chunk->code_line_numbers[chunk->size++] = line_number;
}

unsigned slang_chunk_get_line_number(slang_chunk *chunk, size_t instruction_index)
{
    assert(instruction_index < chunk->size);
    return chunk->code_line_numbers[instruction_index];
}

size_t slang_chunk_write_constant(slang_chunk *chunk, slang_value value)
{
    slang_value_array_write(&chunk->constants, value);
    return chunk->constants.size - 1;
}