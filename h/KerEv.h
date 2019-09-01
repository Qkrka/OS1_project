#ifndef KEREV_H_
#define KEREV_H_

#include "IVTEntry.h"
#include "SCHEDULE.h"

class PCB;
class Event;
typedef unsigned char IVTNo;
class KernelEv{

public:
	KernelEv(IVTNo n);
	~KernelEv();

	void signal();
	void wait();

	IVTNo ivtNum;
	PCB * caller;
	int val;

};


#endif /* H_KEVENT_H_ */
