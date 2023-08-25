#include <xyz/vm.h>

int main(int argc, const char *argv[])
{
    xyz_vm *vm = xyz_vm_new();

    xyz_vm_free(vm);
}