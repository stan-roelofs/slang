#ifndef XYZ_DEBUG_H
#define XYZ_DEBUG_H

#include "chunk.h"
#include "value.h"
#include "vm.h"

void xyz_disassemble_chunk(struct xyz_chunk *chunk, const char *name);
void xyz_print_stack(struct xyz_vm *vm);
void xyz_print_value(xyz_value value);

#endif