#include "event.h"
#include "Lock.h"

// Konstruktor
Event::Event(IVTNo ivtNo){
	locki();
	myImpl = new KernelEv(ivtNo);
	unlocki();
}

// Destruktor
Event::~Event(){
	locki();
	delete myImpl;
	unlocki();
}

// Sacekaj
void Event::wait(){
	locki();
	myImpl->wait();
	unlocki();
}

void Event::signal(){
	locki();
	myImpl -> signal();
	unlocki();
}
