#include "vm_internal.h"

#include <stdio.h>

#include "debug.h"

#include "opcode.h"

void xyz_vm_push_stack(xyz_vm *vm, xyz_value value)
{
    assert((vm->stack_pointer - vm->stack) < XYZ_STACK_SIZE && "Stack overflow");
    *vm->stack_pointer++ = value;
}

xyz_value xyz_vm_pop_stack(xyz_vm *vm)
{
    assert(vm->stack_pointer - vm->stack > 0 && "Stack underflow");
    return *--vm->stack_pointer;
}

static void runtime_error(xyz_vm *vm, const char *message)
{
    size_t instruction = (size_t)(vm->instruction_pointer - vm->chunk->code);
    size_t line = xyz_chunk_get_line_number(vm->chunk, instruction);
    fprintf(stderr, "[line %zu] Runtime error: %s\n", line, message);
    exit(1); // TODO
}

static void xyz_vm_init(xyz_vm *vm)
{
    vm->chunk = NULL;
    vm->instruction_pointer = NULL;
    vm->stack_pointer = vm->stack;
    vm->fatal_handler = runtime_error;
}

xyz_vm *xyz_vm_new(void)
{
    xyz_vm *vm = malloc(sizeof(xyz_vm));
    xyz_vm_init(vm);
    return vm;
}

void xyz_vm_free(xyz_vm *vm)
{
    xyz_vm_init(vm);
}

static enum xyz_run_result run(xyz_vm *vm)
{
#define READ() (*vm->instruction_pointer++)
#define READ_CONSTANT() (vm->chunk->constants.values[READ()])

    for (;;)
    {
#ifdef XYZ_DEBUG_TRACE_EXECUTION
        xyz_disassemble_instruction(vm->chunk, (size_t)(vm->instruction_pointer - vm->chunk->code));
        xyz_print_stack(vm);
#endif
        uint8_t instruction;
        switch (instruction = *vm->instruction_pointer++)
        {
        case XYZ_OPCODE_RETURN:
            // TODO
            xyz_print_value(xyz_vm_pop_stack(vm));
            printf("\n");
            return XYZ_RUN_OK;
        case XYZ_OPCODE_CONSTANT:
        {
            xyz_value constant = READ_CONSTANT();
            xyz_vm_push_stack(vm, constant);
            break;
        }
        case XYZ_OPCODE_NEGATE:
        {
            xyz_vm_push_stack(vm, -xyz_vm_pop_stack(vm));
            break;
        }
        }
    }

#undef READ
#undef READ_CONSTANT
}

enum xyz_interpret_result xyz_vm_run(xyz_vm *vm, xyz_chunk *chunk)
{
    vm->chunk = chunk;
    vm->instruction_pointer = vm->chunk->code;

    return run(vm);
}