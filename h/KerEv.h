#ifndef KEREV_H_
#define KEREV_H_

#include "IVTEntry.h"
#include "SCHEDULE.h"
typedef unsigned char IVTNo;
class PCB;

class KernelEv{
public:
	KernelEv(IVTNo n);
	~KernelEv();

	void signal();
	void wait();

	PCB * owner;
	int val;
	IVTNo ivtNum;

};


#endif
