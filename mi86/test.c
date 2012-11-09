#include <stdio.h>

int main(void){
	unsigned char a;
	int b = 0x8000;
	a = b >> 8;
	printf("%x\n", a);
	}
