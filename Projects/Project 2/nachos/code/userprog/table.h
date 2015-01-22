// table.h
//
// Class that manages the memory so that the kernel can conveniently keep track of 
// which physical page frames are free and which have been allocated.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#ifndef TABLE_H
#define TABLE_H

#include "bitmap.h"
#include "synch.h"

class Thread;
class Lock;

class Table {
public:
/* Create a table to hold at most "size" entries. */
    Table(int size);

/*Destructor*/
    ~Table();

/* Allocate a table slot for "object", returning the "index" of the
   allocated entry; otherwise, return -1 if no free slots are available. */
    int Alloc(void *object);

/* Retrieve the object from table slot at "index", or NULL if that
   slot has not been allocated. */
    void *Get(int index);

/* Free the table slot at index. */
    void Release(int index);
private:
    Typedef T* Tptr;
    Tptr *entryTable;
    Lock* tLock;
};

#endif // TABLE_H

