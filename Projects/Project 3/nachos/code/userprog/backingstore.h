#ifndef BACKINGSTORE_H
#define BACKINGSTORE_H

#include "filesys.h"
#include "addrspace.h"

class BackingStore{
public:
    /* Create a backing store file for an AddrSpace */
    BackingStore(AddrSpace *as);
    
    /* Write the virtual page referenced by pte to the backing store */
	/* Example invocation: PageOut(&machine->pageTable[virtualPage]) or */
	/*                     PageOut(&space->pageTable[virtualPage]) */
	void PageOut(TranslationEntry *pte);

	/* Read the virtual page referenced by pte from the backing store */
	void PageIn(TranslationEntry *pte);

	AddrSpace* addspace;

private:
    int numOfPages;
    FileSystem* backFile;
};

#endif
