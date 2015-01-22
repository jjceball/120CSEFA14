#ifndef KERNELSYSCALL_H
#define KERNELSYSCALL_H

#include "system.h"
#include "syscall.h"

int kernelRead(char *buffer, int size, OpenFileId id);
int kernelWrite(char *buffer, int size, OpenFileId id);

int kernelFork(int function_ptr);

void kernelExit();

void kernelExec();
void kernelJoin();

#endif