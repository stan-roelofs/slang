#include <stdio.h>

#include "test.h"

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

int main(int argc, const char *argv[])
{
    printf("Running tests...\n");

    bool result = true;
    RUN_TEST_SUITE(chunk);
    RUN_TEST_SUITE(value_array);

    if (!result)
    {
        printf("\nSome tests failed!\n");
        return EXIT_FAILURE;
    }

    printf("\nAll tests passed!\n");
    return EXIT_SUCCESS;
}