#include "KerSem.h"
#include "ListSem.h"

extern volatile int lockFlag;
ListSem KernelSem::sems;

KernelSem::KernelSem(int v) {
	val=((v<0)?0:v);
    sems.push(this);
}

KernelSem::~KernelSem() {
	PCB* ptr;
//	printf("KERSEM DESTR\n");
	while (!valBlk.isEmpty()){
		PCB* tek=valBlk.pop();
		tek->working=1;
		Scheduler::put(tek);
	}
	while (!timeBlk.isEmpty()){
		PCB* tek=timeBlk.pop();
		tek->working=1;
		Scheduler::put(timeBlk.pop());
	}
    sems.remove(this);
}

int KernelSem::wait(Time maxTimeToWait) {
	lock();
	if (--val < 0) {
        PCB::running->working = 0;
		if (maxTimeToWait == 0) {
			valBlk.push(PCB::running);
		} else {
//			printf("timeblkadd id: %d\n",PCB::running->id);
			timeBlk.add(PCB::running, maxTimeToWait);
		}
		unlock();
		dispatch();
		lock();
		int ret=PCB::running->waitRet;
		PCB::running->waitRet=1;
//		printf("ID = %d, RET= %d\n",PCB::running->id,ret);
		unlock();
		return ret;
	}
	unlock();
	return 1;
}

int KernelSem::signal(int n) {
	lock();
	//printf("Usao u signal\n");
	if (n < 0){
        unlock();
        return n;
    }
    int toUnblock = ((n==0)?1:n);
//    printf("tounblock= %d\n", toUnblock);
	int cnt = 0;

	if (val <= 0) {
		val += toUnblock;
		for (int i = 0; i < toUnblock; i++) {
            PCB* p = NULL;
            if(!valBlk.isEmpty()) p=valBlk.pop();
            else if(!timeBlk.isEmpty()) p=timeBlk.pop();
            if(p==NULL) break;
			p->working = 1;
//			printf("UNBLOKOVAO SAM ID = %d\n",p->id);
			Scheduler::put(p);
			cnt++;
		}
		unlock();
		return cnt;
	}
	val += toUnblock;
	unlock();
	return 0;
}

int KernelSem::getVal() const {
	return val;
}
