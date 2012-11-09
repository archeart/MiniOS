#include "../types.h"
#include "../clm.h"
#include "../../server.h"


void chdir(char* s){
	struct message m;

	m.type = CHDIR;
	m.p1 = (int)s;
	invoke(PM, &m);
}
