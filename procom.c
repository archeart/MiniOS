#include "kernel.h"
#include "mi86/debug.h"
#define MS_CAP 50
#define MESSAGE_SIZE 12

void idle_cpu(void);
struct pcb* find_proc(int_8);
void pick_proc(void);
extern struct pcb* current_pcb;
struct mailbox mailbuf[50];


void sem_init(struct semaphore* sem, int x, bool y){
	sem->waitlist = NULL;
	sem->value = x;
	sem->INT = y;
}

void P(struct semaphore* sig){

	if (!sig->INT)
		cli();

	sig->value--;
	if (sig->value < 0){
		queue_delete(current_pcb);
		if (sig->value == -1){
			current_pcb->pre_pcb = current_pcb;
			current_pcb->next_pcb = current_pcb;
			sig->waitlist = current_pcb;
		}
		else
			queue_insert(sig->waitlist, current_pcb);	/*wait*/
		proc_run--;
		if (sig->INT)
			pick_proc();
		else 
			yield();
	}

	if (!sig->INT)
		sti();
}


void V(struct semaphore* sig){
	struct pcb* rel;


	if (!sig->INT)
		cli();

	sig->value++;
	/*
	printk("sig->value: %d\n", sig->value);
	*/

	if (sig->value <= 0){
		rel = sig->waitlist;
		sig->waitlist = rel->next_pcb;
		queue_delete(rel);

		queue_autoins(head->next_pcb, rel);	 /* wake up */
		if (sig->value == 0)
			sig->waitlist = NULL;
		proc_run++;
	}

	if (!sig->INT)
		sti();

}

void init_mes(void){
	int i;
	for (i = 0; i < MS_CAP; i++){
		mailbuf[i].next = NULL;
		mailbuf[i].occupy = FALSE;
		}
}

void ker_send(uint_8 pid, struct message* sms){
	struct pcb* s;
	uint_16 s1, s2;
	int i, sp;

	s = find_proc(pid);
	/*
	printk("    IN SEND, dst:%d", pid);
	*/

	for (i = 0; i < MS_CAP; i++)
		if (mailbuf[i].occupy == FALSE) 
		/* make sure it will not be filled*/
			break;
	sp = i;
	
	mailbuf[i].occupy = TRUE;

	s1 = (uint_16)sms;
	s2 = (uint_16)&mailbuf[i].msg;

	
	for (i = 0; i < MESSAGE_SIZE; i++)
		physics_copy(current_pcb->cs, s1+i, kernel_segment, s2+i);
		
	mailbuf[sp].msg.source = current_pcb->pid;

	/*
	if (current_pcb->pid == 7 || pid == 7) {
	printk(" src: %d; dst: %d", current_pcb->pid, pid);
	printk(" msg:(%d %d)\n", mailbuf[sp].msg.type, mailbuf[sp].msg.p1);
	}
	*/

	if (s->mail.value < 0){
		for (i = 0; i < MESSAGE_SIZE; i++)
			physics_copy(kernel_segment, s2+i, s->cs, s->mes_off+i);

		mailbuf[sp].occupy = FALSE;
	}
	else {
		mailbuf[sp].next = s->MesQ;
		s->MesQ = &mailbuf[sp];
	}

	/*
	if (current_pcb->pid == 7 || pid == 7) 
		printk("V\n");
	*/
	V(&s->mail);

}

void ker_receive(struct message* sms){
	int i;
	uint_8 *s1, *s2;


	if (current_pcb->MesQ == NULL)
		current_pcb->mes_off = (uint_16)sms;

	P(&current_pcb->mail);

	s1 = (uint_8*)sms;
	s2 = (uint_8*)(current_pcb->MesQ);
	
	for (i = 0; i < MESSAGE_SIZE; i++, s1++, s2++)
		physics_copy(kernel_segment, (uint_16)s2, current_pcb->cs, (uint_16)s1);
		

	current_pcb->MesQ->occupy = FALSE;
	current_pcb->MesQ = current_pcb->MesQ->next;
	
}

void ker_int_send(uint_8 pid, struct message* sms){
	struct pcb* s;
	uint_16 s1, s2;
	int i, sp;

	s = find_proc(pid);

	for (i = 0; i < MS_CAP; i++)
		if (mailbuf[i].occupy == FALSE) 
		/* make sure it will not be filled*/
			break;
	sp = i;
	
	mailbuf[i].occupy = TRUE;

	s1 = (uint_16)sms;
	s2 = (uint_16)&mailbuf[i].msg;

	
	for (i = 0; i < MESSAGE_SIZE; i++)
		physics_copy(kernel_segment, s1+i, kernel_segment, s2+i);
		
	mailbuf[sp].msg.source = current_pcb->pid;

	if (s->mail.value < 0){
		for (i = 0; i < MESSAGE_SIZE; i++)
			physics_copy(kernel_segment, s2+i, s->cs, s->mes_off+i);

		mailbuf[sp].occupy = FALSE;
	}
	else {
		mailbuf[sp].next = s->MesQ;
		s->MesQ = &mailbuf[sp];
	}

	V(&s->mail);

}

