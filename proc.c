#include "kernel.h"
#include "queue.h"
#include "proc.h"
#include "mi86/debug.h"

struct pcb pcbs[NR_PROCESS+1];
struct pcb *current_pcb = NULL, *head = 0;

void test_thread_a(void);
void test_thread_b(void);
void thread_a(void);
void thread_b(void);
void test1_a(void);
void test1_b(void);
void test1_c(void);
void semtest(void);
void test_server(void);
void test_sender(void);
void tty_driver(void);
void timer_driver(void);
void floppy_driver(void);
void ker_PM(void);

uint_8 stack_idle[256];
uint_8 stack_b[256], stack_a[256], stack_b[256], stack_c[256];
uint_8 stack_driver1[256], stack_driver2[256], stack_driver3[256];
uint_8 proc_run = 0;


void idle(void){
	while (1) idle_cpu();
	}


void init_pcbs(void){
	int i;

	for (i = 0; i < NR_PROCESS; i++){
		pcbs[i].occupy = FALSE;
		pcbs[i].pid = i;		/*this may need some revise*/
		pcbs[i].MesQ = NULL;
		sem_init(&pcbs[i].mail, 0, TRUE);
		}

	head = &pcbs[0];
	head->next_pcb = head;
	head->pre_pcb = head;
	kthread_create(idle, &stack_idle[254], 0);
/*	
	kthread_create(semtest, &stack_a[254], 1);
*/	

	kthread_create(tty_driver, &stack_driver1[254], 3);
	kthread_create(timer_driver, &stack_driver2[254], 3);
	kthread_create(floppy_driver, &stack_driver3[254], 3);
	uproc_create(0x7000, 0, 0x3FFE, 3);	
	uproc_create(0x7400, 0, 0x3FFE, 3);
	kthread_create(ker_PM, &stack_a[254], 3);
	/*
	uproc_create(0x7402, 0, 0x1FFE, 3);
	*/

	
	current_pcb = &pcbs[15];
	pick_proc();

}

void kthread_create(void (*ptr)(void), void *stk_top, int_8 priority){
	int i, *p;
	struct pcb* s = 0;

	proc_run++;

	for (i = 0; i < NR_PROCESS; i++)
		if (pcbs[i].occupy == FALSE){
			s = &pcbs[i];
			pcbs[i].occupy = TRUE;
			break;
		}

	if (s == 0){
		printk("The processes space is full\n");
		return;
	}

	s->ip = (uint_16)ptr;
	s->cs = kernel_segment;
	s->ds = s->cs;
	s->fr = 0x0200;
	s->ss = kernel_segment;
	s->sp = (uint_16)stk_top;
	s->prt = priority;
	s->chc = s->prt;

	p = stk_top;
	*p = (int)kill_proc;

	if (priority != 0)
		queue_autoins(head->next_pcb, s);
}


int uproc_create(uint_16 segment, uint_16 startip, uint_16 stack, int_8 priority){
	int i, *p;
	struct pcb* s = 0;

	proc_run++;

	for (i = 0; i < NR_PROCESS; i++)
		if (pcbs[i].occupy == FALSE){
			s = &pcbs[i];
			pcbs[i].occupy = TRUE;
			break;
		}

	if (s == NULL){
		printk("The processes space is full\n");
		return 0;
	}

	s->ip = startip;
	s->cs = segment;
	s->ds = s->cs;
	s->fr = 0x0200;
	s->ss = s->ds;
	s->sp = stack;
	s->prt = priority;
	s->chc = s->prt;
	/*
	printk("cs, ds, ss, ip, sp: %x %x %x %x %x\n", \
		s->cs, s->ds, s->ss, s->ip, s->sp);
		*/
	queue_autoins(head->next_pcb, s);
	return i;
}


void kill_proc(void){
	proc_run--;
	queue_delete(current_pcb);
	current_pcb->occupy = FALSE;
	printk("The Process %d has been killed\n", current_pcb->pid);
	pick_proc();
	while (1) idle_cpu();
}

struct pcb* find_proc(int_8 pid){
	return &pcbs[pid];
}

void ps(void){
	int i;
	for (i = 0; i < NR_PROCESS; i++)
		if (pcbs[i].occupy)
			printk("pid:%d , state:%c\n");
}

void show_proc(int i){
	printk("The proc_%d: cs=%x, ip=%x, sp=%x, page_st=%d \
	page_end=%d chc=%d prt=%d\n", i, pcbs[i].cs, pcbs[i].ip, pcbs[i].sp, 
	pcbs[i].page_start, pcbs[i].page_end, pcbs[i].chc, pcbs[i].prt);
	}
