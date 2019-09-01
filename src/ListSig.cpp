#include "ListSig.h"

SignalId ListSig::pop(){
	if(first==0)
		return 0;
	SignalId p=first->signal;
	node* n=first;
	first=first->next;
	cnt--;
	if(first==0) last=0;
	delete n;
	return p;
}

void ListSig::push(SignalId p){
	node* n=new node(p);
	if(first==0)
		first=n;
	else
		last->next=n;
	last=n;
	cnt++;
}
ListSig::~ListSig() {
	node *tek=first;
	while (first!=0){
        tek=first;
		first=first->next;
		delete tek;
	}
	cnt=0;
}

void ListSig::remove(SignalId p){
    node* tek = first, *prev=0;
	while (tek != 0) {
		if (tek->signal == p) {
			if (prev != 0) {
				prev->next = tek->next;
				delete tek;
				return;
			} else {
				first = tek->next;
				if (first == 0) last = 0;
                return;
			}
			cnt--;
		}
		prev = tek;
		tek = tek->next;
	}
}
