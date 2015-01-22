// synch.cc
//	Routines for synchronizing threads.  Three kinds of
//	synchronization routines are defined here: semaphores, locks
//   	and condition variables (the implementation of the last two
//	are left to the reader).
//
// Any implementation of a synchronization routine needs some
// primitive atomic operation.  We assume Nachos is running on
// a uniprocessor, and thus atomicity can be provided by
// turning off interrupts.  While interrupts are disabled, no
// context switch can occur, and thus the current thread is guaranteed
// to hold the CPU throughout, until interrupts are reenabled.
//
// Because some of these routines might be called with interrupts
// already disabled (Semaphore::V for one), instead of turning
// on interrupts at the end of the atomic operation, we always simply
// re-set the interrupt state back to its original value (whether
// that be disabled or enabled).
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "synch.h"
#include "system.h"

//----------------------------------------------------------------------
// Semaphore::Semaphore
// 	Initialize a semaphore, so that it can be used for synchronization.
//
//	"debugName" is an arbitrary name, useful for debugging.
//	"initialValue" is the initial value of the semaphore.
//----------------------------------------------------------------------

Semaphore::Semaphore(char* debugName, int initialValue)
{
    name = debugName;
    value = initialValue;
    queue = new List;
}

//----------------------------------------------------------------------
// Semaphore::Semaphore
// 	De-allocate semaphore, when no longer needed.  Assume no one
//	is still waiting on the semaphore!
//----------------------------------------------------------------------

Semaphore::~Semaphore()
{
    delete queue;
}

//----------------------------------------------------------------------
// Semaphore::P
// 	Wait until semaphore value > 0, then decrement.  Checking the
//	value and decrementing must be done atomically, so we
//	need to disable interrupts before checking the value.
//
//	Note that Thread::Sleep assumes that interrupts are disabled
//	when it is called.
//----------------------------------------------------------------------

void
Semaphore::P()
{
    IntStatus oldLevel = interrupt->SetLevel(IntOff);	// disable interrupts

    while (value == 0) { 			// semaphore not available
        queue->Append((void *)currentThread);	// so go to sleep
        currentThread->Sleep();
    }
    value--; 					// semaphore available,
    // consume its value

    (void) interrupt->SetLevel(oldLevel);	// re-enable interrupts
}

//----------------------------------------------------------------------
// Semaphore::V
// 	Increment semaphore value, waking up a waiter if necessary.
//	As with P(), this operation must be atomic, so we need to disable
//	interrupts.  Scheduler::ReadyToRun() assumes that threads
//	are disabled when it is called.
//----------------------------------------------------------------------

void
Semaphore::V()
{
    Thread *thread;
    IntStatus oldLevel = interrupt->SetLevel(IntOff);

    thread = (Thread *)queue->Remove();
    if (thread != NULL)	   // make thread ready, consuming the V immediately
        scheduler->ReadyToRun(thread);
    value++;
    (void) interrupt->SetLevel(oldLevel);
}

// Dummy functions -- so we can compile our later assignments
// Note -- without a correct implementation of Condition::Wait(),
// the test case in the network assignment won't work!

//In: Lock takes in a char pointer for the debug name for testing purposes
// Out:sets variables required for thread lock, intialized
//
Lock::Lock(char* debugName) {
    name = debugName;
    queue = new List();
    isLocked = false;
    lockThread = NULL;
}

//
// This section destroys the lock where it sets up interrupt, and deletes the 
// whole lock, meaning all the variables must be destroyed
//
//

Lock::~Lock() {
   IntStatus oldLevel = interrupt->SetLevel(IntOff);	// disable interrupts
   ASSERT(isLocked == false);
  
   delete name;
   delete queue;

   (void) interrupt->SetLevel(oldLevel);	// re-enable interrupts

}

//
// Acquire a lock must check how the status of the locks are, and will comfrim
// it has the ability to take a lock. When it has found the status it takes lock
//

void Lock::Acquire() {
    IntStatus oldLevel = interrupt->SetLevel(IntOff);	// disable interrupts

    if (isLocked) {
      ASSERT(lockThread != currentThread);
      queue->Append(currentThread);    //put this thread (the current thread) on the wait Q for the lock;
      currentThread->Sleep();                  //block this thread (make the current thread go to sleep);
    }

    ASSERT(isLocked == false);
    isLocked = true;                          // do some bookkeeping 
    lockThread = currentThread;

    (void) interrupt->SetLevel(oldLevel);	// re-enable interrupts
}

