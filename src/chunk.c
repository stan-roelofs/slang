#include "chunk.h"

#include "memory.h"

#define MINIMUM_CAPACITY 32
static_assert(((size_t)(MINIMUM_CAPACITY * 1.5)) != MINIMUM_CAPACITY, "Minimum capacity is too small");
#define GROW_CHUNK(current_capacity) (((size_t)((current_capacity) < MINIMUM_CAPACITY ? MINIMUM_CAPACITY : (current_capacity)*1.5)))

void xyz_chunk_init(xyz_chunk *chunk)
{
    chunk->size = 0;
    chunk->capacity = 0;
    chunk->code = NULL;
    xyz_value_array_init(&chunk->constants);
}

void xyz_chunk_free(xyz_chunk *chunk)
{
    free(chunk->code);
    xyz_value_array_free(&chunk->constants);
    xyz_chunk_init(chunk);
}

void xyz_chunk_write(xyz_chunk *chunk, uint8_t value)
{
    if (chunk->size == chunk->capacity)
    {
        chunk->capacity = GROW_CHUNK(chunk->capacity);
        chunk->code = xyz_reallocate(chunk->code, chunk->capacity);
    }

    assert(chunk->size < chunk->capacity);

    chunk->code[chunk->size++] = value;
}

size_t xyz_chunk_write_constant(xyz_chunk *chunk, xyz_value value)
{
    xyz_value_array_write(&chunk->constants, value);
    return chunk->constants.size - 1;
}