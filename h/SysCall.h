#ifndef SYSCALL_H_
#define SYSCALL_H_

#include "PCB.h"
typedef void interrupt (*pInterrupt)(...); 

void inic();
void restore();
void interrupt timer(...);


#endif
