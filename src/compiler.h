#ifndef XYZ_COMPILER_H
#define XYZ_COMPILER_H

#include "chunk.h"

xyz_chunk *xyz_compile(const char *source, const char **error_message);

#endif