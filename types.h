#ifndef _TYPES_H_
#define _TYPES_H_

#define int_16 int
#define int_8 char
#define uint_16 unsigned int
#define uint_8 unsigned char
#define bool uint_8

#define TRUE 1
#define FALSE 0
#define NULL (void*)0


typedef struct semaphore{
	struct pcb* waitlist;
	int value;
	bool INT;
};

typedef struct message{
	int type;
	int source;
	int p1, p2, p3, p4;
};

typedef struct mailbox{
	struct message msg;
	struct mailbox* next;
	bool occupy;
};

typedef struct header{
	unsigned char a_magic[2], a_flags, a_cpu;
	unsigned char a_hdrlen, a_unused;
	unsigned short a_version;
	long a_text, a_data, a_bss, a_entry, a_total, a_syms; 
};



#endif
