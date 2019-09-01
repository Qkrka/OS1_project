#include "Lock.h"
#include "thread.h"

volatile int lockFlag = 0;
extern volatile int context_swap_req;

void lock(){
	++lockFlag;
}

void unlock(){
	--lockFlag;
	if(context_swap_req && lockFlag==0) dispatch();
}
