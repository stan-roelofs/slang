#include "debug.h"

#include <stdio.h>

#include "opcode.h"

#define xyz_printf(...) printf(__VA_ARGS__)

static void xyz_print_value(xyz_value value)
{
    xyz_printf("%g", value);
}

static size_t xyz_disassemble_instruction(xyz_chunk *chunk, size_t offset)
{
    uint8_t instruction = chunk->code[offset];
    if (offset > 0 && xyz_chunk_get_line_number(chunk, offset) == xyz_chunk_get_line_number(chunk, offset - 1))
        xyz_printf("   | ");
    else
        xyz_printf("%04d ", chunk->code_line_numbers[offset]);

    switch (instruction)
    {
    case OP_RETURN:
        xyz_printf("OP_RETURN\n");
        return offset + 1;
    case OP_CONSTANT:
    {
        size_t constant_index = chunk->code[offset + 1];
        xyz_value constant = chunk->constants.values[constant_index];
        xyz_printf("OP_CONSTANT %zu '", constant_index);
        xyz_print_value(constant);
        xyz_printf("'\n");
        return offset + 2;
    }
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