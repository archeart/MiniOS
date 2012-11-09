#include "kernel.h"

void end_of_interrupt(void){
	out_byte(0x20, 0x20);
	}

void write_byte(uint_8 data, uint_16 seg, uint_16 offset){
	physics_copy(kernel_segment, (uint_16)&data, seg, offset);
}

void read_sector(int id, int seg, int offset){
	int p = (id - 1) / 18;
	int q = (id - 1) % 18;
	int head = p & 1;
	int track = p >> 1;
	int sector = q + 1;
	read_disk((track << 8) | sector, (head << 8), seg, offset);
	}

void write_sector(int id, int seg, int offset){
	int p = (id - 1) / 18;
	int q = (id - 1) % 18;
	int head = p & 1;
	int track = p >> 1;
	int sector = q + 1;
	write_disk((track << 8) | sector, (head << 8), seg, offset);
}

void setup_irq(uint_16 irq, void (*ptr)(void)){
	write_byte(((uint_16)ptr) % 256, 0, 4 * irq + 0);
	write_byte(((uint_16)ptr) / 256, 0, 4 * irq + 1);
	write_byte((kernel_segment) % 256, 0, 4 * irq + 2);
	write_byte((kernel_segment) / 256, 0, 4 * irq + 3);
}

