#include <stdio.h>

#include "chunk.h"
#include "debug.h"
#include "opcode.h"

#include "test.h"

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

int main(int argc, const char *argv[])
{
    printf("Running tests...\n");

    bool result = true;
    RUN_TEST_SUITE(chunk);
    RUN_TEST_SUITE(value_array);

    if (!result)
    {
        printf("\nSome tests failed!\n");
        return EXIT_FAILURE;
    }

    printf("\nAll tests passed!\n");

    struct xyz_chunk chunk;
    xyz_chunk_init(&chunk);
    size_t constant = xyz_chunk_write_constant(&chunk, 1.2);
    xyz_chunk_write(&chunk, OP_CONSTANT, 123);
    xyz_chunk_write(&chunk, constant, 123);
    xyz_chunk_write(&chunk, OP_RETURN, 123);
    xyz_disassemble_chunk(&chunk, "test chunk");

    return EXIT_SUCCESS;
}