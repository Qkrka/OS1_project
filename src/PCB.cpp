#include "PCB.h"
#include <dos.h>
#include "stdio.h"

int PCB::IDs = 1;
PCB* PCB::running = 0;
PCB* PCB::idle = 0;
List PCB::PCBs;

PCB::PCB() {
	id = -1;
	working = 0;
	done = 0;
	stack = NULL;
	waitRet = 1;
	mythread = 0;
	toKill = 0;
}

PCB::PCB(StackSize size, Time t, Thread* thr) {
	waitRet = 1;
	PCBs.push(this);

	this->timeSlice = timeSlice;
//	int size = stackSize / 2;
//	stack=new unsigned[size];
	size /= 2;
	stack = new unsigned[size];
	int thrSeg, thrOff;
#ifndef BCC_BLOCK_IGNORE
	thrSeg = FP_SEG(thr);
	thrOff = FP_OFF(thr);
#endif

	stack[size - 1] = thrSeg;
	stack[size - 2] = thrOff;
	stack[size - 5] = 0x200;
#ifndef BCC_BLOCK_IGNORE
	stack[size - 6] = FP_SEG(wrapper);
	stack[size - 7] = FP_OFF(wrapper);
	ss = FP_SEG(&stack[size - 16]);
	sp = FP_OFF(&stack[size - 16]);
	bp = FP_OFF(&stack[size - 16]);
#endif

	stack[size - 16] = bp;

	mythread = thr;
	done = 0;
	working = 0;
	toKill = 0;
	id = IDs++;

}

PCB::PCB(StackSize size) {
	waitRet = 1;
//	PCBs.push(this);
	timeSlice = 1;
	size /= 2;
	stack = new unsigned[size];
	stack[size - 5] = 0x200;
#ifndef BCC_BLOCK_IGNORE
	stack[size - 6] = FP_SEG(idleWrapper);
	stack[size - 7] = FP_OFF(idleWrapper);
	ss = FP_SEG(&stack[size - 16]);
	sp = FP_OFF(&stack[size - 16]);
	bp = FP_OFF(&stack[size - 16]);
#endif

	stack[size - 16] = bp;
	done = 0;
	working = 1;
	id = 0;
	toKill = 0;
}

int syscall = 0;

void PCB::wrapper(Thread* me) {
	running->mythread->run();
	//printf("endofrun\n");
	while (!running->waiting.isEmpty()) {
		PCB* p = PCB::running->waiting.pop();
		p->working = 1;
		Scheduler::put(p);
	}

	lock();
	syscall = 1;
	if (running->mythread) {
		running->mythread->signal(2);
//		printf("parent = %p\n", running->mythread->parent);
		if (running->mythread->parent) {
//			printf("signalin 1\n");
			running->mythread->parent->signal(1);
		}
	}
	syscall = 0;
	unlock();
//	printf("sad cu da te sredim id=%d\n", running->id);
	if (running->mythread)
		running->mythread->handle();
//	printf("sredio id= %d\n", running->id);

	running->working = 0;
	running->toKill = 1;
	dispatch();
}
void PCB::idleWrapper() {
	while (1) {
		//printf("idle");
	}
}

PCB::~PCB() {
//	printf("pcb destr\n");
	PCBs.remove(this);
	if(stack!=0)delete stack;
}

void PCB::startPCB() {
//	printf("StartPCB usao ");
	if (!working) {
		working = 1;
//		printf("i Ubacio %d  working = %d\n", id, working);

		Scheduler::put(this);
	}
}

void PCB::waitToComplete() {
//	printf("nit %d status %d\n", id, working);
	if (toKill == 0) {
		running->working = 0;
		waiting.push(running);
		dispatch();
	}
}
