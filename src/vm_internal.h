#ifndef XYZ_VM_INTERNAL_H
#define XYZ_VM_INTERNAL_H

#include <xyz/vm.h>

#include "chunk.h"
#include "value.h"

#define XYZ_STACK_SIZE 256

typedef void (*xyz_fatal_handler)(struct xyz_vm *vm, const char *message);

/// \brief The virtual machine, it runs a chunk of bytecode
struct xyz_vm
{
    xyz_chunk *chunk;
    uint8_t *instruction_pointer;
    xyz_value stack[XYZ_STACK_SIZE];
    xyz_value *stack_pointer;
    xyz_fatal_handler fatal_handler;
    char *error_message;
};

void xyz_vm_push_stack(xyz_vm *vm, xyz_value value);
xyz_value xyz_vm_pop_stack(xyz_vm *vm);

void xyz_set_error(char **buffer, const char *message);

#endif