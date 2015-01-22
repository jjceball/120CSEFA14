#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H

class Thread;
class Lock;
#include "synch.h"


class ProcessManager{
public:
    ProcessManager(int s); // Table in description	
    ~ProcessManager();	
    int Alloc(void *obj);
    void *Get(int index);
    void Release(int index);

private:
    void **inArray;
    Lock * lock;
    int arrSize;
};

#endif
