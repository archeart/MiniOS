#include "../types.h"
#include "../clm.h"
#include "../../server.h"

int dup(int fd){
	struct message m_dup;
	int val;

	m_dup.type = DUP;
	m_dup.p1 = fd;
	invoke(FM, &m_dup);
	val = m_dup.p1;
	return val;
}


