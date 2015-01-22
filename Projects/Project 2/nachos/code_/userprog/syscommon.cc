#include "syscall.h"
#include "system.h"
#include "directory.h"
#include "filesys.h"
#include "openfile.h"

int fileExists(unsigned char* file) {

    char* signName = new char[64];
    *signName = *file;	
    OpenFile* openFile;
    openFile = fileSystem->Open(signName);
    if (openFile == NULL) {
        return -1;
    } 
    return 1;
}

int fileAddress(char* file) {
    unsigned long size = machine->pageTableSize * PageSize ;
    int length = 64;
    int i, v = 0;

    if ((unsigned long)file > size) { return 0; }
    if (size - (unsigned long)file < 64) 
    {
        length = size - (unsigned long)file;
    }
    while (v && i < length)
    { 
        if (!(machine->ReadMem((unsigned long)file + i, 1, & v))) { return -1;}
        i++;
    } 
    if (v) { return -2;}
    if (i > 0) 
    {
        if (i < length ){ return i;}
	else if (i == length && !v){ return i;}
        else {return -3;}
    }
    else {return -4;}
}




void IncPC() {
    machine->WriteRegister(PrevPCReg, machine->ReadRegister(PCReg));
    machine->WriteRegister(PCReg, machine->ReadRegister(PCReg) + 4);
    machine->WriteRegister(NextPCReg, machine->ReadRegister(PCReg) + 8); 
}




int userStringLength(int s) {
    int i, v;
    i = 0;

    while (v != 0);
    {
        if ( !(machine->ReadMem(s + i, 1, & v)) ){ return -1;}
        i++;
    }     
    return (i - 1);
}

int userToKernMemCopy(unsigned char* param, int s, int end) {
    int i, v;
    for (i = 0; i < end; i++) 
    {
        if (machine->ReadMem(s + i, 1, &v)){ *(param + i) = v;}
        else {return i;}
    }
    return i;
}

int userToKernelMemRead(int s) {
    int v;
    if (!(machine->ReadMem(s, 4, &v))) {ASSERT(false);}
    return v;
}

int userToKernMatrixCopy(unsigned char** param, int s, int end) {
   int i;
   for (i = 0; i < end; i++) 
   {
        int address;
        machine->ReadMem((s + i*4),4,&address);
        param[i] = new unsigned char[userStringLength(address)];
        userToKernMemCopy(param[i],address,userStringLength(address));
   }
   return i;
}

int kernToUserMemCopy(int param, unsigned char* s, int end) {
    int i,v;
    for (i = 0; i < end; i++) 
    {
        v = *(s + i);
        if (!(machine->WriteMem(param + i, 1, v))){ return i;}
    }
    return i;
}

int rw_bufferIOCheck(int buff, int size)
{
    unsigned long task, buffSize;
    task = machine->pageTableSize * PageSize;
    buffSize = (unsigned long)buff + (unsigned long)size;
    if (buffSize > task) { return -1; }
    return 1;
}

int bufferIOCheck(int buff, int size) {
    unsigned long task, buffSize;
    task = machine->pageTableSize * PageSize;
    buffSize = (unsigned long)buff + (unsigned long)size;

    if (buffSize > task) {return -1; }
    else if (size < 0) { return -2; }
    else { return 1; }
}
