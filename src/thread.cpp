#include "thread.h"
#include "PCB.h"
#include "lock.h"

extern volatile int context_swap_req;
int Thread::globalBlock[16] = { 0 };

Thread::Thread(StackSize stackSize, Time timeSlice) {

	myPCB = new PCB(stackSize, timeSlice, this);
//	printf("pcb=%p\n", myPCB);
	parent = PCB::running->mythread;
//	if(parent == 0){}
//	printf("size=%d", sizeof(*this));
	if (parent == 0) {
		for (int i = 0; i < 16; i++)
			localBlock[i] = 0;
	} else {
		for (int i = 0; i < 16; i++) {
			localBlock[i] = parent->localBlock[i];
			handlers[i].inherit(parent->handlers[i]);
		}
	}

	myPCB->startPCB();
}

void handlerZero() {
	PCB::running->toKill = 1;
//	printf("markirana nit %d", PCB::running->id);
//	unlock();
}

void Thread::start() {
	myPCB->startPCB();
}
;

void Thread::waitToComplete() {
	myPCB->waitToComplete();
}
Thread::~Thread() {
//	waitToComplete();
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
void Thread::registerHandler(SignalId signal, SignalHandler handler) {
	lock();
	if (signal >= 0 && signal <= 15)
		handlers[signal].push(handler);
	unlock();
}
void Thread::unregisterAllHandlers(SignalId id) {
	lock();
	if (id >= 0 && id <= 15)
		handlers[id].clear();
	unlock();
}
void Thread::swap(SignalId id, SignalHandler hand1, SignalHandler hand2) {
	lock();
	if (id >= 0 && id <= 15)
		handlers[id].swap(hand1, hand2);
	unlock();
}
void Thread::blockSignal(SignalId signal) {
	lock();
	if (signal >= 0 && signal <= 15)
		localBlock[signal] = 1;
	unlock();
}
void Thread::blockSignalGlobally(SignalId signal) {
	lock();
	if (signal >= 0 && signal <= 15)
		globalBlock[signal] = 1;
	unlock();
}
void Thread::unblockSignal(SignalId signal) {
	lock();
	if (signal >= 0 && signal <= 15)
		localBlock[signal] = 0;
	unlock();
}
void Thread::unblockSignalGlobally(SignalId signal) {
	lock();
	if (signal >= 0 && signal <= 15)
		globalBlock[signal] = 0;
	unlock();
}
extern int syscall;
void Thread::signal(SignalId id) {
	lock();
	if ((id >= 0 && id <= 15) && ((id != 1 && id != 2) || syscall == 1)) {
		signals.push(id);
//		printf("signal %d\n", id);
	}
	unlock();
}

void Thread::handle() {
	int c = signals.getCnt();
	if (c == 0)
		return;
	lock();
#ifndef BCC_BLOCK_IGNORE
	asm {
		pushf;
		sti;
	}
#endif

	int i = 0;
	while (i++ < c) {
		SignalId s = signals.pop();
		if (globalBlock[s] == 1 || localBlock[s] == 1)
			signals.push(s);
		else {
//			printf("handlujem signal %d u niti %d\n", s, PCB::running->id);
			handlers[s].exec();
			if (s == 0) {
				handlerZero();
				break;
			}
		}
	}
#ifndef BCC_BLOCK_IGNORE
	asm popf;
#endif
	unlock();
}

#include "STDIO.H"
void dispatch() {
#ifndef BCC_BLOCK_IGNORE
	asm {
		pushf;
		cli;
	}
#endif
	context_swap_req = 1;
asm int 8h;
#ifndef BCC_BLOCK_IGNORE
	asm popf;
#endif
}
