#ifndef SYSCOMMON_H
#define SYSCOMMON_H

//check if the files exists, returns different negative values based on which failure happens
int fileExists(unsigned char* file);

//checks if the file exists in the address space, returns a negative value on failure
int fileAddress(char* file);

//adds 4 to everything to move PC forward
void IncPC();

//increments a value to find the length of a string
int userStringLength(int src);

//copies one char at a time into param
int userToKernMemCopy(unsigned char* param, int s, int end);

// reads then returns value
int userToKernMemRead(int src);

//copies one char at a time into param (multi level)
int userToKernMatrixCopy(unsigned char** param, int src, int end);

//copies one char at a time into param
int kernToUserMemCopy(int param, unsigned char* src, int end);

//checks to see if the buffer can be used in the size limitations
int bufferIOCheck(int buff, int size);

// Buffer check for Read/Write
int rw_bufferIOCheck(int buff, int size);

// Read stack
int readArg(int index);

#endif
