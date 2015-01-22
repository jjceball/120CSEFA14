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
ThreadTest1()
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
    printf("L1:0\n");
    Mailboxtest1->Send(5);
    printf("L1:1\n");
    currentThread->Yield();
    printf("L1:2\n");
    Mailboxtest1->Receive(message2);
    printf("L1:3\n");
    currentThread->Yield();
    printf("L1:4\n");
}

void
MailboxThread2(int param)
{
    printf("L1:0\n");
    Mailboxtest1->Send(10);
    printf("L1:1\n");
    currentThread->Yield();
    printf("L1:2\n");
    Mailboxtest1->Receive(message1);
    printf("L1:3\n");
    currentThread->Yield();
    printf("L1:4\n");
}

void
MailboxTest1()
{
    DEBUG('t', "Entering MailboxTest1");

    Mailboxtest1 = new Mailbox("Mailboxtest1", mailcond1, bufcond1, maillock1, buflock1);

    Thread *t = new Thread("one");
    t->Fork(MailboxThread1, 0);
    t = new Thread("two");
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
    child->Join();
}

void
JoinThread2(int param)
{
    printf("L1:0\n");
    child->Join();
    printf("L1:1\n");
    currentThread->Yield();
    printf("L1:2\n");
    parent->Join();
}

void
JoinTest1()
{
    DEBUG('t', "Entering JoinTest1");

    parent = new Thread("Parent1", 1);
    child = new Thread("Child1", 0);

    Thread *t = new Thread("one");
    t->Fork(JoinThread1, 0);
    t = new Thread("two");
    t->Fork(JoinThread2, 0);
}



//----------------------------------------------------------------------
// ThreadTest
// 	Invoke a test routine.
//----------------------------------------------------------------------

void
ThreadTest()
{
    switch (testnum) {
    case 1:
        ThreadTest1();
        break;
    case 2:
        LockTest1();
	break;
    case 3:
	ConditionTest1();
	break; 
    case 4:
        MailboxTest1();
	break;
    case 5:
        JoinTest1();
        break;
    default:
        printf("No test specified.\n");
        break;
    }
}

