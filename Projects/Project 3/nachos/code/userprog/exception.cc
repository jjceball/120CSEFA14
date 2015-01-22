// exception.cc
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"
#include "processmanager.h"
#include "backingstore.h"
#include "thread.h"
#include "bitmap.h"
#include "memorymanager.h"
#include "addrspace.h"
#include "filesys.h"
#include "synchconsole.h"

//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2.
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions
//	are in machine.h.
//----------------------------------------------------------------------
extern SynchConsole *synchconsole;
extern ProcessManager *processManager;

void fixProgramCounter()
{
    int PC;
    PC = machine->ReadRegister(PCReg);
    machine->WriteRegister(PrevPCReg, PC);
    PC = machine->ReadRegister(NextPCReg);
    machine->WriteRegister(PCReg, PC);
    PC += 4;
    machine->WriteRegister(NextPCReg,PC);
}

void Exit()
{
    AddrSpace *addspace;
    int spaceid;
    addspace = currentThread->space;
    int exitVal = machine->ReadRegister(4);
    printf("Exit value: %d\n", exitVal); 
    addspace->~AddrSpace();
    spaceid = currentThread->getPID();
    processManager->Release(spaceid);
    currentThread->Finish();
    fixProgramCounter();
}

void
ExceptionHandler(ExceptionType which)
{
    int type = machine->ReadRegister(2);

    if (which == SyscallException) {
        switch (type) {
            case SC_Halt:
                DEBUG('a', "Shutdown, initiated by user program.\n");
                printf("halt\n");
                interrupt->Halt();
                break;

            case SC_Exit:
                // Deleted implementation from project 2
                break;

            case SC_Exec:
                // Deleted implementation from project 2
                break;

            case SC_Join:
                // Deleted implementation from project 2
                break;

            case SC_Fork:
                // Deleted implementation from project 2
                break;

            case SC_Yield:
                // Deleted implementation from project 2
                break;
            
            case SC_Read:
                // Deleted implementation from project 2
                break;

            case SC_Write:
                // Deleted implementation from project 2
                break;

            default:
                printf("Unexpected user mode exception %d %d\n", which, type);
                //interrupt->Halt();
                ASSERT(FALSE);
        }
    }
    else if (which == NumExceptionTypes) {
        // Print error message then exit
        printf("Number exception\n");
        Exit();
    }
    else if (which == IllegalInstrException) {
        // Print error message then exit        
        printf("Illegal instruction exception\n");
        Exit();
    } 
    else if (which == OverflowException) {
        // Print error message then exit        
        printf("Overflow %d %d\n", which, type);
        Exit();
    }
    else if (which == AddressErrorException) {
        // Print error message then exit
        printf("Address error %d %d\n", which, type);
        Exit();
	}
    else if (which == BusErrorException) {
        // Print error message then exit
        printf("Bus Error %d %d\n", which, type);
        Exit();
    }
    else if (which == ReadOnlyException)
    {
        // Print error message then exit
        printf("Read Only error %d %d\n", which, type);
        Exit();
    }

    /**
    **********************************************************************
    AddrSpace::InitOnFault    Project 3.1.4
    **********************************************************************
    */
    else if (which == PageFaultException) {
        int faultedPN;
		// We have a page fault do to invalid bit
        printf("Page fault has occurred. Loading executable into table.\n");
		// doesnt kill process; needs to begin loading executable into pagetable
		// Acquired page number of the page that 
        faultedPN = machine->ReadRegister(BadVAddrReg);
        faultedPN = divRoundDown(faultedPN,PageSize);
        currentThread->space->InitOnFault(faultedPN); // Call Page Fault Initialization 
		//sets all the variables to allow for execution
		machine->pageTable[faultedPN].valid = TRUE;
		machine->pageTable[faultedPN].use = FALSE;
		machine->pageTable[faultedPN].dirty = FALSE;
		machine->pageTable[faultedPN].readOnly = FALSE; 
    } else {
        printf("Unexpected user mode exception %d %d\n", which, type);
    }
}
