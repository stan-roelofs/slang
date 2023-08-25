#ifndef XYZ_OPCODE_H
#define XYZ_OPCODE_H

enum xyz_opcode
{
    XYZ_OPCODE_RETURN,
    XYZ_OPCODE_CONSTANT, // 1 operand TODO: what if we have more than 256 constants?
    XYZ_OPCODE_NEGATE,
};

#endif