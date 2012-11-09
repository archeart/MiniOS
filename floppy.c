/* The floopy driver is simple.
   It accepts DEV_READ and DEV_WRITE message:
     p1: sector id
     p2: the buffer address (in their user space)
   for READ request, floopy driver read that block and fill the buffer,
   for WRITE request, floppy driver write the buffer to the specified block */

#include "kernel.h"
/* let this be PID of floopy_driver */
static struct message m;

struct pcb *current_pcb;

void init_servers(void);
void floppy_driver(void) {
	struct pcb *p;


	while (1) {
		receive((struct message*)&m);
		if (m.type == DEV_READ) {
			/* rather simple and understandable */
			p = find_proc(m.source);
			read_sector(m.p1, p->ds, m.p2);
			m.type = DEV_DONE;
			send(m.source, (struct message*)&m);
		} else if (m.type == DEV_WRITE) {
			p = find_proc(m.source);
			write_sector(m.p1, p->ds, m.p2);
			/* TODO: write_sector is almost exactly same as
				the read_sector, only change is the
				AH register of int 0x13 is 0x03.
				you must write this. */
			m.type = DEV_DONE;
			send(m.source, (struct message*)&m);
		}
	}
}

