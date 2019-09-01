#include "List.h"
#include "PCB.h"

PCB* List::pop(){
	if(first==0)
		return 0;
	PCB* p=first->ptr;
	node* n=first;
	first=first->next;
	if(first==0) last=0;
	delete n;
	return p;
}

void List::push(PCB* p){
	node* n=new node(p);
	if(first==0)
		first=n;
	else
		last->next=n;
	last=n;
}
List::~List() {
	node *tek=first;
	while (first!=0){
        tek=first;
		first=first->next;
		delete tek;
	}
}

void List::remove(PCB* p){
    node* tek = first, *prev=0;
	while (tek != 0) {
		if (tek->ptr == p) {
            tek->next = 0;
			if (prev != 0) {
				prev->next = tek->next;
				delete tek;
				return;
			} else {
				first = tek->next;
				if (first == 0) last = 0;
                return;
			}
		}
		prev = tek;
		tek = tek->next;
	}
}

Thread* List::getThreadById(ID id) {
	node* p = first;
	while (p) {
		if (p->ptr->id == id) break;
		p = p->next;
	}
	if (!p) return 0;
	return p->ptr->mythread;
}
