#include "system.h"
#include "synch.h"

#ifndef BUFFPIPE_H
#define BUFFPIPE_H

class BuffPipe 
{
public:
	BuffPipe();
	~BuffPipe();

	char pop();
	int push(char);

private:
	List* queue;
	Semaphore* semaphore;
	Lock* lock;
};

#endif