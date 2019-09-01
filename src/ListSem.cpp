#include "ListSem.h"
#include "KerSem.h"
KernelSem* ListSem::pop() {
	if (first == 0)
		return 0;
	KernelSem* p = first->ptr;
	node* n = first;
	first = first->next;
	if (first == 0)
		last = 0;
	delete n;
	return p;
}

void ListSem::push(KernelSem* p) {
	node* n = new node(p);
	if (first == 0)
		first = n;
	else
		last->next = n;
	last = n;
}
ListSem::~ListSem() {
	node *tek = first;
	while (first != 0) {
		tek = first;
		first = first->next;
		delete tek;
	}
}

void ListSem::remove(KernelSem* p) {
	node* tek = first, *prev = 0;
	while (tek != 0) {
		if (tek->ptr == p) {
			if (prev != 0) {
				prev->next = tek->next;
				delete tek;
				return;
			} else {
				first = tek->next;
				if (first == 0)
					last = 0;
				return;
			}
		}
		prev = tek;
		tek = tek->next;
	}
}

void ListSem::decTime() {
	node* pok = first;
//    printf("dectime \n");
	while (pok != 0) {
//    	printf("inlist \n");
		pok->ptr->timeBlk.decTime();
		pok = pok->next;
	}
}
