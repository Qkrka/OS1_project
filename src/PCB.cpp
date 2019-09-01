#include "PCB.h"
#include <dos.h>
#include "stdio.h"

int PCB::IDs = 1;
PCB* PCB::running = 0;
PCB* PCB::idle = 0;
List PCB::PCBs;


PCB::PCB(){
	id=-1;
	working=0;
	done=0;
	stack=NULL;
	waitRet = 1;
}

PCB::PCB(StackSize stackSize, Time t, Thread* thr) {
	waitRet = 1;
	PCBs.push(this);

	this->timeSlice = timeSlice;
	int size = stackSize / 2;
	stack=new unsigned[size];
/* 
	int thrSeg, thrOff;
#ifndef BCC_BLOCK_IGNORE
	thrSeg = FP_SEG(thr);
	thrOff = FP_OFF(thr);
#endif

	stack[size - 1] = thrSeg;
	stack[size - 2] = thrOff;*/
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
	id = IDs++;

}

PCB::PCB(StackSize stackSize) {
	waitRet = 1;
	PCBs.push(this);
	timeSlice = 1;
	int size = stackSize / 2;

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
}


void PCB::wrapper() {
	running->mythread->run();
	//printf("endofrun\n");
	while (!running->waiting.isEmpty()) {
		PCB* p = PCB::running->waiting.pop();
		p->working=1;
		Scheduler::put(p);

	}
	running->working = 0;
	dispatch();
}
void PCB::idleWrapper(Thread* running) {
	while (1) {
		//printf("idle");
	}
}


PCB::~PCB() {
	PCBs.remove(this);
	delete stack;
}

void PCB::startPCB() {
	//printf("StartPCB usao ");
	if (!working) {
		working = 1;
		//printf("i Ubacio %d \n", id);
		Scheduler::put(this);
	}
}

void PCB::waitToComplete() {
	if (working) {
		printf("w8complete is working ==1\n");
		running->working = 0;
		waiting.push(running);
		dispatch();
	}
}
