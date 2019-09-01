#ifndef LISTSEM_H_
#define LISTSEM_H_

class KernelSem;

class ListSem {
public:
	struct node {
		KernelSem* ptr;
		node *next;

		node(KernelSem* p) {
			ptr = p;
			next = 0;
		}
	};
	node *first, *last;

	ListSem() {
		first = last = 0;
	}
	;
	~ListSem();
	void push(KernelSem *p);
	KernelSem* pop();
	int isEmpty() {
		return ((first == 0) ? 1 : 0);
	};
	void remove(KernelSem *p);
	void decTime();
};

#endif
