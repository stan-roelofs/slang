#ifndef XYZ_VM_H
#define XYZ_VM_H

#include "chunk.h"

struct xyz_vm
{
    struct xyz_chunk *chunk;
    uint8_t *instruction_pointer;
};

#endif