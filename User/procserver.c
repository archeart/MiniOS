

#include "types.h"


int main(void){
	struct message m;

	m.type = LOAD_PROCESS;
	m.p1 = 1;
	invoke(FM, &m);

	invoke(KER_PM, &m);

	while (1){
		receive(&m);
		if (m.type == LOAD_PROCESS){
			invoke(FM, &m);
		}
		/*
		else if (m.type == DO_FORK)

		TO BE CONTINUE
		*/

	}
}

