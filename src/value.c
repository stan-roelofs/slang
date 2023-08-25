#include "value.h"

#include "memory.h"

#define MINIMUM_CAPACITY 32
static_assert(((size_t)(MINIMUM_CAPACITY * 1.5)) != MINIMUM_CAPACITY, "Minimum capacity is too small");
#define GROW_VALUE_ARRAY(current_capacity) (((size_t)((current_capacity) < MINIMUM_CAPACITY ? MINIMUM_CAPACITY : (current_capacity)*1.5)))

void xyz_value_array_init(xyz_value_array *values)
{
    values->size = 0;
    values->capacity = 0;
    values->values = NULL;
}

void xyz_value_array_free(xyz_value_array *values)
{
    free(values->values);
    xyz_value_array_init(values);
}

void xyz_value_array_write(xyz_value_array *values, xyz_value value)
{
    if (values->size == values->capacity)
    {
        values->capacity = GROW_VALUE_ARRAY(values->capacity);
        values->values = xyz_reallocate(values->values, values->capacity);
    }

    assert(values->size < values->capacity);

    values->values[values->size++] = value;
}