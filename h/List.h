#ifndef LIST_H_
#define LIST_H_

class Thread;
class PCB;
typedef int ID;

class List {
public:
	struct node {
		PCB *ptr;
		node *next;

		node(PCB * p) {
			ptr = p;
			next = 0;
		}
	};
	node *first, *last;

	List() {
		first = last = 0;
	}
	;
	~List();
	void push(PCB *p);
	PCB* pop();
	int isEmpty() {
		return ((first == 0) ? 1 : 0);
	};
	void remove(PCB *p);
	Thread* getThreadById(ID id);
};

#endif
