#ifndef IVTENTRY_H_
#define IVTENTRY_H_

#include "event.h"
#include "SysCall.h"

#define PREPAREENTRY(intNo, flag) \
void interrupt newintr##intNo(...); \
IVTEntry newivte##intNo(intNo, newintr##intNo); \
void interrupt newintr##intNo(...) { \
	newivte##intNo.signal(); \
	if (flag) { \
		newivte##intNo.callOldInt(); \
	} \
}


typedef unsigned char IVTNo;
class KernelEv;

class IVTEntry{
public:
	KernelEv* ev;
    pInterrupt oldInt, newInt;
	IVTNo ivtNum;

	IVTEntry(IVTNo n, pInterrupt p);
	~IVTEntry();

	void callOldInt(){ if(oldInt) (*oldInt)();};
	void signal();

    static IVTEntry* IVT[256];
};

#endif
