#include "test.h"

#include "vm_internal.h"

START_TEST_SUITE(vm)

ADD_TEST(init)
{
    xyz_vm *vm = xyz_vm_new();
    EXPECT(vm->instruction_pointer == NULL);
    EXPECT(vm->stack_pointer == vm->stack);
    EXPECT(vm->chunk == NULL);

    xyz_vm_free(vm);
}

ADD_TEST(push_stack)
{
    xyz_vm *vm = xyz_vm_new();

    xyz_vm_push_stack(vm, 5);
    EXPECT(vm->stack_pointer == vm->stack + 1);
    EXPECT(vm->stack[0] == 5);

    xyz_vm_free(vm);
}

ADD_TEST(stack_max)
{
    xyz_vm *vm = xyz_vm_new();

    for (size_t i = 0; i < XYZ_STACK_SIZE; i++)
        xyz_vm_push_stack(vm, 5);
    EXPECT(vm->stack_pointer == vm->stack + XYZ_STACK_SIZE);
    EXPECT(vm->stack[XYZ_STACK_SIZE - 1] == 5);

    for (size_t i = 0; i < XYZ_STACK_SIZE; i++)
        EXPECT(xyz_vm_pop_stack(vm) == 5);
    EXPECT(vm->stack_pointer == vm->stack);

    xyz_vm_free(vm);
}

ADD_TEST(pop_stack)
{
    xyz_vm *vm = xyz_vm_new();

    xyz_vm_push_stack(vm, 5);
    xyz_vm_push_stack(vm, 7);
    EXPECT(xyz_vm_pop_stack(vm) == 7);
    EXPECT(xyz_vm_pop_stack(vm) == 5);

    xyz_vm_free(vm);
}

ADD_TEST(free)
{
    xyz_vm *vm = xyz_vm_new();
    xyz_vm_push_stack(vm, 5);
    xyz_vm_free(vm);

    EXPECT(vm->instruction_pointer == NULL);
    EXPECT(vm->stack_pointer == vm->stack);
    EXPECT(vm->chunk == NULL);
}

END_TEST_SUITE()