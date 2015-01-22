// table.c
//	Routines to manage the memory to keep track of which physical 
//	page frames are free and which have been allocated.  
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "bitmap.h"

//----------------------------------------------------------------------
// Table::Table
// 	
//----------------------------------------------------------------------

Table::Table(int size)
{
    tLock = new Lock();
    entryTable = new Tptr[size];
}

//----------------------------------------------------------------------
// Table::~Table
//
//----------------------------------------------------------------------

Table::~Table()
{
    delete entryTable;
    delete tLock;
    	
}

//----------------------------------------------------------------------
// Table::Alloc
//
//----------------------------------------------------------------------

int
Table::Alloc()
{
    tLock->Acquire();
    for(int i=0; i < size; ++i)
    {
        if(*(entryTable)==NULL)
        {
          tLock->Release();
          return i;
        }
    }
    tLock->Release();
    return -1;
}

//----------------------------------------------------------------------
// Table::*Get
//
//----------------------------------------------------------------------

void
Table::*Get(int index)
{
    tLock->Acquire();
    void *object;
    object = *(entryTable[index]);
    tLock->Release();
}

//----------------------------------------------------------------------
// Table::Release
//
//----------------------------------------------------------------------

bool
Table::Release(int index)
{
    tLock->Acquire();
    entryTable[index] = NULL;
    tLock->Release();
}

