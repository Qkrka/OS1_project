#include "KerEv.h"
#include "IVTEntry.h"
#include "PCB.h"

KernelEv::KernelEv(IVTNo n) {
	caller = PCB::running;

	ivtNum = n;
	val = 0;

	// Povezujemo sa tabelom
	IVTEntry * newEntry = IVTEntry::IVT[ivtNum];

	if (newEntry && newEntry->ev == 0) //da li je preparovan?
		newEntry->ev = this;

}

KernelEv::~KernelEv() {
	// Zovemo signal da odblokiramo
	signal(); //smem li ovo da radim ili samo da putujem u sched?

	// Brisemo ulaz
	if (IVTEntry::IVT[ivtNum]
			&& IVTEntry::IVT[ivtNum]->ev == this) // ako je ovaj ulaz odgovarajuc za ovu
		IVTEntry::IVT[ivtNum]->ev = 0; // raskidamo vezu
}

// Signal binarnom semaforu
void KernelEv::signal() {
	if (val == 0) {
		val = 1;
		caller->working = 1;
		Scheduler::put(caller); //ubaci se u scheduler

		dispatch(); //dispatch na neku novu nit
	}
}

void KernelEv::wait() {

	// ako je running caller moze uopste da se waituj
	if (PCB::running == caller) {

		val = 0;
		caller->working = 0; //nece uci u scheduler
		dispatch();

	}

}

