#include "../types.h"
#include "../clm.h"
#include "../../server.h"

void dup2(int fd1, int fd2){
	struct message m;

	m.type = DUP2;
	m.p1 = fd1;
	m.p2 = fd2;
	invoke(FM, &m);
}


