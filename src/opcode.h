#ifndef SLANG_OPCODE_H
#define SLANG_OPCODE_H

enum slang_opcode
{
    SLANG_OPCODE_RETURN,
    SLANG_OPCODE_CONSTANT, // 1 operand TODO: what if we have more than 256 constants?
    SLANG_OPCODE_NEGATE,
};

#endif