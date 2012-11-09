#include "../types.h"
#include "../clm.h"
#include "../../server.h"


int open(char* s, int type){
	struct message m_open;

	m_open.type = OPEN;
	m_open.p1 = type;
	m_open.p2 = (int)s;
	invoke(PM, &m_open);
	return m_open.p1;
}
