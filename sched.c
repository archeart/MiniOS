#include "kernel.h"
#include "mi86/debug.h"

extern struct pcb pcbs[];

void maintain(){

	current_pcb->chc--;

	if (current_pcb->chc < 1)
		current_pcb->chc = current_pcb->prt;

	if (current_pcb->chc == 0)
		current_pcb = current_pcb->next_pcb;

	if (proc_run != 1){
		queue_delete(current_pcb);
		queue_autoins(head->next_pcb, current_pcb);
		}

}

void scheduler(void){

	current_pcb = head->next_pcb;
/*	
	if (current_pcb->pid == 8){
	printk(" Scheduler, current_pcb: %d\n", current_pcb->pid);
	printk(" cs:ip = [%x:%x]\n", current_pcb->cs, current_pcb->ip);
	}
	*/
	/*
	queue_visit();
	*/
	/*
	printk("  Current pid, ip : %d %x\n", current_pcb->pid, current_pcb->ip);
	*/
	
/*	
	printk("  Currentip, cs, ..: %x %x %x\n", current_pcb->pid, current_pcb->ip, current_pcb->cs);
	*/
	
}
