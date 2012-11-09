#include "../types.h"
#include "../clm.h"
#include "../../server.h"


void lseek(int fd, int offset, int whence){
	struct message m_lseek;

	m_lseek.type = LSEEK;
	m_lseek.p1 = fd;
	m_lseek.p2 = offset;
	m_lseek.p3 = whence;
	invoke(PM, &m_lseek);
}
