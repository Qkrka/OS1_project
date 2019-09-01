#ifndef OS_H_PCB_H_
#define OS_H_PCB_H_

#include "Thread.h"
#include "SCHEDULE.h"
#include "stdio.h"
#include "list.h"
#include "lock.h"

class PCB {

public:

	unsigned* stack;
	unsigned bp;
	unsigned sp;
	unsigned ss;
	ID id;
	Thread* mythread;
	int done;
	int working;
	int timeSlice;
	int waitRet;

	PCB();
	PCB(StackSize stackSize, Time ts, Thread* thr);
	PCB(StackSize stackSize);
	void startPCB();
	void waitToComplete();
	List waiting;
	//statics
	static List PCBs;
	/*static Elem* PCBs;
	static Elem* last;
	static Elem* waiting;
	static Elem* lastw;*/
	static int IDs;

	virtual ~PCB();

	static PCB* running, *idle;
	static void wrapper();
	static void idleWrapper(Thread* running);
};

#endif
