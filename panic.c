#include "kernel.h"

void panic(char *str){
	cli();
	printk("\n\n**kernel panic: %s **\n", str);
	while (1){
		idle_cpu();
		}
}
