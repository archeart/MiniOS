#ifndef _I86_H_	
#define _I86_H_


void printc(char str);

unsigned char in_byte(unsigned int port);
void out_byte(unsigned int port, unsigned char data);
void read_disk(int, int, int, int);
void write_disk(int, int, int, int);

void cli(void);
void sti(void);
void idle_cpu(void);
void exit(void);
void yield(void);

void physics_copy(unsigned int src_segment,
				 unsigned int src_offset,
				 unsigned int dest_segment,
				 unsigned int dest_offset);

#endif
