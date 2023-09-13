#include "vm_internal.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "compiler.h"
#include "debug.h"
#include "opcode.h"
#include "memory.h"
#include "util.h"

void slang_vm_push_stack(slang_vm *vm, slang_value value)
{
    assert((vm->stack_pointer - vm->stack) < SLANG_STACK_SIZE && "Stack overflow");
    *vm->stack_pointer++ = value;
}

slang_value slang_vm_pop_stack(slang_vm *vm)
{
    assert(vm->stack_pointer - vm->stack > 0 && "Stack underflow");
    return *--vm->stack_pointer;
}

static void runtime_error(slang_vm *vm, const char *message)
{
    size_t instruction = (size_t)(vm->instruction_pointer - vm->chunk->code);
    size_t line = slang_chunk_get_line_number(vm->chunk, instruction);
    fprintf(stderr, "[line %zu] Runtime error: %s\n", line, message);
    exit(1); // TODO
}

static void slang_vm_init(slang_vm *vm)
{
    vm->chunk = NULL;
    vm->instruction_pointer = NULL;
    vm->stack_pointer = vm->stack;
    vm->fatal_handler = runtime_error;
    vm->error_message = NULL;
}

slang_vm *slang_vm_new(void)
{
    slang_vm *vm = slang_allocate(sizeof(slang_vm));
    slang_vm_init(vm);
    return vm;
}

void slang_vm_free(slang_vm *vm)
{
    if (!vm)
        return;

    slang_free(vm->error_message);
    slang_free(vm);
}

static slang_run_result run(slang_vm *vm)
{
#define READ() (*vm->instruction_pointer++)
#define READ_CONSTANT() (vm->chunk->constants.values[READ()])
#define BINARY_ARITHMETIC_OP(op)                \
    do                                          \
    {                                           \
        slang_value b = slang_vm_pop_stack(vm); \
        slang_value a = slang_vm_pop_stack(vm); \
        slang_vm_push_stack(vm, a op b);        \
    } while (0)

    for (;;)
    {
#ifdef SLANG_DEBUG_TRACE_EXECUTION
        slang_disassemble_instruction(vm->chunk, (size_t)(vm->instruction_pointer - vm->chunk->code));
        slang_print_stack(vm);
#endif
        uint8_t instruction;
        switch (instruction = READ())
        {
        case OP_RETURN:
            // TODO
            return SLANG_RUN_OK;
        case OP_CONSTANT:
        {
            slang_value constant = READ_CONSTANT();
            slang_vm_push_stack(vm, constant);
            break;
        }
        case OP_NEGATE:
        {
            slang_vm_push_stack(vm, -slang_vm_pop_stack(vm));
            break;
        }
        case OP_ADD:
            BINARY_ARITHMETIC_OP(+);
            break;
        case OP_SUBTRACT:
            BINARY_ARITHMETIC_OP(-);
            break;
        case OP_MULTIPLY:
            BINARY_ARITHMETIC_OP(*);
            break;
        case OP_DIVIDE:
            BINARY_ARITHMETIC_OP(/);
            break;
        default:
            slang_vm_set_error(vm, "Unknown opcode: %d", instruction);
            return SLANG_RUNTIME_ERROR;
        }

#undef READ
#undef READ_CONSTANT
#undef BINARY_ARITHMETIC_OP
    }
}

slang_run_result slang_vm_run_chunk(slang_vm *vm, slang_chunk *chunk)
{
    vm->chunk = chunk;
    vm->instruction_pointer = vm->chunk->code;

    return run(vm);
}

//! \returns A pointer to a buffer containing the contents of the file, or NULL if an error occurred
static char *read_file(const char *path, char **error_message)
{
    FILE *file = fopen(path, "rb");
    if (file == NULL)
    {
        slang_set_error(error_message, "Could not open file: %s", path);
        return NULL;
    }

    fseek(file, 0L, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);

    char *buffer = slang_allocate(file_size + 1);
    if (buffer == NULL)
    {
        slang_set_error(error_message, "Could not allocate memory for file contents");
        fclose(file);
        return NULL;
    }

    size_t bytes_read = fread(buffer, sizeof(char), file_size, file);
    if (bytes_read < file_size)
    {
        slang_free(buffer);
        fclose(file);
        slang_set_error(error_message, "Could not read file");
        return NULL;
    }

    buffer[bytes_read] = '\0';
    fclose(file);
    return buffer;
}

slang_run_result slang_run_string(slang_vm *vm, const char *source)
{
    slang_chunk *chunk = slang_compile(source, &vm->error_message);
    if (chunk == NULL)
        return SLANG_COMPILE_ERROR;

    slang_run_result result = slang_vm_run_chunk(vm, chunk);
    slang_chunk_free(chunk);
    return result;
}

slang_run_result slang_run_file(slang_vm *vm, const char *path)
{
    char *source = read_file(path, &vm->error_message);
    if (source == NULL)
        return SLANG_FILE_ERROR;

    slang_run_result result = slang_run_string(vm, source);
    slang_free(source);
    return result;
}

const char *slang_get_error(slang_vm *vm)
{
    return vm->error_message != NULL ? vm->error_message : "";
}

void slang_vm_set_error(slang_vm *vm, const char *message, ...)
{
    va_list valist;
    va_start(valist, message);
    slang_set_errorv(&vm->error_message, message, valist);
    va_end(valist);
}