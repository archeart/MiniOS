
#include "../types.h"
#include "../clm.h"
#include "../../server.h"

void wait_pid(int pid){
	struct message wait_m;

	wait_m.type = WAIT_PID;
	wait_m.p1 = pid;
	invoke(PM, &wait_m);
}


