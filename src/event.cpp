#include "event.h"
#include "Lock.h"

Event::Event(IVTNo ivtNo){
#ifndef BCC_BLOCK_IGNORE
	asm {
		pushf;
		cli;
	}
#endif
	myImpl = new KernelEv(ivtNo);
#ifndef BCC_BLOCK_IGNORE
	asm {
		popf;
	}
#endif
}

Event::~Event(){
#ifndef BCC_BLOCK_IGNORE
	asm {
		pushf;
		cli;
	}
#endif
	delete myImpl;
#ifndef BCC_BLOCK_IGNORE
	asm {
		popf;
	}
#endif
}

void Event::wait(){
#ifndef BCC_BLOCK_IGNORE
	asm {
		pushf;
		cli;
	}
#endif
	myImpl->wait();
#ifndef BCC_BLOCK_IGNORE
	asm {
		popf;
	}
#endif
}

void Event::signal(){
#ifndef BCC_BLOCK_IGNORE
	asm {
		pushf;
		cli;
	}
#endif
	myImpl -> signal();
#ifndef BCC_BLOCK_IGNORE
	asm {
		popf;
	}
#endif
}
