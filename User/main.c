#include "../types.h"


#define SERVER 2

uint_8 stack1[256], stack2[256], stack3[256];


void printk(char*, ...);
void server(void);
void normal_process1(void);
void normal_process2(void);
void create_proc(uint_16, uint_16, uint_16, uint_8);
void exit(void);
void send(uint_8, struct message*);
void invoke(uint_8, struct message*);
void receive(struct message*);
void idle_cpu(void);
void cli(void);
void sti(void);



void main(void){
	create_proc(0x7000, (uint_16)server, (uint_16)&stack1[254], 3);
	create_proc(0x7000, (uint_16)normal_process1, (uint_16)&stack2[254],2);
	create_proc(0x7000, (uint_16)normal_process2, (uint_16)&stack3[254],2);

	exit();
}

void server(void){
	struct message mes;
	while (1){
		receive(&mes);
		cli();	
		printk("SERVER receive message(%d,%d,%d) from %d\n",
			mes.p1, mes.p2, mes.p3, mes.source);
		sti();

		send(mes.source, &mes);
	}
}

void normal_process1(void){
	struct message mes;
	mes.type = 1;
	mes.p1 = 1;
	mes.p2 = 2;
	mes.p3 = 3;
	while (1){
		cli();
			printk("User1\n");
		sti();
		invoke(SERVER, &mes); 
		mes.p3++;
		}
}

void normal_process2(void){
	struct message mes;
	mes.type = 1;
	mes.p1 = 2;
	mes.p2 = 1;
	mes.p3 = 3;
	while (1){
		cli();
			printk("User2\n");
		sti();
		invoke(SERVER, &mes); 
		sti();
		mes.p3++;
		}
	}