//
// lockEqualCurent is a function that returns a boolean determining whether 
// the lock thread is the current thread and whether the thread is already
// locked 
//

bool Lock::lockEqualCurrent(){
   bool equal = (lockThread == currentThread);
   bool held = (isLocked == true); 

   return (equal && held);
}

//
// Release() is a function that disables interrupts, asserts that this thread
// should be released, then creates a new thred which removes the blocked 
// thread from the queue, and if the newthread exists, it unblocks the 
// removed thread, then finally re-enable interrupts 
//

void Lock::Release() {
    IntStatus oldLevel = interrupt->SetLevel(IntOff);	// disable interrupts

    ASSERT(isLocked);
    ASSERT(lockThread);
    ASSERT(lockThread == currentThread);

    lockThread = NULL;
    isLocked = false; 
    Thread *newThread = (Thread *)queue->Remove();  // remove a blocked thread from the Q, 

    if(newThread != NULL)
    {
      scheduler->ReadyToRun(newThread);       // if there is one, unblock the removed thread;
    }                                            

    (void) interrupt->SetLevel(oldLevel);	// re-enable interrupts
}

//
// The initialization of the condition class with a debugName and 
// the creation of a queue.  
//

Condition::Condition(char* debugName) {
    name = debugName;
    queue = new List;
}

//
// Function to delete a condition, including its name and if the 
// queue isEmpty(), then it deletes the queue.  
//

Condition::~Condition() { 
    delete name;
    ASSERT(queue->IsEmpty()); 
    delete queue;
}

//
// This function is meant to take in a lock as a parameter, then assert 
// that the lock is the current lock. It then disables interrupts, and 
// releases the lock. It then puts the current thread on a wait queue
// and then blocks the current thread. The lock is then acquired and
// interrupts are re-enabled. 
//

void Condition::Wait(Lock* conditionLock) {
    ASSERT(conditionLock->lockEqualCurrent());
    IntStatus oldLevel = interrupt->SetLevel(IntOff);	// disable interrupts

    conditionLock->Release();   // release Lock
    
    queue->Append((void *)currentThread);    //put this thread (the current thread) on the wait Q for the lock;
    currentThread->Sleep();                  //block this thread (make the current thread go to sleep);

    conditionLock->Acquire();  // acquires Lock

    (void) interrupt->SetLevel(oldLevel);	// re-enable interrupts
}

//
// Function within the Condition class which creates a new thread, asserts that
// the lock is the current lock, it then disables interrupts, and if the queue
// isEmpty() it removes the blocked thread from the queue and unblocks the removed
// thread. Finally it reenables interrupts. Meant to signal the lock is ready to
// be removed. 
//

void Condition::Signal(Lock* conditionLock) { 
    Thread *newThread;  // Create new Thread 
    
    ASSERT(conditionLock->lockEqualCurrent());
    IntStatus oldLevel = interrupt->SetLevel(IntOff);	// disable interrupts
    if (queue->IsEmpty()){
      newThread = (Thread *)queue->Remove();  // remove a blocked thread from the Q 
      scheduler->ReadyToRun(newThread);    // if there is one, unblock the removed thread; 
    }                                           

    (void) interrupt->SetLevel(oldLevel);	// re-enable interrupts
}

//
// Function that is similar to the Signal method above, with the main difference
// being that while the queue is empty, it removes the blocked thread from the
// queue and as long as there is a newThread it get is ready to run  
//

void Condition::Broadcast(Lock* conditionLock) { 
    Thread *newThread;  // Create new Thread 
     
    ASSERT(conditionLock->lockEqualCurrent());
    IntStatus oldLevel = interrupt->SetLevel(IntOff);	// disable interrupts

    while(queue->IsEmpty())
    {
      newThread = (Thread *)queue->Remove();  // remove a blocked thread from the Q 
      if(newThread != NULL)
      {
        scheduler->ReadyToRun(newThread);      // if there is one, unblock the removed thread;
      }
      else
      {
        break;
      }                                           
    }

    (void) interrupt->SetLevel(oldLevel);	// re-enable interrupts
}

