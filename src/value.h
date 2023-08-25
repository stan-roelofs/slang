#ifndef XYZ_VALUE_H
#define XYZ_VALUE_H

#include "common.h"

typedef double xyz_value;

/// \brief A dynamic array of values
typedef struct
{
    xyz_value *values;
    size_t capacity; // total capacity
    size_t size;     // actual size of code, should always be <= capacity
} xyz_value_array;

void xyz_value_array_init(xyz_value_array *chunk);
void xyz_value_array_free(xyz_value_array *chunk);
void xyz_value_array_write(xyz_value_array *chunk, xyz_value value);

#endif