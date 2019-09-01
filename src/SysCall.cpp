#include "SysCall.h"
#include <dos.h>
#include "KerSem.h"
#include "Lock.h"

pInterrupt oldTimer;

void inic() {
#ifndef BCC_BLOCK_IGNORE
	asm cli;
	oldTimer=getvect(0x0008);
	setvect(0x0008,timer);
	asm sti;
#endif
PCB::idle=new PCB(500);
PCB* mainPCB = new PCB();
PCB::running=mainPCB;
}

void restore() {
#ifndef BCC_BLOCK_IGNORE
	asm cli;
	setvect(0x0008,oldTimer);
	asm sti;
#endif
}

unsigned tsp;
unsigned tss;
unsigned tbp;
volatile int cnt;
volatile int context_swap_req = 0;
extern int tick();
extern volatile int lockFlag;

#include "stdio.h"

PCB* temp = 0;

void interrupt timer(...) {
//	printf("timer\n");
if (!context_swap_req) {
cnt--;
tick();
//printf("dectime");
KernelSem::sems.decTime();
(*oldTimer)();
}
	//	printf("contswreq = %d",context_swap_req);
if (cnt <= 0 || context_swap_req) {
if (!lockFlag) {

#ifndef BCC_BLOCK_IGNORE
		asm {
			mov tsp, sp
			mov tss, ss
			mov tbp, bp
		}
#endif
		PCB::running->sp = tsp;
		PCB::running->ss = tss;
		PCB::running->bp = tbp;

		if (PCB::running->toKill == 1)
			temp = PCB::running;

		if (PCB::running != PCB::idle && PCB::running->working != 0 && PCB::running->toKill == 0) {
//			printf("scheduler put");
			Scheduler::put(PCB::running);
		}
		
	do {
		PCB::running = Scheduler::get();
	} while (PCB::running->toKill == 1);
		//		printf("got id = %d\n",PCB::running->id);
		if (!PCB::running) {
			PCB::running = PCB::idle;
		}
		//		printf("nit %d status %d\n", PCB::running->id, PCB::running->working);
		cnt = PCB::running->timeSlice;

		tsp = PCB::running->sp;
		tss = PCB::running->ss;
		tbp = PCB::running->bp;
#ifndef BCC_BLOCK_IGNORE
		asm {
			mov sp, tsp   // restore sp
			mov ss, tss
			mov bp, tbp
		}
#endif

//		printf("trenutno %d\n", PCB::running->id);
		if (temp != 0) {
//			printf("ubijam %d\n", temp->id);
//			printf("u kontekstu : %d\n", PCB::running->id);

			while (!temp->waiting.isEmpty()) {
				PCB* tek = temp->waiting.pop();
				tek->working = 1;
//				printf("stavio id=%d\n", tek->id);
				Scheduler::put(tek);
			}
			temp->working = 0;
			delete temp->stack;
			temp->stack = 0;
			temp = 0;
//			printf("%d\n", temp);
		}

		context_swap_req = 0;

		//printf("sacu handlujem");
		if (PCB::running != PCB::idle && PCB::running->mythread != 0) {
			PCB::running->mythread->handle();
		}


} else
	context_swap_req = 1;

}
}
