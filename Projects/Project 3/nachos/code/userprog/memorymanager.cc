// memorymanager.c
//	Routines to manage the memory to keep track of which physical 
//	page frames are free and which have been allocated.  
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "memorymanager.h"
#include "system.h"
//#include "copyright.h"
#include "bitmap.h"

//----------------------------------------------------------------------
// MemoryManager::MemoryManager
// 	
//----------------------------------------------------------------------

MemoryManager::MemoryManager(int numPages)
{
   freedPages = numPages; 

   // init bitmap to num of phys pages
   this->memMap = new BitMap(numPages);

   mLock = new Lock("Memory Lock");
}

//----------------------------------------------------------------------
// MemoryManager::~MemoryManager
//
//----------------------------------------------------------------------

MemoryManager::~MemoryManager()

{
    delete memMap;
    delete mLock; 	
}

//----------------------------------------------------------------------
// MemoryManager::AllocPage
//
//----------------------------------------------------------------------

int
MemoryManager::AllocPage()
{
    int avail;

    mLock->Acquire();
    avail = memMap->Find();
    
    if (avail > 0)
    {
      memMap->Mark(avail);
    }

    mLock->Release();

   
    
    return avail;
}

//----------------------------------------------------------------------
// MemoryManager::FreePage
//
//----------------------------------------------------------------------

void
MemoryManager::FreePage(int physPageNum)
{
    mLock->Acquire();
    memMap->Clear(physPageNum);
    mLock->Release();
}


//----------------------------------------------------------------------
// MemoryManager::GetFreePageCount
//
//----------------------------------------------------------------------

int
MemoryManager::GetFreePageCount()
{
    int freepage;
    mLock->Acquire();

    freepage = memMap->NumClear();

    mLock->Release();
    return freepage;

}


//----------------------------------------------------------------------
// MemoryManager::PageIsAllocated
//
//----------------------------------------------------------------------

bool
MemoryManager::PageIsAllocated(int physPageNum)
{
    mLock->Acquire();
    int alloc = memMap->Test(physPageNum);
    mLock->Release();
    return alloc;
}