//
// Mailbox takes in a lot of variables to ajust the settings of the mailbox
// including what is locked, and buffered 
//

Mailbox::Mailbox(char* debugName, Condition* condition, Condition* condbuf, Lock* lock, Lock* lockbuf) {
  name = debugName;
  mailcond = condition;
  bufcond = condbuf;
  maillock = lock;
  buflock = lockbuf;
  inc = 0;
}

//
// Deletes all the mailboxes and remove all the variables
//

Mailbox::~Mailbox() {
  delete name;
  delete buffer;
  delete mailcond;
  delete maillock;
}

//
// This area sends a message of an integer and put it into the stack for use
//

void Mailbox::Send(int message) {
  maillock->Acquire(); //must take the lock because i/o only happens one way

  inc++;
 
  if(inc > 0){  //if there is a message, so wait
    mailcond->Wait(maillock);
  }
  else { // there is no message so this lock is ineligible  
    mailcond->Signal(maillock);
    maillock->Release();
  }
  buffer =  &message;
  buflock->Acquire();
  bufcond->Signal(buflock);
  buflock->Release();
}

//
// Mailbox gets a pointer to the position of the message in the stack to be use
//

void Mailbox::Receive(int * message) {
  maillock->Acquire();  //buffer lock again

  inc--; //takes the number of messages down
 
  if(inc < 0){
    mailcond->Wait(maillock);
  }
  else {  //this should never be used
    mailcond->Signal(maillock);
    maillock->Release();
  }
  buflock->Acquire();
  bufcond->Wait(buflock);
  ASSERT(buffer!=NULL);  //makes sure not pulling from an empty stack
  *message = *buffer;
}

//
// Whale class only requires a third item to send and wait for comapred to mailbox
//

// The Whale class is similar to the Mailbox problem 

Whale::Whale(char* debugName) {
  name = debugName;
  match = new Condition("match");
  lock = new Lock("lock");

  male = 0;
  female = 0;
  matchmaker = 0;
//flags and variables
  maleready = 0;
  femaleready = 0;
  matchmakerready = 0; 
}

//
// Deletes all items involved with the whale

Whale::~Whale() {
  delete match;
  delete lock;
  delete male;
  delete female;
}

//
//  checks all the flags to see if there is ready for a match to be made
//

bool Whale::matchable(){
  ASSERT(lock->lockEqualCurrent());
  if((*male) > 0 && (*female) > 0 && (*matchmaker) > 0) {
    return true;
  }
  else {
    return false;
  }
}

//
// the male whale must be ready, there may be multiple (threads)
// each whale requires a lock, and then waits

void Whale::Male() {
  lock->Acquire();
  
  (*male)++;
//matchable is broadcast to all threads when there are enough waiting
  if(matchable()){
    (*male)--;
    (*female)--;
    (*matchmaker)--;
    (*maleready)++;
    (*femaleready)++;
    (*matchmakerready)++;
    match->Broadcast(lock); //lets the flag show that it is ready
  }
//waiting with male being ready
  while(!((*maleready) > 0)) {
    match->Wait(lock);
  }

  (*maleready)--;
 
  lock->Release();
}

//
// see above same as male

void Whale::Female() {
  lock->Acquire();
  
  (*female)++;
  if(matchable()){
    (*male)--;
    (*female)--;
    (*matchmaker)--;
    (*maleready)++;
    (*femaleready)++;
    (*matchmakerready)++;
    match->Broadcast(lock);
  }

  while(!((*femaleready) > 0)) {
    match->Wait(lock);
  }

  (*femaleready)--;
 
  lock->Release();
}

//
// see above same as male
//

void Whale::Matchmaker() {
  lock->Acquire();
  
  (*matchmaker)++;
  if(matchable()){
    (*male)--;
    (*female)--;
    (*matchmaker)--;
    (*maleready)++;
    (*femaleready)++;
    (*matchmakerready)++;
    match->Broadcast(lock);
  }

  while(!((*matchmakerready) > 0)) {
    match->Wait(lock);
  }

  (*matchmakerready)--;
 
  lock->Release();
}
