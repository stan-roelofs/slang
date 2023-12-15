#ifndef SLANG_VM_H
#define SLANG_VM_H

typedef struct slang_vm slang_vm;

slang_vm *slang_vm_new();
void slang_vm_free(slang_vm *vm);

typedef enum
{
    SLANG_RUN_OK,
    SLANG_COMPILE_ERROR,
    SLANG_RUNTIME_ERROR,
    SLANG_FILE_ERROR
} slang_run_result;

/**
 * Compiles a string into bytecode and runs it.
 * @param vm The VM to run the bytecode on.
 * @param source The source code to compile. This must be a null-terminated string.
 * @return SLANG_RUN_OK if the code ran successfully.
 * @return SLANG_COMPILE_ERROR if there was a compilation error.
 * @return SLANG_RUNTIME_ERROR if there was a runtime error.
 */
slang_run_result slang_run_string(slang_vm *vm, const char *source);

/**
 * Compiles a file into bytecode and runs it.
 * @param vm The VM to run the bytecode on.
 * @param path The path to the file to compile. This must be a null-terminated string.
 * @returns SLANG_RUN_OK if the code ran successfully.
 * @returns SLANG_COMPILE_ERROR if there was a compilation error.
 * @returns SLANG_RUNTIME_ERROR if there was a runtime error.
 * @returns SLANG_FILE_ERROR if the file could not be opened.
 */
slang_run_result slang_run_file(slang_vm *vm, const char *path);

const char *slang_get_error(slang_vm *vm);

#endif