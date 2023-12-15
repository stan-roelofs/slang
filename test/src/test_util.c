#include "test.h"

#include "memory.h"
#include "util.h"

START_TEST_SUITE(util)

ADD_TEST(set_error)
{
    char *buffer = NULL;
    slang_set_error(&buffer, "Hello, %s!", "world");
    ASSERT(STRING_EQUAL(buffer, "Hello, world!"));
    slang_free(buffer);
}

ADD_TEST(append_error)
{
    char *buffer = NULL;
    slang_append_error(&buffer, "Hello, %s!", "world");
    ASSERT(STRING_EQUAL(buffer, "Hello, world!"));

    slang_append_error(&buffer, " %d", 42);
    ASSERT(STRING_EQUAL(buffer, "Hello, world! 42"));

    slang_free(buffer);
}

END_TEST_SUITE()
