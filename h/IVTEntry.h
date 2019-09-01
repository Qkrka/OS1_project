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
//
//#define PREPAREENTRY(intNo, callOld) \
//void interrupt newInt##intNo(...); \
//IVTEntry newEntry##intNo(intNo, newInt##intNo); \
//void interrupt newInt##intNo(...) { \
//	if (callOld) { \
//		newEntry##intNo.callOldInt(); \
//	} \
//	newEntry##intNo.signal(); \
//}

typedef unsigned char IVTNo;
class KernelEv;

class IVTEntry{
public:
	// Vektor prekida
    pInterrupt oldInt, newInt;
	KernelEv* ev;
	IVTNo ivtNum;

	IVTEntry(IVTNo n, pInterrupt p);
	~IVTEntry();

	void callOldInt(){ if(oldInt) (*oldInt)();}; // Zovi staru prekidnu rutinu
	void signal();

    static IVTEntry* IVT[256];
    /*
    IVTEntry(IVTNo ivtN,pInterrupt tR);
	~IVTEntry();
	void signal();
	void callOldI(){
		if(oldR!=0)
			(*oldR)();
	};

	static IVTEntry *IVTP[256];
	IVTNo ivtN;
	KernelEv *ev;
	pInterrupt oldR,newR; */
};

#endif
