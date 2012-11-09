#include "kernel.h"
#define mask 127

#define WFD 1
#define WFN 0


struct pcb pcbs[];

static struct message pool[50];
static int f, r;

void enqueue(struct message* m){
	pool[r].type = m->type;
	pool[r].source = m->source;
	pool[r].p1 = m->p1;
	pool[r].p2 = m->p2;
	pool[r].p3 = m->p3;
	pool[r].p4 = m->p4;
	r++;
}

void dequeue(struct message* m){
	m->type = pool[f].type;
	m->source = pool[f].source;
	m->p1 = pool[f].p1;
	m->p2 = pool[f].p2;
	m->p3 = pool[f].p3;
	m->p4 = pool[f].p4;
	f++;
}

void hangon(int, struct message*);

void ker_PM(void){
	int source, i, j, pid;
	int proc_wait[NR_PROCESS];
	uint_16 seg, Stack;
	bool state;
	struct message m;

	printk("this is PM\n");

	for (i = DAEMON; i < NR_PROCESS; i++)
		proc_wait[i] = 0;


	m.type = LOAD_PROCESS;
	m.p1 = 1;
	send(FM, &m);

	receive(&m);
	/*suppose this can always be done right */
	seg = m.p1 * 32 + 0x3000 + 2;
	Stack = (m.p2 - m.p1 + 1) * 512 - 32 - 2 ; 
	pid = uproc_create(seg, 0, Stack, 2);

	pcbs[pid].page_start = m.p1;
	pcbs[pid].page_end = m.p2;

/*	show_proc(pid);
*/

	f = 0; r = 0;
	state = WFN;

	while (1){
		if (state == WFN){
			if (f < r)
				dequeue(&m);
			else
				receive(&m);
		}
		else if (state == WFD){
			receive(&m);
			while (m.source!= MM && m.source!= FM){
				enqueue(&m);
				receive(&m);
				}
		}

		if (m.type == FORK){
			#ifdef DEBUG
			printk("%d FORK!\n", m.source);
			#endif

			for (i = DAEMON; i < NR_PROCESS; i++)
				if (pcbs[i].occupy == FALSE)
					break;

			if (i == NR_PROCESS){
				printk("CAN'T FORK!");
				/*may be kill it*/
				m.type = FORK_FAILURE;
				send(m.p4, &m);
				state = WFN;
			}
			else{

				m.type = FORK;
				m.p4 = m.source;
				m.p1 = pcbs[m.source].page_start;
				m.p2 = pcbs[m.source].page_end;
		/*		printk("page: %d %d\n", m.p1, m.p2);
				printk(" sending to MM");
		*/
				send(MM, &m);
				state = WFD;
			}

		}

		else if (m.type == FORK_DONE){
			for (i = DAEMON; i < NR_PROCESS; i++)
				if (pcbs[i].occupy == FALSE)
					break;


			pcbs[i].occupy = TRUE;

			pcbs[i].ax = pcbs[m.p4].ax;
			pcbs[i].bx = pcbs[m.p4].bx;
			pcbs[i].cx = pcbs[m.p4].cx;
			pcbs[i].dx = pcbs[m.p4].dx;

			pcbs[i].si = pcbs[m.p4].si;

			pcbs[i].es = pcbs[m.p4].es;
			pcbs[i].cs = 0x3000 + m.p1 * 32 + 2;
			pcbs[i].ds = pcbs[i].cs;
			pcbs[i].ss = pcbs[i].cs;

			pcbs[i].page_start = m.p1;
			pcbs[i].page_end = m.p2;
			pcbs[i].mes_off = pcbs[m.p4].mes_off;

			pcbs[i].sp = pcbs[m.p4].sp;
			pcbs[i].bp = pcbs[m.p4].bp;
			pcbs[i].ip = pcbs[m.p4].ip;
			pcbs[i].fr = pcbs[m.p4].fr;
			pcbs[i].pid = i;
			pcbs[i].ppid = m.p4;
			pcbs[i].prt = 2;
			pcbs[i].chc = 2;
			pcbs[i].MesQ = 0;
			
			/*block self*/
			pcbs[i].mail.value = -1;
			pcbs[i].mail.waitlist = &pcbs[i];
			pcbs[i].pre_pcb = &pcbs[i];
			pcbs[i].next_pcb = &pcbs[i];

			#ifdef DEBUG
			printk("FORK DONE !\n");
			#endif

			m.type = FORK;
			m.p1 = m.p4;
			m.p2 = i;
			hangon(FM, &m);

			pid = m.p4; 
			m.p4 = i;
			send(pid, &m);

			m.p4 = 0;
			send(i, &m);

			state = WFN;
			
		}

		else if (m.type == EXEC){
			/*
			printk("EXEC\n");
			*/

			/*m.p1 = path_off but in this lab, it's only a number*/
			/*m.p2 = arg_off this should be important*/

			/*
			printk("proc_%d EXEC %d\n", m.source, m.p1);
			*/

			i = m.p1; j = m.p2; 
			/*printk("PM, seg: %d, off: %d\n", pcbs[m.source].cs, i);
			*/

			pid = m.source;

			m.p4 = m.source;
			m.p1 = pcbs[m.source].page_start;
			m.p2 = pcbs[m.source].page_end;
			m.type = RELEASE_MEMORY;
			send(MM, &m);
			receive(&m);

			while (m.source != MM){
				enqueue(&m);
				receive(&m);
			}

			m.type = LOAD_BUF;
			m.p1 = pcbs[pid].cs;
			m.p2 = j;
			m.p3 = 48;
			send(MM, &m);
			receive(&m);
			while (m.source != MM){
				enqueue(&m);
				receive(&m);
			}
			/*
			printk("mess: (%d %d %d %d %d)\n", m.type, m.source, m.p1, m.p2, m.p3);
			*/

			m.type = EXEC;
			m.p1 = i;
			m.p2 = j;
			m.p3 = pcbs[pid].cs;
			send(FM, &m); 
			state = WFD;

		}
		else if (m.type == LOADPROC_DONE){

			i = m.p1;
			j = m.p2;

			m.type = PUSH_BUF;
			m.p1 = 0x3000 + i * 32 + 2;
			m.p2 = (j - i + 1) * 512 - 1058; /*-1024 - 32 - 2*/
			m.p3 = 48;
			send(MM, &m);
			receive(&m);
			while (m.source != MM){
				enqueue(&m);
				receive(&m);
			}
			
			m.p1 = i;
			m.p2 = j;
			i = pid;
			pcbs[i].cs = 0x3000 + m.p1 * 32 + 2;
			pcbs[i].ds = pcbs[i].cs;
			pcbs[i].ss = pcbs[i].cs;

			pcbs[i].ip = 0;
			pcbs[i].sp = (m.p2 - m.p1 + 1) * 512 - 32 - 2 ; 
			pcbs[i].fr = 0x0200;
			pcbs[i].page_start = m.p1;
			pcbs[i].page_end = m.p2;
			pcbs[i].prt = 2;
			pcbs[i].chc = 2;

			pcbs[i].MesQ = 0;
			pcbs[i].mail.value = 0;
			pcbs[i].mail.waitlist = 0;

			/*show_proc(i);
			*/
			
			/*queue_visit();*/
			queue_autoins(head->next_pcb, &pcbs[i]);
			proc_run++;

			/*printk("EXEC DONE\n");
			*/
			/*
			queue_visit();
			*/

			state = WFN;
			
		}

		else if (m.type == EXIT){
			pid = m.source;
			i = m.p1;

			#ifdef DEBUG
			printk("EXIT\n");
			printk(" to kill proc_%d\n", pid);
			#endif

			m.type = RELEASE_MEMORY;
			m.p1 = pcbs[pid].page_start;
			m.p2 = pcbs[pid].page_end;
			send(MM, &m);
			receive(&m);
			while (m.source != MM){
				enqueue(&m);
				receive(&m);
			}
			/*queue_visit();
			*/
			pcbs[pid].occupy = FALSE;

			for (i = DAEMON; i < NR_PROCESS; i++)
				if (proc_wait[i] == pid){
					send(i, &m);
					proc_wait[i] = 0;
			}

			m.type = EXIT;
			m.p1 = pid;
			send(FM, &m);
			receive(&m);
			while (m.source != FM){
				enqueue(&m);
				receive(&m);
			}

			state = WFN;

		}
		else if (m.type == WAIT_PID){
			if (pcbs[i].occupy == TRUE)
				proc_wait[m.source] = m.p1;
			else
				send(m.source, &m);

			state = WFN;
		}
		else if (m.type == SLEEP){
			m.p2 = m.source;
			m.type = DEV_WRITE;
			send(TIMER, &m);
		}
		else if (m.type == DEV_DONE && m.source == TIMER)
			send(m.p1, &m);

		else if (m.type == OPEN) {
			pid = m.source;

			m.p4 = pcbs[pid].cs;
			m.p3 = pid;

			#ifdef DEBUG
			printk("PM open \n");
			#endif

			send(FM, &m);

			receive(&m);
			while (m.type != DONE && m.source != FM){
				enqueue(&m);
				receive(&m);
			}

			#ifdef DEBUG
			printk("open over PM\n");
			#endif

			send(pid, &m);
		}
		else if (m.type == CLOSE) {
			pid = m.source;
			m.p2 = pid;
			hangon(FM, &m);
			send(pid, &m);
		}

		else if (m.type == READ) { /*p1 fd p2 off p3 length */
			pid = m.source;
			m.p1 = (m.source << 8) + m.p1;
			m.p4 = pcbs[pid].cs;

			send(FM, &m);

			receive(&m);
			while (m.type != DONE && m.source != FM){
				enqueue(&m);
				receive(&m);
			}

			send(pid, &m);
		}

		else if (m.type == WRITE) {

			pid = m.source;
			m.p1 = (m.source << 8) + m.p1;
			m.p4 = pcbs[pid].cs;

			send(FM, &m);
			receive(&m);
			while (m.type != DONE && m.source != FM){
				enqueue(&m);
				receive(&m);
			}

			send(pid, &m);
		}

		else if (m.type == LSEEK) {

			pid = m.source;
			m.p1 = (pid << 8) + m.p1;
			/*
			send(FM, &m);
			receive(&m);
			while (m.type != DONE && m.source != FM){
				enqueue(&m);
				receive(&m);
			}
			*/

			hangon(FM, &m);
			send(pid, &m);
		}

		else if (m.type == CHDIR) {
			pid = m.source;
			m.p2 = pcbs[pid].cs;
			hangon(FM, &m);
			send(pid, &m);

		}

		else if (m.type == LSDIR) {
			hangon(FM, &m);
			send(pid, &m);
		}

		else if (m.type == MKDIR) { 
			pid = m.source;
			m.p2 = pcbs[pid].cs;
			hangon(FM, &m);
			send(pid, &m);

		}

		else if (m.type == RMDIR) {
			/*printk("PM, rmdir\n");
			*/
			pid = m.source;
			m.p2 = pcbs[pid].cs;
			hangon(FM, &m);
			send(pid, &m);
		}

	}

}

void hangon(int pid, struct message* m){
	send(pid, m);
	receive(m);
	while (m->type != DONE && m->source != FM){
		enqueue(m);
		receive(m);
	}
}
	
