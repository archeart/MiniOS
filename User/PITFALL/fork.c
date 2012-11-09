#include "../types.h"
#include "../clm.h"
#include "../../server.h"

int fork(void){
	struct message m_fork;
	int retval;

	m_fork.type = FORK;
	invoke(PM, &m_fork);
	retval = m_fork.p4;
	return retval;
}


