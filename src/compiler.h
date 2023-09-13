#ifndef SLANG_COMPILER_H
#define SLANG_COMPILER_H

#include "chunk.h"

slang_chunk *slang_compile(const char *source, char **error_message);

#endif