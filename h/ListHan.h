#ifndef LISTHAN_H_
#define LISTHAN_H_

class Thread;
typedef void (*SignalHandler)();
typedef unsigned SignalId;

class ListHan {
public:
	struct node {
		SignalHandler handler;
		node *next;

		node(SignalHandler s) {
			handler = s;
			next = 0;
		}
	};
	node *first, *last;

	ListHan(){
		first=last=0;
	}
	~ListHan();
	void push(SignalHandler p);
	SignalHandler pop();
	int isEmpty() {
		return ((first == 0) ? 1 : 0);
	};
    void swap(SignalHandler h1, SignalHandler h2);
	void remove(SignalHandler p);
    void clear();
    void exec();
    void execZero();
    void inherit(ListHan l);
};

#endif
