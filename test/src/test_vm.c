#include "test.h"

#include <string.h>

#include "chunk.h"
#include "opcode.h"
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

ADD_TEST(set_get_error)
{
    slang_vm *vm = slang_vm_new();
    EXPECT(vm->error_message == NULL);
    slang_vm_set_error(vm, "error");
    ASSERT(vm->error_message != NULL)
    EXPECT(STRING_EQUAL(slang_get_error(vm), "error"));
    slang_vm_free(vm);
}

ADD_TEST(op_return)
{
    slang_vm *vm = slang_vm_new();
    slang_chunk chunk;
    slang_chunk_init(&chunk);

    slang_chunk_write(&chunk, OP_RETURN, 1);

    slang_run_result result = slang_vm_run_chunk(vm, &chunk);
    EXPECT(result == SLANG_RUN_OK);

    slang_chunk_free(&chunk);
    slang_vm_free(vm);
}

ADD_TEST(op_constant)
{
    slang_vm *vm = slang_vm_new();
    slang_chunk chunk;
    slang_chunk_init(&chunk);

    slang_chunk_write(&chunk, OP_CONSTANT, 1);
    slang_chunk_write(&chunk, 0, 1);
    slang_chunk_write_constant(&chunk, 5);
    slang_chunk_write(&chunk, OP_RETURN, 1);

    EXPECT(SLANG_RUN_OK == slang_vm_run_chunk(vm, &chunk));
    EXPECT(slang_vm_pop_stack(vm) == 5);

    slang_chunk_free(&chunk);
    slang_vm_free(vm);
}

ADD_TEST(op_negate)
{
    slang_vm *vm = slang_vm_new();
    slang_chunk chunk;
    slang_chunk_init(&chunk);

    slang_chunk_write(&chunk, OP_NEGATE, 1);
    slang_vm_push_stack(vm, 5);
    slang_chunk_write(&chunk, OP_RETURN, 1);

    EXPECT(SLANG_RUN_OK == slang_vm_run_chunk(vm, &chunk));
    EXPECT(slang_vm_pop_stack(vm) == -5);

    slang_chunk_free(&chunk);
    slang_vm_free(vm);
}

ADD_TEST(op_add)
{
    slang_vm *vm = slang_vm_new();
    slang_chunk chunk;
    slang_chunk_init(&chunk);

    slang_chunk_write(&chunk, OP_ADD, 1);
    slang_vm_push_stack(vm, 5);
    slang_vm_push_stack(vm, 7);
    slang_chunk_write(&chunk, OP_RETURN, 1);

    EXPECT(SLANG_RUN_OK == slang_vm_run_chunk(vm, &chunk));
    EXPECT(slang_vm_pop_stack(vm) == 12);

    slang_chunk_free(&chunk);
    slang_vm_free(vm);
}

ADD_TEST(op_subtract)
{
    slang_vm *vm = slang_vm_new();
    slang_chunk chunk;
    slang_chunk_init(&chunk);

    slang_chunk_write(&chunk, OP_SUBTRACT, 1);
    slang_vm_push_stack(vm, 5);
    slang_vm_push_stack(vm, 7);
    slang_chunk_write(&chunk, OP_RETURN, 1);

    EXPECT(SLANG_RUN_OK == slang_vm_run_chunk(vm, &chunk));
    EXPECT(slang_vm_pop_stack(vm) == -2);

    slang_chunk_free(&chunk);
    slang_vm_free(vm);
}

ADD_TEST(op_multiply)
{
    slang_vm *vm = slang_vm_new();
    slang_chunk chunk;
    slang_chunk_init(&chunk);

    slang_chunk_write(&chunk, OP_MULTIPLY, 1);
    slang_vm_push_stack(vm, 5);
    slang_vm_push_stack(vm, 7);
    slang_chunk_write(&chunk, OP_RETURN, 1);

    EXPECT(SLANG_RUN_OK == slang_vm_run_chunk(vm, &chunk));
    EXPECT(slang_vm_pop_stack(vm) == 35);

    slang_chunk_free(&chunk);
    slang_vm_free(vm);
}

ADD_TEST(op_divide)
{
    slang_vm *vm = slang_vm_new();
    slang_chunk chunk;
    slang_chunk_init(&chunk);

    slang_chunk_write(&chunk, OP_DIVIDE, 1);
    slang_vm_push_stack(vm, 4);
    slang_vm_push_stack(vm, 2);
    slang_chunk_write(&chunk, OP_RETURN, 1);

    EXPECT(SLANG_RUN_OK == slang_vm_run_chunk(vm, &chunk));
    EXPECT(slang_vm_pop_stack(vm) == 2);

    slang_chunk_free(&chunk);
    slang_vm_free(vm);
}

// TODO run string
// TODO run file

END_TEST_SUITE()