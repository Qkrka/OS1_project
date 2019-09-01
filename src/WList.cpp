#include"WList.h"

#include "PCB.h"
#include "SCHEDULE.h"

void WaitList::add(PCB *p,Time t){

	if(t==0) return;
	Node *tek=first,*prev=0;

	if(first==NULL)
		first=new Node(p,t);
	else{
		if(first->time>=t){
			Node *n=new Node(p,t);
			first->time-=t;
			n->next=first;
			first=n;
		}else{
			while (tek!=NULL && tek->time<=t){
				t-=tek->time;
				prev=tek;
				tek=tek->next;
			}

			Node* n=new Node(p,t);
			n->next=tek;
			prev->next=n;
			if(tek!=0)
				tek->time-=t;
		}
	}
};

int WaitList::decTime(){

	Node* t=first;
	int cnt = 0;

	if(first!=0) first->time--;

	while (first!=NULL && first->time<=0){
		//first->pcb->wasunblocked = 1; za wait povratnu vrednost
		first->pcb->working = 1;
		first->pcb->waitRet=0;
		Scheduler::put(first->pcb);
		t=first;
		first=first->next;
		first->time+=t->time;
		cnt++;
		delete t;
	}
	return cnt;
}

PCB* WaitList::pop(){
	if(first==NULL)
		return 0;
	PCB* p=first->pcb;
	Node* n=first;
	first=first->next;
	first->time += n->time;
	delete n;
	return p;
}

WaitList::~WaitList() {
	Node *tek=first;
	while(tek!=0){
		first=first->next;
		delete tek;
		tek=first;
	}
}

