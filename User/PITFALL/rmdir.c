#include "../types.h"
#include "../clm.h"
#include "../../server.h"


void rmdir(char* s){
	struct message m;

	m.type = RMDIR;
	m.p1 = (int)s;
	invoke(PM, &m);
}
