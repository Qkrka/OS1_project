#include "ListHan.h"
#include "STDIO.H"

//void handlerZero();

SignalHandler ListHan::pop() {
	if (first == 0)
		return 0;
	SignalHandler p = first->handler;
	node* n = first;
	first = first->next;
	if (first == 0)
		last = 0;
	delete n;
	return p;
}

void ListHan::push(SignalHandler p) {
	node* n = new node(p);
	if (first == 0)
		first = n;
	else
		last->next = n;
	last = n;
}

void ListHan::remove(SignalHandler p) {
	node* tek = first, *prev = 0;
	while (tek != 0) {
		if (tek->handler == p) {
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

void ListHan::swap(SignalHandler h1, SignalHandler h2) {
	node* tek = first, *han1 = 0, *han2 = 0;
	while (tek) {
		if (tek->handler == h1)
			han1 = tek;
		if (tek->handler == h2)
			han2 = tek;
		tek = tek->next;
	}
	if (han1 == 0 || han2 == 0)
		return;

	SignalHandler temp = han1->handler;
	han1->handler = han2->handler;
	han2->handler = temp;

}
;

ListHan::~ListHan() {
	while (first != 0) {
		last = first;
		first = first->next;
		delete last;
	}
}

void ListHan::clear() {
	while (first != 0) {
		last = first;
		first = first->next;
		delete last;
	}
	last = 0;
}
;

void ListHan::exec() {
	node* tek = first;
//	printf("execfrist: %p\n",first);
	while (tek) {
		tek->handler();
		tek = tek->next;
	}
}
;

void ListHan::execZero() {
	node* kill = first;
//	printf("execzero");
	if (first) {
		node* tek = first->next;
		while (tek) {
			tek->handler();
			tek = tek->next;
		}
	}
//	printf("kill je sta? %p\n",kill);
	kill->handler();
}

//doesn't work
void ListHan::inherit(ListHan l) {
	node* tek = l.first;
	while (tek) {
		push(tek->handler);
		tek = tek->next;
	}
}
;
