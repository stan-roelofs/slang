#include "test.h"

#include "compiler.h"
#include "memory.h"
#include "opcode.h"

START_TEST_SUITE(compiler)

#define SETUP()               \
    slang_chunk chunk;        \
    slang_chunk_init(&chunk); \
    char *error_message = NULL;

#define TEARDOWN()            \
    slang_chunk_free(&chunk); \
    slang_free(error_message);

ADD_TEST(invalid_expression)
{
    SETUP()

    ASSERT(slang_compile(&chunk, "+", &error_message) == false);
    ASSERT(chunk.size == 0);
    ASSERT(chunk.constants.size == 0);
    ASSERT(error_message != NULL);

    TEARDOWN()
}

ADD_TEST(number)
{
    SETUP()

    ASSERT(slang_compile(&chunk, "123", &error_message) == true);
    ASSERT(chunk.size == 2);
    ASSERT(chunk.code[0] == OP_CONSTANT);
    ASSERT(chunk.code[1] == 0);
    ASSERT(chunk.constants.size == 1);
    ASSERT(chunk.constants.values[0] == 123);

    TEARDOWN()
}

ADD_TEST(unary_minus_number)
{
    SETUP()

    ASSERT(slang_compile(&chunk, "-123", &error_message) == true);
    ASSERT(chunk.size == 3);
    ASSERT(chunk.code[0] == OP_CONSTANT);
    ASSERT(chunk.code[1] == 0);
    ASSERT(chunk.code[2] == OP_NEGATE);
    ASSERT(chunk.constants.size == 1);
    ASSERT(chunk.constants.values[0] == 123);

    TEARDOWN()
}

ADD_TEST(grouping)
{
    SETUP()

    ASSERT(slang_compile(&chunk, "(123)", &error_message) == true);
    ASSERT(chunk.size == 2);
    ASSERT(chunk.code[0] == OP_CONSTANT);
    ASSERT(chunk.code[1] == 0);
    ASSERT(chunk.constants.size == 1);
    ASSERT(chunk.constants.values[0] == 123);

    TEARDOWN()
}

ADD_TEST(GROUPING_WITHOUT_CLOSING_PARENTHESIS)
{
    SETUP()

    ASSERT(slang_compile(&chunk, "(123", &error_message) == false);
    ASSERT(error_message != NULL);

    TEARDOWN()
}

END_TEST_SUITE()