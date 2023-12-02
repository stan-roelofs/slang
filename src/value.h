#ifndef SLANG_VALUE_H
#define SLANG_VALUE_H

#include <stdint.h>

typedef double slang_value;

/// \brief A dynamic array of values
typedef struct
{
    slang_value *values;
    size_t capacity; // total capacity
    size_t size;     // actual size of code, should always be <= capacity
} slang_value_array;

void slang_value_array_init(slang_value_array *chunk);
void slang_value_array_free(slang_value_array *chunk);
void slang_value_array_write(slang_value_array *chunk, slang_value value);

#endif