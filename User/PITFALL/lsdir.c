#include "../types.h"
#include "../clm.h"
#include "../../server.h"


void lsdir(void){
	struct message m;

	m.type = LSDIR;
	invoke(PM, &m);
}
