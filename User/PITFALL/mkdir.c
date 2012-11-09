#include "../types.h"
#include "../clm.h"
#include "../../server.h"


void mkdir(char* s){
	struct message m;

	m.type = MKDIR;
	m.p1 = (int)s;
	invoke(PM, &m);
}
