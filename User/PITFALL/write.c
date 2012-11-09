#include "../types.h"
#include "../clm.h"
#include "../../server.h"


void write(int fd, char* s, int length){
	struct message m_write;

	m_write.type = WRITE;
	m_write.p1 = fd;
	m_write.p2 = (int)s;
	m_write.p3 = length;

	invoke(PM, &m_write);

	if (m_write.p3 == -1){
		m_write.p1 = (int)s;
		m_write.p2 = length;
		m_write.type = DEV_WRITE;
		invoke(TTY, &m_write);
	}

}
