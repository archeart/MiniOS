#include "../types.h"
#include "../../server.h"
#include "../clm.h"


void exit(int relval){
	struct message exit_m;

	exit_m.type = EXIT;
	exit_m.p1 = relval;
	invoke(PM, &exit_m);
	while (1);
}
