#include "mi86/i86.h"
#include "mi86/debug.h"

void printk(char *clt, ...);


void test_printk(void){
	call_add();
	showCurAdd();
	printk("Here begins test for printk.\n");
	showCurAdd();
	printk("	Numbers: %d, %d, %d, %d, %x.\n", 0, 42, 2011, -1024, 0xABCD);
	showCurAdd();
	printk("	Strings: \"%s\", \"s\"\n", "hello", "world");
	}
