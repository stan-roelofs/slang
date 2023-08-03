#include <stdio.h>

#include "chunk.h"
#include "debug.h"
#include "opcode.h"

int main(int argc, const char *argv[])
{
    xyz_chunk chunk;
    xyz_chunk_init(&chunk);
    xyz_chunk_write(&chunk, (uint8_t)OP_RETURN);

    xyz_disassemble_chunk(&chunk, "test chunk");

    xyz_chunk_free(&chunk);
}