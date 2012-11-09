#include "kernel.h" 

#define FLAG_EXT 0x0100
#define FLAG_BREAK 0x0080
#define ESC (FLAG_EXT + 0x01)
#define TAB (FLAG_EXT + 0x02)
#define ENTER 0x1C 
#define BACKSPACE '\b'
#define SHIFT_L (FLAG_EXT + 0x08)
#define SHIFT_R (FLAG_EXT + 0x09)
#define CTRL_L (FLAG_EXT + 0x0A)
#define CTRL_R (FLAG_EXT + 0x0B)
#define ALT_L (FLAG_EXT + 0x0C)
#define ALT_R (FLAG_EXT + 0x0D)
#define UP 0x48
#define DOWN 0x50
#define LEFT 0x4B
#define RIGHT 0x4D
#define F1 0x3B
#define F2 0x3C
#define F3 0x3D
#define F4 0x3E
#define F5 0x3F
#define F6 0x40
#define F7 0x41
#define F8 0x42
#define F9 0x43
#define F10 0x44
#define SPACE 0x39 


unsigned int keymap[83] = {0, ESC, '1', '2', '3', '4', '5', '6', '7', '8',
				'9', '0', '-', '=', BACKSPACE, TAB, 'q', 'w', 'e',
				'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']',
				ENTER, CTRL_L, 'a', 's', 'd', 'f', 'g', 'h', 'j',
				'k', 'l', ';', 111, 111, SHIFT_L, 111, 'z', 'x',
				'c', 'v', 'b', 'n', 'm', ',', '.', '/', SHIFT_R,
				'*', ALT_L, 111, 111, F1, F2, F3, F4, F5, F6, F7,
				F8, F9, F10, 111, 111, 111, UP, 111, 111, LEFT, 
				111, RIGHT, 111, 111, DOWN, 111, 111};

unsigned int keybuffer[20];

void keyboard_handle(void){
	unsigned int code = in_byte(0x60);
	unsigned int val = in_byte(0x61);
	int a = 10;
	unsigned int tmp;

	out_byte(0x61, val | 0x80);
	out_byte(0x61, val);
	out_byte(0x20, 0x20);



	if (code == SPACE)
		printc(' ');
	else if (code < FLAG_BREAK){
		tmp = code;
		if (tmp == ENTER)
			printk("\n");
		else 
			printk("%c", keymap[tmp]);
	}
}
