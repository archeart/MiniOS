#include "proc.h"
void printk(char*,...);
void printc(char);
void printd(int);
extern struct pcb* head;

void queue_insert(struct pcb* current, struct pcb* newnode){
	newnode->pre_pcb = current->pre_pcb;	
	newnode->next_pcb = current;
	current->pre_pcb->next_pcb = newnode;
	current->pre_pcb = newnode;
}

void queue_autoins(struct pcb* current, struct pcb* newnode){

	while (current->chc >= newnode->chc)
		current = current->next_pcb;
	/* insert in front of */
	newnode->next_pcb = current;
	newnode->pre_pcb = current->pre_pcb;
	current->pre_pcb = newnode;
	newnode->pre_pcb->next_pcb = newnode;

}

void queue_delete(struct pcb* current){

	current->pre_pcb->next_pcb = current->next_pcb;
	current->next_pcb->pre_pcb = current->pre_pcb;
}


void queue_visit(void){
	struct pcb* s = head->next_pcb;

	printk("    queue_visit:\n");
	while (s != head){
		show_proc(s->pid);
		s = s->next_pcb;
		}
	show_proc(s->pid);
/*	printk("    head pid:%d; head chc:%d; head prt: %d\n", head->pid, head->chc, head->prt);
*/
}
