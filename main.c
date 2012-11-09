#include "kernel.h"
#include "mi86/debug.h"

void test_printk(void);
void init_mes(void);


void write_byte(uint_8, uint_16, uint_16);
void init_servers(void){
	unsigned int i, j;
	int base = 0x7000;
	
	printk("	Servers initializing ....\n");


	for (i = 0; i < 16; i++){
		read_sector(129+i, base, 0);
		base += 32;
		printk(".");
		}
	printk("\nMM load complete\n");
	
	base = 0x7400;
	for (i = 0; i < 16; i++){
		read_sector(145+i, base, 0);
		base += 32;
		printc('.');
		}
	printk("\nFM load complete\n");
	

/*
	printk("  %x\n\n", base);
	for (i = 0; i < 16; i++){
		read_sector(257+i, base, 0);
		base += 32;
		}
		*/

}

int main(void){
	int i;

	/*run test_printk()*/

	printk("Welcome to MINI OS\n\n");
	init_servers();

	cli();
	i = 0;
	setup_irq(8, timer_handle);
	setup_irq(9, keyboard_int);
	setup_irq(0x80, trap_int);

	init_mes();
	init_pcbs();
	
	sti();

	while (1){
		printk("idiot");
		idle_cpu();
		}
}
