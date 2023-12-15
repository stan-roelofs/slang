#ifndef SLANG_COMPILER_H
#define SLANG_COMPILER_H

#include <stdbool.h>

#include "chunk.h"

bool slang_compile(slang_chunk *chunk, const char *source, char **error_message);

#endif