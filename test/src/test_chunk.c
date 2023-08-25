#include "test.h"

#include "chunk.h"

START_TEST_SUITE(chunk)

ADD_TEST(init)
{
    xyz_chunk chunk;
    xyz_chunk_init(&chunk);

    EXPECT(chunk.capacity == 0);
    EXPECT(chunk.size == 0);
    EXPECT(chunk.code == NULL);

    xyz_chunk_free(&chunk);
}

ADD_TEST(write)
{
    xyz_chunk chunk;
    xyz_chunk_init(&chunk);

    xyz_chunk_write(&chunk, 5, 3);
    EXPECT(chunk.capacity >= 1);
    ASSERT(chunk.size == 1);
    EXPECT(chunk.code[0] == 5);
    EXPECT(chunk.code_line_numbers[0] == 3);

    xyz_chunk_free(&chunk);
}

ADD_TEST(capacity_grows)
{
    xyz_chunk chunk;
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

    xyz_chunk_free(&chunk);
}

ADD_TEST(free)
{
    xyz_chunk chunk;
    xyz_chunk_init(&chunk);
    xyz_chunk_write(&chunk, 5, 7);
    xyz_chunk_free(&chunk);

    EXPECT(chunk.capacity == 0);
    EXPECT(chunk.size == 0);
    EXPECT(chunk.code == NULL);

    xyz_chunk_free(&chunk);
}

ADD_TEST(write_constant)
{
    xyz_chunk chunk;
    xyz_chunk_init(&chunk);

    ASSERT(xyz_chunk_write_constant(&chunk, 5) == 0);
    EXPECT(chunk.constants.size == 1);
    EXPECT(chunk.constants.values[0] == 5);

    ASSERT(xyz_chunk_write_constant(&chunk, 7) == 1);
    EXPECT(chunk.constants.size == 2);
    EXPECT(chunk.constants.values[1] == 7);

    xyz_chunk_free(&chunk);
}

END_TEST_SUITE()