#ifndef XYZ_VM_H
#define XYZ_VM_H

#include "chunk.h"
#include "value.h"

#define XYZ_STACK_SIZE 256

typedef void (*xyz_fatal_handler)(struct xyz_vm *vm, const char *message);

/// \brief The virtual machine, it runs a chunk of bytecode
struct xyz_vm
{
    struct xyz_chunk *chunk;
    uint8_t *instruction_pointer;
    xyz_value stack[XYZ_STACK_SIZE];
    xyz_value *stack_pointer;
    xyz_fatal_handler fatal_handler;
};

void xyz_vm_init(struct xyz_vm *vm);
void xyz_vm_free(struct xyz_vm *vm);
void xyz_vm_push_stack(struct xyz_vm *vm, xyz_value value);
xyz_value xyz_vm_pop_stack(struct xyz_vm *vm);

enum xyz_run_result
{
    XYZ_RUN_OK,
    XYZ_RUN_COMPILE_ERROR,
    XYZ_RUN_RUNTIME_ERROR
};

enum xyz_run_result xyz_vm_run(struct xyz_vm *vm, struct xyz_chunk *chunk);

#endif