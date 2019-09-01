#ifndef LISTSIG_H_
#define LISTSIG_H_

class Thread;
typedef void (*SignalHandler)();
typedef unsigned SignalId;

class ListSig {
	struct node {
		SignalId signal;
		node *next;

		node(SignalId s) {
			signal = s;
			next = 0;
		}
	};
	node *first, *last;
	int cnt;
public:

	ListSig() {
		first = last = 0;
		cnt=0;
	};
	int getCnt(){ return cnt; }
	~ListSig();
	void push(SignalId p);
	SignalId pop();
	int isEmpty() {
		return ((first == 0) ? 1 : 0);
	};
	void remove(SignalId p);
};

#endif
