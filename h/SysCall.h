#ifndef _SYSCALL_H_
#define _SYSCALL_H_

#include "PCB.h"
typedef void interrupt (*pInterrupt)(...); 

void inic();
void restore();
void interrupt timer(...);


#endif
