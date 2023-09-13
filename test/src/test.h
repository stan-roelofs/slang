#ifndef SLANG_TEST_H
#define SLANG_TEST_H

#include <stdbool.h>
#include <stdio.h>

#define ASSERT(condition)                                                       \
    if (!(condition))                                                           \
    {                                                                           \
        printf("Assert failed at %s:%d: %s\n", __FILE__, __LINE__, #condition); \
        result = false;                                                         \
        break;                                                                  \
    }

#define EXPECT(condition)                                                       \
    if (!(condition))                                                           \
    {                                                                           \
        printf("Expect failed at %s:%d: %s\n", __FILE__, __LINE__, #condition); \
        result = false;                                                         \
    }

#define FLOAT_EQUAL(a, b) (fabs((a) - (b)) < 0.00001)

#define TEST_PASS() return true;

#define RUN_TEST_SUITE(name)                           \
    extern bool suite_##name();                        \
    printf("\n## Running test suite: %s ##\n", #name); \
    result &= suite_##name();

#define START_TEST_SUITE(name) \
    bool suite_##name(void)    \
    {                          \
        bool result = true;

#define ADD_TEST(test_name)                   \
    printf("Running test: %s\n", #test_name); \
    bool result_##test_name = true;           \
    for (int i = 0; i < 1; ++i)

#define END_TEST_SUITE() \
    return result;       \
    }
#endif