#include "../types.h"
#include "../clm.h"
#include "../../server.h"


void close(char* s, int type){
	struct message m_close;

	m_close.type = CLOSE;
	m_close.p1 = type;
	m_close.p2 = (int)s;
	invoke(PM, &m_close);
}
