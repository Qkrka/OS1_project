/*
 * main.cpp
 *
 *  Created on: Jun 29, 2019
 *      Author: OS1
 */

#include "SysCall.h"
#include "PCB.h"

extern int userMain(int argc, char* argv[]);

int main(int argc, char* argv[]){
	inic();
	userMain(argc, argv);
	restore();
}
