#ifndef SLANG_VM_INTERNAL_H
#define SLANG_VM_INTERNAL_H

#include <slang/vm.h>

#include "chunk.h"
#include "value.h"

#define SLANG_STACK_SIZE 256

typedef void (*slang_fatal_handler)(struct slang_vm *vm, const char *message);

/// \brief The virtual machine, it runs a chunk of bytecode
struct slang_vm
{
    slang_chunk *chunk;
    uint8_t *instruction_pointer;
    slang_value stack[SLANG_STACK_SIZE];
    slang_value *stack_pointer;
    slang_fatal_handler fatal_handler;
    char *error_message;
};

void slang_vm_push_stack(slang_vm *vm, slang_value value);
slang_value slang_vm_pop_stack(slang_vm *vm);

#endif