void printc(char);

void prints(char* s){
	for (;*s;s++)
		printc(*s);
}

void printd(int x){
	char c, str[30];
	int top = 0, i;
	
	if (x == 0)
		printc('0');
	else{
		if (x < 0) {
			printc('-');
			x = -x;
			}
		while(x > 0){
			str[top] = x % 10;
			x /= 10;
			top++;
		}
		for (i = top - 1; i >= 0; i--)
			printc(str[i]+'0');
	}
}

void printx(unsigned int x){
	int top = 0, i;
	char str[30];
	if (x == 0)
		printc('0');
	else while(x > 0){
		str[top] = x % 16;
		x /= 16;
		top++;
		}
	for (i = top - 1; i >= 0; i--)
		if (str[i] < 10)
			printc(str[i] + '0');
		else
			printc(str[i] - 10 + 'A');
}

void printk(char* clt, ...){
	int* arg = (int*) &clt;

	while (*clt){
		if (*clt == '%'){
			clt++;
			arg++;
			if (*clt == 'd') 
				printd(*(int*)arg);
			else if (*clt == 'c')
				printc(*(char*)arg);
			else if (*clt == 's')
				prints(*(char**)arg);
			else if (*clt == 'x')
				printx(*(unsigned int*)arg);
			clt++;
		}
		else if (*clt == '\n'){
			printc(*clt++);
			printc('\r');
		}
		else if (*clt == ' '){
			printc(' ');
			clt++;
			}
		else printc(*clt++);
	}
}	
