#include "KerEv.h"
#include "IVTEntry.h"
#include "PCB.h"

KernelEv::KernelEv(IVTNo n) {
	val = 0;
	owner = PCB::running;
	ivtNum = n;
	IVTEntry * newEntry = IVTEntry::IVT[ivtNum];
	if (newEntry && newEntry->ev == 0)
		newEntry->ev = this;
}

KernelEv::~KernelEv() {
	signal();
	if (IVTEntry::IVT[ivtNum] && IVTEntry::IVT[ivtNum]->ev == this)
		IVTEntry::IVT[ivtNum]->ev = 0;
}
void KernelEv::wait() {
	if (owner == PCB::running) {
		val = 0;
		owner->working = 0;
		dispatch();
	}
}
void KernelEv::signal() {
	if (!val) {
		val = 1;
		owner->working = 1;
		Scheduler::put(owner);
		dispatch();
	}
}

