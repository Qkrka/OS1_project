#ifndef LOCK_H_
#define LOCK_H_


void lock();
void unlock();

#define locki() { asm pushf; asm cli; }

#define unlocki() { asm popf; }

#endif
