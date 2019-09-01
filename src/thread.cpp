#include "thread.h"
#include "PCB.h"
#include "lock.h"

extern int context_swap_req;

Thread::Thread(StackSize stackSize, Time timeSlice) {

	myPCB = new PCB(stackSize, timeSlice, this);
	myPCB->startPCB();

}
void Thread::start() {
	myPCB->startPCB();
};

void Thread::waitToComplete() {
	myPCB->waitToComplete();
}
Thread::~Thread() {
	waitToComplete();
	delete myPCB;
}
ID Thread::getId() {
	return myPCB->id;
}
ID Thread::getRunningId() {
	return PCB::running->id;
}
Thread* Thread::getThreadById(ID id) {
	return PCB::PCBs.getThreadById(id);
}

#include "STDIO.H"
void dispatch() {
	context_swap_req = 1;
//	printf("dispatch\n");
	asm int 8h;
}
