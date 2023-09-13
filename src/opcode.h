#ifndef SLANG_OPCODE_H
#define SLANG_OPCODE_H

enum slang_opcode
{
    OP_RETURN,
    OP_CONSTANT, // 1 operand TODO: what if we have more than 256 constants?

    // Arithmetic
    OP_NEGATE,
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE
};

#endif