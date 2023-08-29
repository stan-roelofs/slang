#include "vm_internal.h"

#include <stdio.h>
#include <string.h>

#include "compiler.h"
#include "debug.h"
#include "opcode.h"
#include "memory.h"

void xyz_vm_push_stack(xyz_vm *vm, xyz_value value)
{
    assert((vm->stack_pointer - vm->stack) < XYZ_STACK_SIZE && "Stack overflow");
    *vm->stack_pointer++ = value;
}

xyz_value xyz_vm_pop_stack(xyz_vm *vm)
{
    assert(vm->stack_pointer - vm->stack > 0 && "Stack underflow");
    return *--vm->stack_pointer;
}

static void runtime_error(xyz_vm *vm, const char *message)
{
    size_t instruction = (size_t)(vm->instruction_pointer - vm->chunk->code);
    size_t line = xyz_chunk_get_line_number(vm->chunk, instruction);
    fprintf(stderr, "[line %zu] Runtime error: %s\n", line, message);
    exit(1); // TODO
}

static void xyz_vm_init(xyz_vm *vm)
{
    vm->chunk = NULL;
    vm->instruction_pointer = NULL;
    vm->stack_pointer = vm->stack;
    vm->fatal_handler = runtime_error;
    vm->error_message = NULL;
}

xyz_vm *xyz_vm_new(void)
{
    xyz_vm *vm = xyz_allocate(sizeof(xyz_vm));
    xyz_vm_init(vm);
    return vm;
}

void xyz_vm_free(xyz_vm *vm)
{
    if (!vm)
        return;

    xyz_chunk_free(vm->chunk);
    xyz_free(vm->error_message);
    xyz_free(vm);
}

static enum xyz_run_result run(xyz_vm *vm)
{
#define READ() (*vm->instruction_pointer++)
#define READ_CONSTANT() (vm->chunk->constants.values[READ()])

    for (;;)
    {
#ifdef XYZ_DEBUG_TRACE_EXECUTION
        xyz_disassemble_instruction(vm->chunk, (size_t)(vm->instruction_pointer - vm->chunk->code));
        xyz_print_stack(vm);
#endif
        uint8_t instruction;
        switch (instruction = *vm->instruction_pointer++)
        {
        case XYZ_OPCODE_RETURN:
            // TODO
            xyz_print_value(xyz_vm_pop_stack(vm));
            printf("\n");
            return XYZ_RUN_OK;
        case XYZ_OPCODE_CONSTANT:
        {
            xyz_value constant = READ_CONSTANT();
            xyz_vm_push_stack(vm, constant);
            break;
        }
        case XYZ_OPCODE_NEGATE:
        {
            xyz_vm_push_stack(vm, -xyz_vm_pop_stack(vm));
            break;
        }
        }
    }

#undef READ
#undef READ_CONSTANT
}

static xyz_run_result vm_run_chunk(xyz_vm *vm, xyz_chunk *chunk)
{
    vm->chunk = chunk;
    vm->instruction_pointer = vm->chunk->code;

    return run(vm);
}

//! \returns A pointer to a buffer containing the contents of the file, or NULL if an error occurred
static char *read_file(const char *path, const char **error_message)
{
    FILE *file = fopen(path, "rb");
    if (file == NULL)
    {
        xyz_set_error(error_message, "Could not open file");
        return NULL;
    }

    fseek(file, 0L, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);

    char *buffer = xyz_allocate(file_size + 1);
    if (buffer == NULL)
    {
        xyz_set_error(error_message, "Could not allocate memory for file contents");
        fclose(file);
        return NULL;
    }

    size_t bytes_read = fread(buffer, sizeof(char), file_size, file);
    if (bytes_read < file_size)
    {
        xyz_free(buffer);
        fclose(file);
        xyz_set_error(error_message, "Could not read file");
        return NULL;
    }

    buffer[bytes_read] = '\0';
    fclose(file);
    return buffer;
}

xyz_run_result xyz_run_string(xyz_vm *vm, const char *source)
{
    xyz_chunk *chunk = xyz_compile(source, &vm->error_message);
    if (chunk == NULL)
        return XYZ_COMPILE_ERROR;

    xyz_run_result result = vm_run_chunk(vm, chunk);
    xyz_chunk_free(chunk);
    return result;
}

xyz_run_result xyz_run_file(xyz_vm *vm, const char *path)
{
    char *source = read_file(path, &vm->error_message);
    if (source == NULL)
        return XYZ_FILE_ERROR;

    xyz_run_result result = xyz_run_string(vm, source);
    xyz_free(source);
    return result;
}

const char *xyz_get_error(xyz_vm *vm)
{
    return vm->error_message != NULL ? vm->error_message : "";
}

void xyz_set_error(char **buffer, const char *message)
{
    *buffer = xyz_reallocate(*buffer, strlen(message) + 1);
    strcpy((char *)(*buffer), message);
}