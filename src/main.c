#include <stdio.h>

static void usage(const char *program_name)
{
    printf("Usage: %s script", program_name);
}

int main(int argc, const char *argv[])
{
    if (argc != 2)
    {
        usage(argv[0]);
        return 1;
    }

    return 0;
}