#include "../../server.h"
#include "../types.h"
#include "../clm.h"

void sleep(int s){
	struct message m;

	m.type = SLEEP;
	m.p1 = s;

	invoke(PM, &m);
	}

