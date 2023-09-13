#ifndef SLANG_DEBUG_H
#define SLANG_DEBUG_H

#include "chunk.h"
#include "value.h"
#include "vm_internal.h"

void slang_disassemble_chunk(slang_chunk *chunk, const char *name);
void slang_print_stack(slang_vm *vm);
void slang_print_value(slang_value value);

#endif