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

END_TEST_SUITE()
