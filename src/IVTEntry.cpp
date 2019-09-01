#include "IVTEntry.h"
#include "KerEv.h"
#include <dos.h>

IVTEntry *IVTEntry::IVT[256] ={0};

IVTEntry::IVTEntry(IVTNo n,pInterrupt p) {
	ivtNum=n;
    ev=0;
	newInt=p;
#ifndef BCC_BLOCK_IGNORE
	oldInt=getvect(ivtNum);
	setvect(ivtNum,newInt);
#endif
    IVT[ivtNum]=this;
}

IVTEntry::~IVTEntry() {

#ifndef BCC_BLOCK_IGNORE
	setvect(ivtNum,oldInt);
#endif
	IVT[ivtNum]=0;
    callOldInt();
}


void IVTEntry::signal(){
	if (ev){
			ev->signal();
		}
}
