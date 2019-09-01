#ifndef PCB_H_
#define PCB_H_

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
	int toKill;

	PCB();
	PCB(StackSize stackSize, Time ts, Thread* thr);
	PCB(StackSize stackSize);
	void startPCB();
	void waitToComplete();
	List waiting;
	static List PCBs;
	static int IDs;
	virtual ~PCB();
	static PCB* running, *idle;
	static void wrapper(Thread* me);
	static void idleWrapper();
};

#endif
