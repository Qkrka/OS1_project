#ifndef WLIST_H_
#define WLIST_H_

#include "PCB.h"

struct Node{
		PCB* pcb;
		Time time;
		Node* next;
		Node(PCB* p, Time t){
			pcb=p;
			time=t;
			next=NULL;
		}
	};
	
class WaitList {

private:

	Node* first;

public:

	WaitList(){
		first=NULL;
	}
	~WaitList();

	void add(PCB* p, Time t);
	PCB* pop();
	PCB* remove();
	int decTime();

	int isEmpty(){
		return ((first!=NULL) ? 0:1);
	}
};



#endif