#ifndef XYZ_OPCODE_H
#define XYZ_OPCODE_H

typedef enum
{
    OP_RETURN,
    OP_CONSTANT, // 1 operand TODO: what if we have more than 256 constants?
} xyz_opcode;

#endif