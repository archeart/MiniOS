#ifndef _KERNEL_H_
#define _KERNEL_H_

#include "mi86/i86.h"
#include "mi86/debug.h"
#include "proc.h"
#include "procom.h"
#include "types.h"
#include "server.h"
#include "queue.h"


/*pid of three app_driver*/
#define TTY_DRIVER 1
#define TIMER_DRIVER 2
#define FLOPPY_DRIVER 3

extern uint_16 kernel_segment;

extern struct pcb *current_pcb, *head;

void printd(int x);
void prints(char*);
void printk(char *str, ...);
void test_printk(void);

void panic(char *str);

void setup_irq(uint_16 irq, void (*ptr)(void));
void keyboard_int(void);
void timer_handle(void);
void trap_int(void);
void trap_showAdd(void);
void end_of_interrupt(void);

void maintain(void);
void scheduler(void);

void send(uint_8, struct message*);
void receive(struct message*);
void invoke(uint_8, struct message*);

void read_sector(int, int, int);
void write_sector(int, int, int);

#endif
