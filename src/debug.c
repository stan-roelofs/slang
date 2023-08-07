#include "debug.h"

#include <stdio.h>

#include "opcode.h"

#define xyz_printf(...) printf(__VA_ARGS__)

void xyz_print_value(xyz_value value)
{
    xyz_printf("%g", value);
}

static size_t xyz_disassemble_instruction(struct xyz_chunk *chunk, size_t offset)
{
    uint8_t instruction = chunk->code[offset];
    if (offset > 0 && xyz_chunk_get_line_number(chunk, offset) == xyz_chunk_get_line_number(chunk, offset - 1))
        xyz_printf("   | ");
    else
        xyz_printf("%04d ", chunk->code_line_numbers[offset]);

    switch (instruction)
    {
    case XYZ_OPCODE_RETURN:
        xyz_printf("OP_RETURN\n");
        return offset + 1;
    case XYZ_OPCODE_CONSTANT:
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

void xyz_disassemble_chunk(struct xyz_chunk *chunk, const char *name)
{
    xyz_printf("disassembling chunk: \"%s\"\n", name);

    for (size_t offset = 0; offset < chunk->size;)
    {
        assert(offset < chunk->size);

        xyz_printf("%04zu ", offset);

        offset = xyz_disassemble_instruction(chunk, offset);
    }
}

void xyz_print_stack(struct xyz_vm *vm)
{
    xyz_printf("          ");
    for (xyz_value *i = vm->stack; i < vm->stack_pointer; i++)
    {
        xyz_printf("[ ");
        xyz_print_value(*i);
        xyz_printf(" ]");
    }
    xyz_printf("\n");
}