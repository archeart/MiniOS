#include "kernel.h"
#include "mi86/debug.h"

#define SERVER 1

struct semaphore a, b, c, full, empty, mutex; 

extern uint_8 stack_b[], stack_c[] ;

int f, r, pro;

void test_producer1(void);
void test_producer2(void);
void test_customer1(void);
void test_customer2(void);

void pro_cus(void){
	f = 0;
	r = 0;
	pro = 0;

	sem_init(&full, 0, FALSE);
	sem_init(&empty, 100, FALSE);
	sem_init(&mutex, 1, FALSE);

	cli();
	kthread_create(test_producer1, &stack_b[254], 3);
	kthread_create(test_producer2, &stack_c[254], 3);
	sti();
}

void test_producer1(void){
	while (1){
	P(&empty);
		P(&mutex);
			pro++;
			r = (r + 1) % 100;
			
			V(&full);
		V(&mutex);
		}
}

void test_producer2(void){
	while (1){
	P(&empty);
		P(&mutex);
			pro++;
			r = (r + 1) % 100;
			
			V(&full);
		V(&mutex);
		}
}

void test_customer1(void){
	while (1){
	P(&full);
		P(&mutex);
			pro--;
			f = (f + 1) % 100;
			
			V(&empty);
		V(&mutex);
		}
}

void test_customer2(void){
	while (1){

	P(&full);
		P(&mutex);
			pro--;
			f = (f + 1) % 100;
			
			V(&empty);
		V(&mutex);
		}
}

void test_server(void){
	struct message ms;

	while (1){
		receive(&ms);
		printk("SERVER receive message (%d,%d,%d) from PID:%d", 
			ms.p1, ms.p2, ms.p3, ms.source);
		send(2, &ms);
		idle_cpu();
		}
}

void test_sender(void){
	struct message ms;

	ms.p1 = 1;
	ms.p2 = 2;
	ms.p3 = 3;
	while (1){
		printk("SENDER send message (%d,%d,%d) to SERVER\n",
			ms.p1, ms.p2, ms.p3);

		invoke(SERVER, &ms);
		ms.p3++;
		idle_cpu();
		}
}

void sem_a(void);
void sem_b(void);
void sem_c(void);
void semtest(void){
	int i;
	printk("this should be a test for semaphore\n");
	printk("wish you good luck\n");
	sem_init(&a, 1, FALSE);
	sem_init(&b, 0, FALSE);
	sem_init(&c, 0, FALSE);
	cli();
	kthread_create(sem_a, &stack_b[254], 2);
	kthread_create(sem_b, &stack_c[254], 2);
	sti();
}

void sem_a(void){
	int i;
	while (1){
	P(&a);
		printk("A");
		for (i = 0; i < 40; i++);

	V(&b);
	}
}	

void sem_b(void){
	int i;
	while (1){
	P(&b);
		printk("B");
		for (i = 0; i < 40; i++);
	V(&c);
	}
}

void sem_c(void){
	int i;
	while (1){
	P(&c);
		printk("C");
		for (i = 0; i < 40; i++);
	V(&a);
	}
}

void test1_a(void){
	int i = 10;
	char c = 'a';
	while (i--) {
		printk("why %c ", c); 
		c++;
		if (c > 'z')
			c = 'a';
		yield();
		printk("back from yield\n");
	}

	while (1) printk("this is A\n");
}

void test1_b(void){
	int i = 10;
	char c = 'A';
	while (i--) {
		printk("%c ", c); 
		c++;
		if (c > 'Z')
			c = 'A';
		yield();
		}
		while(1) printk("this is B\n");
	}


void test1_c(void){
	int i = 0;
	while (i < 1000){
		i++;
		printk("%d ", i);
		yield();
		}
		while (1) printk("this is C\n");
	}

int cnt = 0;
void thread_a(void){
	int tmp;
	while (1){
		tmp = cnt+1;
		printk("\ndata%d \n", tmp);
		cnt = tmp;
		for (tmp = 0; tmp < 20; tmp++);
		idle_cpu();
		}
}
void thread_b(void){
	int tmp;
	while(1){
		tmp = cnt+1;
		printk("\ndata%d \n", tmp);
		cnt = tmp;
		for (tmp = 0; tmp < 20; tmp++);
		idle_cpu();
	}
}

static uint_16 count = 0;
void test_thread_a(void) {
	int i, j, k, l, tmp;
	for (i = 0; i < 512; i++)
	for (j = 0; j < 512; j++)
	for (k = 0; k < 512; k++){
/*		dis_int();
*/
		tmp = count + 1;
		count = tmp;
		if (count == 0){
			printk("a");
			printk("\n");
			}

		
/*		en_int();
*/
		}
	cli();
	printk("A reported: total = %d\n", count);
	sti();
}

void test_thread_b(void){
	int i, j, k, l, tmp;
	for (i = 0; i < 512; i++)
	for (j = 0; j < 512; j++)
	for (k = 0; k < 512; k++){
/*		dis_int();
*/
		tmp = count + 1;
		count = tmp;
/*		printk("%d ", count);
*/

/*		en_int();
*/
		}
	cli();
	printk("B reported: total = %d\n", count);
	sti();
}

