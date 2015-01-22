// addrspace.cc
//	Routines to manage address spaces (executing user programs).
//
//	In order to run a user program, you must:
//
//	1. link with the -N -T 0 option
//	2. run coff2noff to convert the object file to Nachos format
//		(Nachos object code format is essentially just a simpler
//		version of the UNIX executable object code format)
//	3. load the NOFF file into the Nachos file system
//		(if you haven't implemented the file system yet, you
//		don't need to do this last step)
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "addrspace.h"
#include "machine.h"
#include "copyright.h"
#include "system.h"
#include "memorymanager.h"
#ifdef HOST_SPARC
#include <strings.h>
#endif

//----------------------------------------------------------------------
// SwapHeader
// 	Do little endian to big endian conversion on the bytes in the
//	object file header, in case the file was generated on a little
//	endian machine, and we're now running on a big endian machine.
//----------------------------------------------------------------------

static void
SwapHeader (NoffHeader *noffH)
{
    noffH->noffMagic = WordToHost(noffH->noffMagic);
    noffH->code.size = WordToHost(noffH->code.size);
    noffH->code.virtualAddr = WordToHost(noffH->code.virtualAddr);
    noffH->code.inFileAddr = WordToHost(noffH->code.inFileAddr);
    noffH->initData.size = WordToHost(noffH->initData.size);
    noffH->initData.virtualAddr = WordToHost(noffH->initData.virtualAddr);
    noffH->initData.inFileAddr = WordToHost(noffH->initData.inFileAddr);
    noffH->uninitData.size = WordToHost(noffH->uninitData.size);
    noffH->uninitData.virtualAddr = WordToHost(noffH->uninitData.virtualAddr);
    noffH->uninitData.inFileAddr = WordToHost(noffH->uninitData.inFileAddr);
}

//----------------------------------------------------------------------
// AddrSpace::Initialize
// 	Create an address space to run a user program.
//	Load the program from a file "executable", and set everything
//	up so that we can start executing user instructions.
//
//	Assumes that the object code file is in NOFF format.
//
//	First, set up the translation from program memory to physical
//	memory.  For now, this is really simple (1:1), since we are
//	only uniprogramming, and we have a single unsegmented page table
//
//	"executable" is the file containing the object code to load into memory
//----------------------------------------------------------------------
int
AddrSpace::Initialize(OpenFile *executable)
{
    NoffHeader noffH;
    unsigned int i, size;

    executable->ReadAt((char *)&noffH, sizeof(noffH), 0);
    if ((noffH.noffMagic != NOFFMAGIC) &&
            (WordToHost(noffH.noffMagic) == NOFFMAGIC))
        SwapHeader(&noffH);
    ASSERT(noffH.noffMagic == NOFFMAGIC);

// how big is address space?
    size = noffH.code.size + noffH.initData.size + noffH.uninitData.size
           + UserStackSize;	// we need to increase the size
    // to leave room for the stack
    numPages = divRoundUp(size, PageSize);
    size = numPages * PageSize;

    //ASSERT(numPages <= NumPhysPages);		// check we're not trying
    // to run anything too big --
    // at least until we have
    // virtual memory
    pageTable = new TranslationEntry[numPages];

    DEBUG('a', "Initializing address space, num pages %d, size %d\n",
          numPages, size);
// first, set up the translation
    //pageTable = new TranslationEntry[numPages];
    for (i = 0; i < numPages; i++) {
        pageTable[i].virtualPage = i;	// for now, virtual page # = phys page #
	// Virtual page number should now be i 
	// We aren't using real virtual memory, so we assign a physical memory page
      //  pageTable[i].physicalPage =  memoryManager->AllocPage();
		
		
	// Zeros out physical page frame; setting aside now
	/////bzero(&machine->mainMemory[pageTable[i].physicalPage * PageSize], PageSize);
	
	
        pageTable[i].valid = FALSE;	// SET TO INVALID; PROJECT 3.1
        pageTable[i].use = FALSE;
        pageTable[i].dirty = FALSE;
        pageTable[i].readOnly = FALSE;  // if the code segment was entirely on
        // a separate page, we could set its
        // pages to be read-only
    }

// zero out the entire address space, to zero the uninitialized data segment
// and the stack segment
 
   // No longer needed 
   //
   //bzero(machine->mainMemory, size);

    return 1;
}

