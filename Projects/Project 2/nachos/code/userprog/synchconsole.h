#ifndef SYNCHCONSOLE_H
#define SYNCHCONSOLE_H

#include "console.h"
#include "synch.h"

class SynchConsole {

private:
    Console* console;
    Semaphore *read_semaphore;
    Lock *read_lock;
    Semaphore *write_semaphore;
    Lock *write_lock;

public:
    SynchConsole(char* in, char* out);
    ~SynchConsole();

    char GetChar();
    void PutChar(char);
    void ReadDone();
    void WriteDone();

};

void test();

#endif
