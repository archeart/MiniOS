#include "kernel.h"

void trap_handler(uint_16 ax, uint_16 bx, uint_16 cx, uint_16 dx){
	uint_8 opr = ax;



	if (opr == 0){
		kill_proc();
		}
	else if (opr == 1){
		pick_proc();
		}
	else if (opr == 2){
		uproc_create(bx, cx, dx, ax >> 8);
		}
	else if (opr == 3){
		ker_receive((struct message*)bx);
	}
	else if (opr == 4){
		ker_send(cx, (struct message*)bx);
		ker_receive((struct message*)bx);
	}
	else if (opr == 5){
		ker_send(cx, (struct message*)bx);
	}	
}

