
#include "../../server.h"
#include "../types.h"
#include "../clm.h"


int exec(char* path, char* args){
	struct message exec_m;
	exec_m.type = EXEC;
	exec_m.p1 = (int)path;
	exec_m.p2 = (int)args;
	invoke(PM, &exec_m);
}
	
	
