// memorymanager.h
//
// Class that manages the memory so that the kernel can conveniently keep track of 
// which physical page frames are free and which have been allocated.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

#include "bitmap.h"
#include "synch.h"

class Thread;
class Lock;

class MemoryManager {
public:
	MemoryManager(int numPages);
	~MemoryManager();
	int AllocPage(bool check);
	void FreePage(int physPageNum);
	bool PageIsAllocated(int physPageNum);
	int GetFreePageCount();

private:
	BitMap* memMap;
	Lock* mLock;
	int freedPages;
};

#endif // MEMORYMANAGER_H
