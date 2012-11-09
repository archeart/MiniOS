#ifndef _PROC_H_
#define _PROC_H_

#include "types.h"
#define NR_PROCESS 32 
#define KSTACK_SIZE 512

typedef struct pcb{
	uint_16 ax, bx, cx, dx;
	uint_16 bp, sp, si, di;
	uint_16 cs, ds, ss, es;
	uint_16 ip, fr;
	uint_8 kernel_stack[KSTACK_SIZE];

	int_8 pid, prt, chc, ppid;
	bool occupy;
	struct pcb *pre_pcb, *next_pcb;

	int page_start, page_end;

	struct mailbox *MesQ;
	struct semaphore mail;
	uint_16 mes_off;

	};


void init_pcbs(void);

void kthread_create(void (*ptr)(void), void *stk_top, int_8 x);
int uproc_create(uint_16, uint_16, uint_16, int_8);
void kill_proc(void);
void shif_proc(void);
void pick_proc(void);
void show_proc(int);
struct pcb* find_proc(int_8);


extern uint_8 proc_run;

#endif
