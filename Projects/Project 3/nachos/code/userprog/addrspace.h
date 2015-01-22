// addrspace.h
//	Data structures to keep track of executing user programs
//	(address spaces).
//
//	For now, we don't keep any information about address spaces.
//	The user level CPU state is saved and restored in the thread
//	executing the user program (see thread.h).
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#ifndef ADDRSPACE_H
#define ADDRSPACE_H

#include "copyright.h"
#include "filesys.h"
#include "syscommon.h"
#include "machine.h"
#include "noff.h"
#include "stats.h"

#define UserStackSize		1024 	// increase this as necessary!

class AddrSpace {
public:
    int Initialize(OpenFile *executable); // Initliazes AddrSpace

    int InitOnFault(int faultedPN); // now initializing on a page fault
	// pagetables initialized to fault
	
    // Project 3 Part 2

    void demandPaging();
    void evictPage();
    void markPage();

    ~AddrSpace();          // De-allocate an address space

    void InitRegisters();		// Initialize user-level CPU registers,
    // before jumping to user code

    void SaveState();			// Save/restore address space-specific
    void RestoreState();		// info on a context switch

    unsigned int getNumPages() {return numPages;}
    TranslationEntry* getPageTable() {return pageTable;}

private:
    TranslationEntry *pageTable;	// Assume linear page table translation
    // for now!
    unsigned int numPages;		// Number of pages in the virtual
    // address space
};

#endif // ADDRSPACE_H
