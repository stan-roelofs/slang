#ifndef XYZ_VM_H
#define XYZ_VM_H

typedef struct xyz_vm xyz_vm;

xyz_vm *xyz_vm_new();
void xyz_vm_free(xyz_vm *vm);

#endif