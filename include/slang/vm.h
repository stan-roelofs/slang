#ifndef SLANG_VM_H
#define SLANG_VM_H

typedef struct slang_vm slang_vm;

slang_vm *slang_vm_new();
void slang_vm_free(slang_vm *vm);

typedef int slang_run_result;

#define SLANG_RUN_OK 0
#define SLANG_COMPILE_ERROR 1
#define SLANG_RUNTIME_ERROR 2
#define SLANG_FILE_ERROR 3

slang_run_result slang_run_string(slang_vm *vm, const char *source);
slang_run_result slang_run_file(slang_vm *vm, const char *path);

const char *slang_get_error(slang_vm *vm);

#endif