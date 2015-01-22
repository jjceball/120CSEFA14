// threadtest.cc
//	Simple test case for the threads assignment.
//
//	Create two threads, and have them context switch
//	back and forth between themselves by calling Thread::Yield,
//	to illustratethe inner workings of the thread system.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "synch.h"

// testnum is set in main.cc
int testnum = 1;

//----------------------------------------------------------------------
// SimpleThread
// 	Loop 5 times, yielding the CPU to another ready thread
//	each iteration.
//
//	"which" is simply a number identifying the thread, for debugging
//	purposes.
//----------------------------------------------------------------------

void
SimpleThread(int which)
{
    int num;

    for (num = 0; num < 5; num++) {
        printf("*** thread %d looped %d times\n", which, num);
        currentThread->Yield();
    }
}

//----------------------------------------------------------------------
// ThreadTest1
// 	Set up a ping-pong between two threads, by forking a thread
//	to call SimpleThread, and then calling SimpleThread ourselves.
//----------------------------------------------------------------------

void
ThreadTest()
{
    DEBUG('t', "Entering ThreadTest1");

    Thread *t = new Thread("forked thread");

    t->Fork(SimpleThread, 1);
    SimpleThread(0);
}

//----------------------------------------------------------------------
// LockTest1
//----------------------------------------------------------------------

Lock *locktest1 = NULL;

void
LockThread1(int param)
{
    printf("L1:0\n");
    locktest1->Acquire();
    printf("L1:1\n");
    currentThread->Yield();
    printf("L1:2\n");
    locktest1->Release();
    printf("L1:3\n");
}

void
LockThread2(int param)
{
    printf("L2:0\n");
    locktest1->Acquire();
    printf("L2:1\n");
    currentThread->Yield();
    printf("L2:2\n");
    locktest1->Release();
    printf("L2:3\n");
}

// acquire lock again
void
LockThread3(int param)
{
    printf("L3:0\n");
    locktest1->Acquire();
    printf("L3:1\n");
    //currentThread->Yield();
	locktest1->Acquire();
    printf("L3:2\n");
    locktest1->Release();
    printf("L3:3\n");
}

// delete held lock
void
LockThread4(int param)
{
    printf("L4:0\n");
    locktest1->Acquire();
    printf("L4:1\n");
    //currentThread->Yield();
	delete locktest1;
    printf("L4:2\n");
    locktest1->Release();
    printf("L4:3\n");
}

// release lock that isnt held
void
LockThread5(int param)
{
    printf("L5:0\n");
    locktest1->Release();
    printf("L5:1\n");
}

void
LockTest1()
{
    DEBUG('t', "Entering LockTest1");

    locktest1 = new Lock("LockTest1");

    Thread *t = new Thread("one");
    t->Fork(LockThread1, 0);
    t = new Thread("two");
    t->Fork(LockThread2, 0);
}

// test acquire lock twice
void
LockTest2()
{
    DEBUG('t', "Entering LockTest2");

    locktest1 = new Lock("LockTest2");

    Thread *t = new Thread("one");
    t->Fork(LockThread1, 0);
    t = new Thread("three");
    t->Fork(LockThread3, 0);
}

// test delete held lock
void
LockTest3()
{
    DEBUG('t', "Entering LockTest3");

    locktest1 = new Lock("LockTest3");

    Thread *t = new Thread("one");
    t->Fork(LockThread1, 0);
    t = new Thread("four");
    t->Fork(LockThread4, 0);
}

// test release when not held
void
LockTest4()
{
    DEBUG('t', "Entering LockTest4");

    locktest1 = new Lock("LockTest4");

    Thread *t = new Thread("one");
    t->Fork(LockThread1, 0);
    t = new Thread("five");
    t->Fork(LockThread5, 0);
}




//----------------------------------------------------------------------
// ConditionTest1
//----------------------------------------------------------------------


Condition *conditiontest1 = NULL;

void
ConditionThread1(int param)
{
    printf("L1:0\n");
    conditiontest1->Wait(locktest1);
    printf("L1:1\n");
    currentThread->Yield();
    printf("L1:2\n");
    conditiontest1->Signal(locktest1);
    printf("L1:3\n");
    currentThread->Yield();
    printf("L1:4\n");
    conditiontest1->Broadcast(locktest1);
    printf("L1:5\n");
}

void
ConditionThread2(int param)
{
    printf("L1:0\n");
    conditiontest1->Wait(locktest1);
    printf("L1:1\n");
    currentThread->Yield();
    printf("L1:2\n");
    conditiontest1->Signal(locktest1);
    printf("L1:3\n");
    currentThread->Yield();
    printf("L1:4\n");
    conditiontest1->Broadcast(locktest1);
    printf("L1:5\n");
}

void
ConditionTest1()
{
    DEBUG('t', "Entering ConditionTest1");

    conditiontest1 = new Condition("ConditionTest1");
    locktest1 = new Lock("LockTest1");

    Thread *t = new Thread("one");
    t->Fork(ConditionThread1, 0);
    t = new Thread("two");
    t->Fork(ConditionThread2, 0);
}


