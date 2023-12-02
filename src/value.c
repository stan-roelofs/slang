#include "value.h"

#include <assert.h>

#include "memory.h"

#define MINIMUM_CAPACITY 32
#define GROW_VALUE_ARRAY(current_capacity) (((size_t)((current_capacity) < MINIMUM_CAPACITY ? MINIMUM_CAPACITY : (current_capacity) * 1.5)))

void slang_value_array_init(slang_value_array *values)
{
    values->size = 0;
    values->capacity = 0;
    values->values = NULL;
}

void slang_value_array_free(slang_value_array *values)
{
    slang_free(values->values);
    slang_value_array_init(values);
}

void slang_value_array_write(slang_value_array *values, slang_value value)
{
    if (values->size == values->capacity)
    {
        values->capacity = GROW_VALUE_ARRAY(values->capacity);
        values->values = slang_reallocate(values->values, values->capacity);
    }

    assert(values->size < values->capacity);

    values->values[values->size++] = value;
}