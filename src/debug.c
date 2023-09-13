#include "debug.h"

#include <stdio.h>

#include "opcode.h"

#define slang_printf(...) printf(__VA_ARGS__)

void slang_print_value(slang_value value)
{
    slang_printf("%g", value);
}

static size_t slang_disassemble_instruction(slang_chunk *chunk, size_t offset)
{
    uint8_t instruction = chunk->code[offset];
    if (offset > 0 && slang_chunk_get_line_number(chunk, offset) == slang_chunk_get_line_number(chunk, offset - 1))
        slang_printf("   | ");
    else
        slang_printf("%04d ", chunk->code_line_numbers[offset]);

    switch (instruction)
    {
    case SLANG_OPCODE_RETURN:
        slang_printf("OP_RETURN\n");
        return offset + 1;
    case SLANG_OPCODE_CONSTANT:
    {
        size_t constant_index = chunk->code[offset + 1];
        slang_value constant = chunk->constants.values[constant_index];
        slang_printf("OP_CONSTANT %zu '", constant_index);
        slang_print_value(constant);
        slang_printf("'\n");
        return offset + 2;
    }
    case SLANG_OPCODE_NEGATE:
        slang_printf("OP_NEGATE\n");
        return offset + 1;
    default:
        slang_printf("Unknown opcode %d\n", instruction);
        return offset + 1;
    }
}

void slang_disassemble_chunk(slang_chunk *chunk, const char *name)
{
    slang_printf("disassembling chunk: \"%s\"\n", name);

    for (size_t offset = 0; offset < chunk->size;)
    {
        assert(offset < chunk->size);

        slang_printf("%04zu ", offset);

        offset = slang_disassemble_instruction(chunk, offset);
    }
}

void slang_print_stack(slang_vm *vm)
{
    slang_printf("          ");
    for (slang_value *i = vm->stack; i < vm->stack_pointer; i++)
    {
        slang_printf("[ ");
        slang_print_value(*i);
        slang_printf(" ]");
    }
    slang_printf("\n");
}