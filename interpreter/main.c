#include <xyz/vm.h>

#include <stdio.h>

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

static void repl(xyz_vm *vm)
{
    for (;;)
    {
        printf("> ");

        char line[1024];
        if (!fgets(line, sizeof(line), stdin))
        {
            printf("\n");
            break;
        }

        xyz_run_result result = xyz_run_string(vm, line);
        if (result != XYZ_RUN_OK)
        {
            printf("Error: %s\n", xyz_get_error(vm));
            break;
        }
    }
}

static void usage(const char *program_name)
{
    printf("Usage: %s [path]\n", program_name);
    printf("If no path is given, the program will run in REPL mode.\n");
}

int main(int argc, const char *argv[])
{
    xyz_vm *vm = xyz_vm_new();
    int result = EXIT_SUCCESS;

    if (argc <= 1)
    {
        repl(vm);
    }
    else if (argc == 2)
    {
        xyz_run_result result = xyz_run_file(vm, argv[1]);
        if (result != XYZ_RUN_OK)
        {
            printf("Error: %s\n", xyz_get_error(vm));
            result = EXIT_FAILURE;
        }
    }
    else
    {
        usage(argv[0]);
        result = EXIT_FAILURE;
    }

    xyz_vm_free(vm);
    return result;
}