#include "ksyscall.h"
#include "syscall.h"
#include "syscommon.h"
#include "system.h"
#include <stdio.h>
#include "synchconsole.h"

int read(char *buffer, int size, OpenFileId id) {
    int i;
    unsigned char kernel_char;
    if (id == ConsoleInput) {
        //reading from the buffer char by char
        for (i = 0; i < size; i++) {
            kernel_char = synchconsole->GetChar();
            kernToUserMemCopy((int)(buffer + i), &kernel_char, 1);
        }
    } else {
        ASSERT(false);
        //TODO: call OS filesys/openfile.h
    }
    return size;
}

// Buffer is the pointer in User-level memory
void write(char *buffer, int size, OpenFileId id) {
    int i;
    int copied_size;
    unsigned char kernel_char;
    if (id == ConsoleOutput) {
        //reading from the buffer char by char
        for (i = 0; i < size; i++) {
            copied_size = userToKernMemCopy(&kernel_char, (int)(buffer + i), 1);
            if (copied_size != 1) {
                ASSERT(false);
            }
            synchconsole->PutChar(kernel_char);
        }
    } else {
        ASSERT(false);
        //TODO: call OS filesys/openfile.h
    }
}

