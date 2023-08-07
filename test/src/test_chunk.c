#include "test.h"

#include "chunk.h"

START_TEST_SUITE(chunk)

ADD_TEST(init)
{
    struct xyz_chunk chunk;
    xyz_chunk_init(&chunk);

    EXPECT(chunk.capacity == 0);
    EXPECT(chunk.size == 0);
    EXPECT(chunk.code == NULL);
}

ADD_TEST(write)
{
    struct xyz_chunk chunk;
    xyz_chunk_init(&chunk);

    xyz_chunk_write(&chunk, 5, 3);
    EXPECT(chunk.capacity >= 1);
    ASSERT(chunk.size == 1);
    EXPECT(chunk.code[0] == 5);
    EXPECT(chunk.code_line_numbers[0] == 3);
}

ADD_TEST(capacity_grows)
{
    struct xyz_chunk chunk;
    xyz_chunk_init(&chunk);

    size_t old_capacity = chunk.capacity;
    while (chunk.size < chunk.capacity)
    {
        xyz_chunk_write(&chunk, 3, 5);
        EXPECT(chunk.code[chunk.size - 1] == 3);
        EXPECT(chunk.code_line_numbers[chunk.size - 1] == 5);
    }
    EXPECT(chunk.size == chunk.capacity);

    xyz_chunk_write(&chunk, 7, 6);
    EXPECT(chunk.capacity > old_capacity);
    EXPECT(chunk.code[chunk.size - 1] == 7);
    EXPECT(chunk.code_line_numbers[chunk.size - 1] == 6);
}

ADD_TEST(free)
{
    struct xyz_chunk chunk;
    xyz_chunk_init(&chunk);
    xyz_chunk_write(&chunk, 5, 7);
    xyz_chunk_free(&chunk);

    EXPECT(chunk.capacity == 0);
    EXPECT(chunk.size == 0);
    EXPECT(chunk.code == NULL);
}

ADD_TEST(write_constant)
{
    struct xyz_chunk chunk;
    xyz_chunk_init(&chunk);

    ASSERT(xyz_chunk_write_constant(&chunk, 5) == 0);
    EXPECT(chunk.constants.size == 1);
    EXPECT(chunk.constants.values[0] == 5);
}

END_TEST_SUITE()