// wait on CV without holding a lock
void
ConditionThread3(int param)
{
    printf("C1:0\n");	
    locktest1->Acquire();
    printf("C1:1\n");
    conditiontest1->Wait(locktest1);
    printf("C1:2\n");
    currentThread->Yield();
    printf("C1:3\n");
    locktest1->Release();
    printf("C1:4\n");

    
    

}


void
ConditionThread4(int param)
{
    
    printf("C2:0\n");
    locktest1->Acquire();
    printf("C2:1\n");
    conditiontest1->Wait(locktest1);    
    printf("C2:2\n");
    currentThread->Yield();
    printf("C2:3\n");
    locktest1->Release();
    printf("C2:4\n");
    
    
    
}

void
ConditionThread5(int param)
{
    printf("C3:0\n");
    locktest1->Acquire();
    printf("C3:1\n");    
    conditiontest1->Signal(locktest1);
    printf("C3:2\n");
    locktest1->Release();
    printf("C3:3\n");
    

}


void
ConditionThread6(int param)
{
    printf("C3:0\n");
    locktest1->Acquire();
    printf("C3:1\n");    
    conditiontest1->Broadcast(locktest1);
    printf("C3:2\n");
    locktest1->Release();
    printf("C3:3\n");
}



void
ConditionTest2()
{
// lock not held
    DEBUG('t', "Entering ConditionTest2");
    locktest1 = new Lock("LockTestA");
	conditiontest1 = new Condition("ConditionTest2");
	
    Thread *t = new Thread("one");
    t->Fork(ConditionThread1, 0);
    t = new Thread("three");
    t->Fork(ConditionThread3, 0);
}

void 
ConditionTest3()
{
    DEBUG('t', "Entering ConditionTest3");
    locktest1 = new Lock("LockTestA");
    	conditiontest1 = new Condition("ConditionTest3");
    Thread *t = new Thread("three");
    t->Fork(ConditionThread3, 0);
    t = new Thread("four");
    t->Fork(ConditionThread4, 0);
    t = new Thread("five");
    t->Fork(ConditionThread5, 0);

}


void 
ConditionTest4()
{
    DEBUG('t', "Entering ConditionTest4");
    locktest1 = new Lock("LockTestA");
    	conditiontest1 = new Condition("ConditionTest4");
    Thread *t = new Thread("three");
    t->Fork(ConditionThread3, 0);
    t = new Thread("four");
    t->Fork(ConditionThread4, 0);
    t = new Thread("six");
    t->Fork(ConditionThread6, 0);

}



//----------------------------------------------------------------------
// MailboxTest1
//----------------------------------------------------------------------


Mailbox *Mailboxtest1 = NULL;
Condition* mailcond1 = NULL;
Condition* bufcond1 = NULL;
Lock* maillock1 = NULL;
Lock* buflock1 = NULL; 
int* message1;
int* message2;

void
MailboxThread1(int param)
{
    maillock1->Acquire();	
    printf("L1:0\n");
    Mailboxtest1->Send(123);
    printf("L1:1\n");
    currentThread->Yield();
    printf("L1:2\n");
    maillock1->Release();
}

void
MailboxThread2(int param)
{
    maillock1->Acquire();	
    printf("L1:0\n");
    Mailboxtest1->Receive(message1);
    printf("L1:1\n");
    currentThread->Yield();
    printf("L1:2\n");
    maillock1->Release();
}



void
MailboxTest1()
{
    DEBUG('t', "Entering MailboxTest1");
 
    maillock1 = new Lock("MailLock1");	
    
    Mailboxtest1 = new Mailbox("Mailboxtest1");

    Thread *t = new Thread("send");
    t->Fork(MailboxThread1, 123);
    t = new Thread("receive");
    t->Fork(MailboxThread2, 0);


}

//----------------------------------------------------------------------
// JoinTest1
//----------------------------------------------------------------------


Thread *parent = NULL;
Thread *child = NULL;

void
JoinThread1(int param)
{
    printf("L1:0\n");
    parent->Join();
    printf("L1:1\n");
    currentThread->Yield();
    printf("L1:2\n");
}

void
JoinTest1()
{
    DEBUG('t', "Entering JoinTest1");

    parent = new Thread("Parent1", 1);
    child = new Thread("Child1", 0);

    Thread *t = new Thread("one");
    t->Fork(JoinThread1, 0);
}



//----------------------------------------------------------------------
// Test Whale
//----------------------------------------------------------------------

void Whale_male(int param){
	Whale* newWhale = (Whale*) param;
	currentThread->Yield();
	printf("W_Male:0\n");	
	newWhale->Male();
	currentThread->Yield();	
	printf("W_Male:1\n");
}

void Whale_female(int param){
	Whale* newWhale = (Whale*) param;
	currentThread->Yield();
	printf("W_Female:0\n");	
	newWhale->Female();
	currentThread->Yield();
	printf("W_Female:1\n");
	    
}

void Whale_match(int param){
	Whale* newWhale = (Whale*) param;
	currentThread->Yield();
	printf("W_Match:0\n");	
	newWhale->Matchmaker();
	currentThread->Yield();
	printf("W_Match:1\n");
	
		    
}
//----------------------------------------------------------------------