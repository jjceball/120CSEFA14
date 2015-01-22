// Will include the implementations of our system calls

#include "kernelsyscall.h"
#include "syscall.h"
#include "system.h"
#include "thread.h"
#include "syscommon.h"
#include "addrspace.h"
#include "processmanager.h"
#include "directory.h"
#include "filesys.h"
#include "openfile.h"
#include "machine.h"

/** Child Threads forking function where machine is
*   initialized and ran.
*/
void ProcessStart(int argv)
{ 
    currentThread->space->InitRegisters();		// set the initial register values
    currentThread->space->RestoreState();		// load page table register
    machine->Run();			// jump to the user progam
    ASSERT(FALSE);			// machine->Run never returns;
    // the address space exits
    // by doing the syscall "exit"
}

/* Read "size" bytes from the open file into "buffer".
 * Return the number of bytes actually read -- if the open file isn't
 * long enough, or if it is an I/O device, and there aren't enough
 * characters to read, return whatever is available (for I/O devices,
 * you should always wait until you can return at least one character).
 */
int kernelRead(char *buffer, int size, OpenFileId id) 
{
    int i;
    unsigned char kernel_char;
    if (id == ConsoleInput) {
        for (i = 0; i < size; i++) {
            kernel_char = synchconsole->GetChar();
            kernToUserMemCopy((int)(buffer + i), &kernel_char, 1);
        }
    } else {
        ASSERT(false);
    }
    return size;
}

/* Write "size" bytes from "buffer" to the open file. */

// Buffer is the pointer in User-level memory
int kernelWrite(char *buffer, int size, OpenFileId id) {
    int i;
    int copied_size;
    unsigned char kernel_char;
    if (id == ConsoleOutput) {
        for (i = 0; i < size; i++) {
            copied_size = userToKernMemCopy(&kernel_char, (int)(buffer + i), 1);
            if (copied_size != 1) {
                ASSERT(false);
            }
            synchconsole->PutChar(kernel_char);
        }
    } else {
        ASSERT(false);
    }
    return size;
}

void start_child_thread(int func_ptr) 
{
    currentThread->space->InitRegisters();
    currentThread->space->RestoreState();       // load page table register

    machine->WriteRegister(PCReg, func_ptr - 4);
    machine->WriteRegister(NextPCReg, func_ptr);

    //If the child thread return, jump back to Exit System Call
    // machine->WriteRegister(RetAddrReg, 0x10 + 4 * 3);
    // jump back to the main exit jal main. in start.cc
    machine->WriteRegister(RetAddrReg, 4 * 2);
    machine->Run();         // jump to the user progam
    ASSERT(false);
}

int kernelFork(int function_ptr)
{
    Thread* t;
    AddrSpace* new_space;

    new_space = new AddrSpace(); //a new space
    t = new Thread("child thread");

    if (new_space->CopyCurrentSpace()) {
        return -1;
    }

    //DEBUG for space
    t->space = new_space;

    //t->SaveUserState();
    t->spid = currentThread->spid;

    t->Fork(start_child_thread, function_ptr);

    currentThread->Yield();
    return 0;
}

/* This user program is done (status = 0 means exited normally). */
void kernelExit(int status);

/* Run the executable, stored in the Nachos file "name", and return the
 * address space identifier
 */
void kernelExec()
{
    int srcPath, len, argc, argv, opt;
    unsigned char ** passArgv;
    unsigned char * path;
    Thread * t;
    OpenFile * executable;
    AddrSpace * space;
    int spid;
    //printf("Thread name that call EXEC: %s\n", currentThread->getName()); 
    srcPath = machine->ReadRegister(4);
    argc = machine->ReadRegister(5);
    argv = machine->ReadRegister(6);
    opt = machine->ReadRegister(7);
    // first check the location of filename is valid
    if(!fileAddress((char*)srcPath)){
        ASSERT(false);
    }
    len = userStringLength((int)srcPath);
    path = new unsigned char[len+1];
    userToKernMemCopy(path, srcPath, len + 1);
    if (fileExists(path) == -1) {
        ASSERT(false);
    }
    //printf("user str : %s, len: %d \n", path, len);
    if(argc == 0) {
        passArgv = NULL;
    }
    else {
        passArgv = new unsigned char*[argc + 1]; // store argc first
        passArgv[0] = (unsigned char*)argc;
        userToKernMatrixCopy(passArgv+4, argv, argc);
    }
    executable = fileSystem->Open((char*)path);
    space = new AddrSpace();
    t = new Thread("exec new thread", opt);
    if(space->Initialize(executable)) {
        t -> space = space;
    }
    else {
        machine->WriteRegister(2, 0); // return err code 0
        delete t;
        IncPC();
        return;
        ASSERT(false); // should not reach here
    }
    //delete executable;         // project 3.1.(Keep in mind)
    delete path;
    spid = processManager->Alloc((void*)t);
    machine->WriteRegister(2, spid);
    if(spid == 0) {// not enough spid in process manager
        machine->WriteRegister(2, 0); // return err code 0
        delete t;
        IncPC();
        return;
    }
    //printf("new spid: %d\n", spid);
    t->spid = spid;
    t->Fork(ProcessStart, (int)passArgv); 
    currentThread->Yield(); 
    IncPC();
}

void kernelJoin()
{
    int joineeId;
    int exitStatus;
    joineeId = machine->ReadRegister(4);
    Thread * joineeThread;
    joineeThread = (Thread*)(processManager->Get(joineeId));
    joineeThread -> Join();
    exitStatus = joineeThread->exitstatus;
    machine->WriteRegister(2, exitStatus); // return exit code 
    IncPC();
}
