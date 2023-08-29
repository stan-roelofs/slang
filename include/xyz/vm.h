#ifndef XYZ_VM_H
#define XYZ_VM_H

typedef struct xyz_vm xyz_vm;

xyz_vm *xyz_vm_new();
void xyz_vm_free(xyz_vm *vm);

typedef int xyz_run_result;

#define XYZ_RUN_OK 0
#define XYZ_COMPILE_ERROR 1
#define XYZ_RUNTIME_ERROR 2
#define XYZ_FILE_ERROR 3

xyz_run_result xyz_run_string(xyz_vm *vm, const char *source);
xyz_run_result xyz_run_file(xyz_vm *vm, const char *path);

const char *xyz_get_error(xyz_vm *vm);

#endif