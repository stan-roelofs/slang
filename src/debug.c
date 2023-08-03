#include "debug.h"

#include <stdio.h>

#include "opcode.h"

#define xyz_printf(...) printf(__VA_ARGS__)

static size_t xyz_disassemble_instruction(xyz_chunk *chunk, size_t offset)
{
    uint8_t instruction = chunk->code[offset];
    switch (instruction)
    {
    case OP_RETURN:
        xyz_printf("OP_RETURN\n");
        return offset + 1;
    default:
        xyz_printf("Unknown opcode %d\n", instruction);
        return offset + 1;
    }
}

void xyz_disassemble_chunk(xyz_chunk *chunk, const char *name)
{
    xyz_printf("disassembling chunk: \"%s\"\n", name);

    for (size_t offset = 0; offset < chunk->size;)
    {
        assert(offset < chunk->size);

        xyz_printf("%04zu ", offset);

        offset = xyz_disassemble_instruction(chunk, offset);
    }
}