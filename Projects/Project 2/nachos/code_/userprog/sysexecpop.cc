#include "ksyscall.h"
#include "system.h"
#include "thread.h"
#include "syscommon.h"
#include "addrspace.h"
#include "processmanager.h"

void
StartUserProcess(int argv) {
    currentThread->space->InitRegisters();
    currentThread->space->RestoreState();
    machine->Run();
    ASSERT(false);
}

void kernExec() {
    int * passArgv;
    unsigned char * path;
    Thread * t;
    OpenFile * executable;
    AddrSpace * space;
    int spid; 
    int srcPath = machine->ReadRegister(4);
    int argc = machine->ReadRegister(5);
    int argv = machine->ReadRegister(6);
    int option = machine->ReadRegister(7);
    // check file
    if(!fileAddress((char*)srcPath)){
        ASSERT(false);
    }
    length = userStringLength((int)srcPath);
    path = new unsigned char[length+1];
    userToKernMemCopy(path, srcPath, length + 1);
    if (fileExists(path) == -1) {
        ASSERT(false);
    }
    if(argc == 0) {
        passArgv = NULL;
    }
    else {
        passArgv = NULL; //TODO: handle passing argv
    }
    executable = fileSystem->Open((char*)path);
    space = new AddrSpace();
    t = new Thread("exec new thread");
    if(space->Initialize(executable)) {
        t -> space = space;
    }
    else {
        //TODO: return error, not assert
        ASSERT(false);
    }
    delete executable;
    spid = processManager->Alloc((void*)t);
    // TODO: handle when there is no spid
    machine->WriteRegister(2, spid);
    printf("new spid: %d\n", spid);
    t->spid = spid;
    t -> Fork(StartUserProcess, (int)passArgv); 
    currentThread->Yield(); 
    PushPC();
}