/**
**********************************************************************
AddrSpace::InitOnFault    Project 3.1.4
**********************************************************************
*/
int AddrSpace::InitOnFault(int faultedPN) {

    NoffHeader noffH;
    // loads the code and data from the executable; On demand config after page fault
    unsigned int memToSize = noffH.code.size;
    unsigned int memToStart = noffH.code.virtualAddr;
    unsigned int fileBegin = noffH.code.inFileAddr;
    unsigned int memory, var;

    memory = memToStart & (PageSize - 1);

    while (memToSize >= PageSize - memory)
    {
	    // we are now looking at the page number of the faulted page
        var = faultedPN; //pageTable[memToStart / PageSize].physicalPage;

	// left "open" no longer need
	//executable->ReadAt(&(machine->mainMemory[var*PageSize + memory]), PageSize - memory, fileBegin);
	memToSize -= (PageSize - memory);
	memToStart += (PageSize - memory);
	fileBegin += (PageSize - memory);
	memory = memToStart & (PageSize - 1);
    }
    // done adjusting variables

    if (memToSize > 0)
    {
	    var = faultedPN; //pageTable[memToStart / PageSize].physicalPage;

	//executable->ReadAt(&(machine->mainMemory[var*PageSize + memory]), memToSize, fileBegin);
    }

    // reset the vars
    memToSize = noffH.initData.size;
    memToStart = noffH.initData.virtualAddr;
    fileBegin = noffH.initData.inFileAddr;
    memory = memToStart & (PageSize - 1);

    while (memToSize >= PageSize - memory)
    {
        var = faultedPN; //pageTable[memToStart / PageSize].physicalPage;
      
	//executable->ReadAt(&(machine->mainMemory[var*PageSize + memory]),PageSize - memory, fileBegin);

	    memToSize -= (PageSize - memory);
	    memToStart += (PageSize - memory);
	    fileBegin += (PageSize - memory);
	    memory = memToStart & (PageSize - 1);
    }

    if (memToSize > 0)
    {
	    var = faultedPN; //pageTable[memToStart / PageSize].physicalPage;

	    //executable->ReadAt(&(machine->mainMemory[var*PageSize + memory]),memToSize, fileBegin);
    }
	
    return 1;
}

//----------------------------------------------------------------------
// AddrSpace::demandPaging
//  demand paging for Project 3
//----------------------------------------------------------------------

void AddrSpace::demandPaging()
{
    // ?
}

//----------------------------------------------------------------------
// AddrSpace::evictPage
//  evict the page for Project 3
//----------------------------------------------------------------------

void AddrSpace::evictPage()
{
    // ?
}

//----------------------------------------------------------------------
// AddrSpace::markPage
//  demand paging for Project 3
//----------------------------------------------------------------------

void AddrSpace::markPage()
{
    // ?
}

//----------------------------------------------------------------------
// AddrSpace::~AddrSpace
// 	Dealloate an address space.  Nothing for now!
//----------------------------------------------------------------------

AddrSpace::~AddrSpace()
{
    unsigned int i;

    for (i = 0; i < numPages; i++)
    {
        memoryManager->FreePage(pageTable[i].physicalPage);
    }
 
    delete [] pageTable;
}

//----------------------------------------------------------------------
// AddrSpace::InitRegisters
// 	Set the initial values for the user-level register set.
//
// 	We write these directly into the "machine" registers, so
//	that we can immediately jump to user code.  Note that these
//	will be saved/restored into the currentThread->userRegisters
//	when this thread is context switched out.
//----------------------------------------------------------------------

void
AddrSpace::InitRegisters()
{
    int i;

    for (i = 0; i < NumTotalRegs; i++)
        machine->WriteRegister(i, 0);

    // Initial program counter -- must be location of "Start"
    machine->WriteRegister(PCReg, 0);

    // Need to also tell MIPS where next instruction is, because
    // of branch delay possibility
    machine->WriteRegister(NextPCReg, 4);

    // Set the stack register to the end of the address space, where we
    // allocated the stack; but subtract off a bit, to make sure we don't
    // accidentally reference off the end!
    machine->WriteRegister(StackReg, numPages * PageSize - 16);
    DEBUG('a', "Initializing stack register to %d\n", numPages * PageSize - 16);
}

//----------------------------------------------------------------------
// AddrSpace::SaveState
// 	On a context switch, save any machine state, specific
//	to this address space, that needs saving.
//
//	For now, nothing!
//----------------------------------------------------------------------

void AddrSpace::SaveState()
{
    pageTable = machine->pageTable;
    numPages = machine->pageTableSize; 
}

//----------------------------------------------------------------------
// AddrSpace::RestoreState
// 	On a context switch, restore the machine state so that
//	this address space can run.
//
//      For now, tell the machine where to find the page table.
//----------------------------------------------------------------------

void AddrSpace::RestoreState()
{
    machine->pageTable = pageTable;
    machine->pageTableSize = numPages;
}
