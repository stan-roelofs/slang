#include "test.h"

#include "scanner.h"

START_TEST_SUITE(scanner)

ADD_TEST(init)
{
    slang_scanner scanner;
    const char *source = "test";
    slang_scanner_init(&scanner, source);

    EXPECT(scanner.token_start == source);
    EXPECT(scanner.token_current == source);
    EXPECT(scanner.line == 1);
}

END_TEST_SUITE()