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
#include "elevatortest.h"
#include <unistd.h>
// testnum is set in main.cc
int testnum = 1;
int lastPriority = 20;
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
    
    for (num = 0; num < 3; num++) {
		
		printf("------------------------------\n");
		printf("*** thread %d looped %d times\n", which, num);
		//allScheduler->Print();
		scheduler->TravelReadylist();
		while (stats->totalTicks < num * 30);
        currentThread->Yield();
		
    }
}

//----------------------------------------------------------------------
// ThreadTest1
// 	Set up a ping-pong between two threads, by forking a thread 
//	to call SimpleThread, and then calling SimpleThread ourselves.
//  status: ThreadStatus { JUST_CREATED, RUNNING, READY, BLOCKED };
//----------------------------------------------------------------------

void
ThreadTest1()
{
    DEBUG('t', "Entering ThreadTest1");
	printf("SystemTick: %d\n", UserTick);
    Thread *t1 = new Thread("forked thread1");
    Thread *t2 = new Thread("forked thread2");
	Thread *t3 = new Thread("forked thread3");
	
	t3->priority = 16;
	t3->Fork(SimpleThread, (void*)3);//为1，下为0
	
	printf("print ready list now\n");
	//t3->PrintId();
	
	allScheduler->Print();
	scheduler->TravelReadylist();
	//Thread *t4 = new Thread("forked thread4");
	//Thread *t5 = new Thread("forked thread5");
	
	t1->Fork(SimpleThread, (void*)1);//为1，下为0
	//t1->PrintId();
	allScheduler->Print();
	scheduler->TravelReadylist();
	t2->priority = 15;
	t1->priority = 20;
	t2->Fork(SimpleThread, (void*)2);//为1，下为0
	
	//t5->Fork(SimpleThread, (void*)5);//为1，下为0
	//t5->PrintId();
    //SimpleThread(0);
	allScheduler->Print();
	scheduler->TravelReadylist();
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
    default:
	printf("No test specified.\n");
	break;
    }
}

