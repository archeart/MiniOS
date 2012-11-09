#include "../types.h"
#include "../clm.h"
#include "../../server.h"


int read(int fd, char* s, int length){
	struct message m_read;

	m_read.type = READ;
	m_read.p1 = fd;
	m_read.p2 = (int)s;
	m_read.p3 = length;

	invoke(PM, &m_read);

	if (m_read.p3 == -1){
		m_read.p1 = (int)s;
		m_read.p2 = length;
		m_read.type = DEV_READ;
		invoke(TTY, &m_read);
		return m_read.p1;
	}
	else return m_read.p3+1;

}
