#include "test.h"

#include <math.h>

#include "value.h"

START_TEST_SUITE(value_array)

ADD_TEST(init)
{
    struct xyz_value_array value_array;
    xyz_value_array_init(&value_array);

    EXPECT(value_array.capacity == 0);
    EXPECT(value_array.size == 0);
    EXPECT(value_array.values == NULL);

    xyz_value_array_free(&value_array);
}

ADD_TEST(write)
{
    struct xyz_value_array value_array;
    xyz_value_array_init(&value_array);

    xyz_value_array_write(&value_array, 5);
    EXPECT(value_array.capacity >= 1);
    ASSERT(value_array.size == 1);
    EXPECT(FLOAT_EQUAL(value_array.values[0], 5));

    xyz_value_array_free(&value_array);
}

ADD_TEST(capacity_grows)
{
    struct xyz_value_array value_array;
    xyz_value_array_init(&value_array);

    size_t old_capacity = value_array.capacity;
    while (value_array.size < value_array.capacity)
    {
        xyz_value_array_write(&value_array, 3);
        EXPECT(value_array.values[value_array.size - 1] == 3);
    }
    EXPECT(value_array.size == value_array.capacity);

    xyz_value_array_write(&value_array, 7);
    EXPECT(value_array.capacity > old_capacity);
    EXPECT(value_array.values[value_array.size - 1] == 7);

    xyz_value_array_free(&value_array);
}

ADD_TEST(free)
{
    struct xyz_value_array value_array;
    xyz_value_array_init(&value_array);
    xyz_value_array_write(&value_array, 5);
    xyz_value_array_free(&value_array);

    EXPECT(value_array.capacity == 0);
    EXPECT(value_array.size == 0);
    EXPECT(value_array.values == NULL);
}

END_TEST_SUITE()