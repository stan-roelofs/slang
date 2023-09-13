#include "test.h"

#include "vm_internal.h"

START_TEST_SUITE(vm)

ADD_TEST(init)
{
    slang_vm *vm = slang_vm_new();
    EXPECT(vm->instruction_pointer == NULL);
    EXPECT(vm->stack_pointer == vm->stack);
    EXPECT(vm->chunk == NULL);

    slang_vm_free(vm);
}

ADD_TEST(push_stack)
{
    slang_vm *vm = slang_vm_new();

    slang_vm_push_stack(vm, 5);
    EXPECT(vm->stack_pointer == vm->stack + 1);
    EXPECT(vm->stack[0] == 5);

    slang_vm_free(vm);
}

ADD_TEST(stack_max)
{
    slang_vm *vm = slang_vm_new();

    for (size_t i = 0; i < SLANG_STACK_SIZE; i++)
        slang_vm_push_stack(vm, 5);
    EXPECT(vm->stack_pointer == vm->stack + SLANG_STACK_SIZE);
    EXPECT(vm->stack[SLANG_STACK_SIZE - 1] == 5);

    for (size_t i = 0; i < SLANG_STACK_SIZE; i++)
        EXPECT(slang_vm_pop_stack(vm) == 5);
    EXPECT(vm->stack_pointer == vm->stack);

    slang_vm_free(vm);
}

ADD_TEST(pop_stack)
{
    slang_vm *vm = slang_vm_new();

    slang_vm_push_stack(vm, 5);
    slang_vm_push_stack(vm, 7);
    EXPECT(slang_vm_pop_stack(vm) == 7);
    EXPECT(slang_vm_pop_stack(vm) == 5);

    slang_vm_free(vm);
}

ADD_TEST(free)
{
    slang_vm *vm = slang_vm_new();
    slang_vm_push_stack(vm, 5);
    slang_vm_free(vm);
}

END_TEST_SUITE()