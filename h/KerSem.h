#ifndef H_KSEM_H_
#define H_KSEM_H_

#include "PCB.h"
#include "WList.h"
#include "ListSem.h"

class KernelSem {
public:
	int val;
	List valBlk;
	WaitList timeBlk;

	KernelSem(int v);
	virtual ~KernelSem();

	int wait(Time maxTimeToWait);
	int signal(int n);
	int getVal() const;

	static ListSem sems;
};

#endif
