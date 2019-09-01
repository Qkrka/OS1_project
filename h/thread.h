#ifndef _thread_h_
#define _thread_h_
#include "ListHan.h"
#include "ListSig.h"
typedef unsigned long StackSize;
const StackSize defaultStackSize = 4096;
typedef unsigned int Time; // time, x 55ms
const Time defaultTimeSlice = 2; // default = 2*55ms
typedef int ID;
class PCB;
// Kernel's implementation of a user's thread
typedef void (*SignalHandler)();
typedef unsigned SignalId;

class Thread {
public:
	void start();
	void waitToComplete();
	virtual ~Thread();
	ID getId();
	static ID getRunningId();
	static Thread * getThreadById(ID id);

	PCB* myPCB;
	ListHan handlers[16];

	void swap(SignalId id, SignalHandler hand1, SignalHandler hand2);
	void registerHandler(SignalId signal, SignalHandler handler);
	void unregisterAllHandlers(SignalId id);

	void signal(SignalId signal);
	void handle();

	void blockSignal(SignalId signal);
	void unblockSignal(SignalId signal);

	static void blockSignalGlobally(SignalId signal);
	static void unblockSignalGlobally(SignalId signal);
protected:
	static int globalBlock[16];
	int localBlock[16];
	ListSig signals;
	Thread* parent;
//	int nizodnula[100];
	friend class PCB;
	Thread(StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice);
	virtual void run() {}
};
void handlerZero();
void dispatch();
#endif
