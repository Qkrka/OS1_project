#include "SysCall.h"

#include "KerSem.h"
#include "Lock.h"

unsigned oldTimerOFF, oldTimerSEG; // stara prekidna rutina

void inic(){
	asm{
		cli
		push es
		push ax

		mov ax,0   //  ; inicijalizuje rutinu za tajmer
		mov es,ax

		mov ax, word ptr es:0022h //; pamti staru rutinu
		mov word ptr oldTimerSEG, ax	
		mov ax, word ptr es:0020h	
		mov word ptr oldTimerOFF, ax	

		mov word ptr es:0022h, seg timer	 //postavlja 
		mov word ptr es:0020h, offset timer //novu rutinu

		mov ax, oldTimerSEG	 //	postavlja staru rutinu	
		mov word ptr es:0182h, ax //; na int 60h
		mov ax, oldTimerOFF
		mov word ptr es:0180h, ax

		pop ax
		pop es
		sti
	}
	PCB::idle=new PCB(500);
	PCB* mainPCB=new PCB();
	PCB::running=mainPCB;
}

void restore(){
	asm {
		cli
		push es
		push ax

		mov ax,0
		mov es,ax


		mov ax, word ptr oldTimerSEG
		mov word ptr es:0022h, ax
		mov ax, word ptr oldTimerOFF
		mov word ptr es:0020h, ax

		pop ax
		pop es
		sti
	}
}

unsigned tsp;
unsigned tss;
unsigned tbp;
volatile int cnt;
volatile int context_swap_req = 0;
extern int tick();
extern volatile int lockFlag;

#include "stdio.h"
void interrupt timer(...){	// prekidna rutina
//	printf("timer\n");
	if (!context_swap_req) {
		cnt--; 
		tick();
		KernelSem::sems.decTime();
		asm int 60h;
	}
//	printf("contswreq = %d",context_swap_req);
	if (cnt <= 0 || context_swap_req) {
		if(!lockFlag){
		asm {
			mov tsp, sp
			mov tss, ss
            mov tbp, bp
		}

		PCB::running->sp = tsp;
		PCB::running->ss = tss;
        PCB::running->bp = tbp;
        
        if (PCB::running != PCB::idle && PCB::running->working!=0){
			Scheduler::put(PCB::running);
		}
		PCB::running = Scheduler::get();
//		printf("got id = %d\n",PCB::running->id);
		if (!PCB::running) {
            PCB::running = PCB::idle;
        }
        cnt=PCB::running->timeSlice;

		tsp = PCB::running->sp;
		tss = PCB::running->ss; 
        tbp = PCB::running->bp;

		asm {
			mov sp, tsp   // restore sp
			mov ss, tss
            mov bp, tbp
		}     
		context_swap_req = 0;
	} 
	else context_swap_req=1;

}
}
