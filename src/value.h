#ifndef XYZ_VALUE_H
#define XYZ_VALUE_H

#include "common.h"

typedef double xyz_value;

/// \brief A dynamic array of values
struct xyz_value_array
{
    xyz_value *values;
    size_t capacity; // total capacity
    size_t size;     // actual size of code, should always be <= capacity
};

void xyz_value_array_init(struct xyz_value_array *chunk);
void xyz_value_array_free(struct xyz_value_array *chunk);
void xyz_value_array_write(struct xyz_value_array *chunk, xyz_value value);

#endif