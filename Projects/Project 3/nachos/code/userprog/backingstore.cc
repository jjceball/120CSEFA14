#include "backingstore.h"
#include "system.h"

//Constructor
BackingStore::BackingStore(AddrSpace *as) 
{
    addspace = as;
    backFile = new FileSystem(true);
    numOfPages = addspace->getNumPages();
}

void
BackingStore::PageOut(TranslationEntry *pte)
{
	// Need implementation
}


void
BackingStore::PageIn(TranslationEntry *pte)
{
 	// Need implementation 
}