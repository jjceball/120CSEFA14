#include "processmanager.h"
#include "system.h"

//constructor
ProcessManager::ProcessManager(int s) 
{
    inArray = (void**) new int[s];
    this->arrSize = s;
    for( int i = 0; i < s; i++) 
    {
        inArray[i] = NULL;
    }
    this->lock = new Lock("process manager is locked?");
}
//Deconstructor
ProcessManager::~ProcessManager()
{
    for(int i = 0; i < arrSize; i++) 
    {
        delete (int*)inArray[i];
    }
    delete inArray;
    delete lock;
}

//return the index for spid +1 or 0 if it can¿t be assigned
int ProcessManager::Alloc(void *obj)
{
    lock->Acquire();
    int spid = 0;
    for( int i = 0; i < arrSize; i++) 
    {
        if(inArray[i] == NULL) 
	{
            inArray[i] = obj;
            spid = i + 1;
            break;
        }
    }
    lock->Release();
    return spid;
}

void * ProcessManager::Get(int index)
{
    lock->Acquire();
    void * proc;
    if(index >= 0 && index < arrSize) 
    {
    	proc = inArray[index];
    }
    else { proc = NULL; }
    lock->Release();
    return proc;
}

void ProcessManager::Release(int index)
{
    lock->Acquire();
    if(index >= 0 && index < arrSize) {
        inArray[index-1] = NULL;
    }
    lock->Release();
}