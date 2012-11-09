#include "fsinfo.h"

#define bool unsigned char
#define true 1
#define false 0

char buf[543];
char command[30], args[60];

bool strcmp(char* s1, char* s2) {

	while (*s1 != 0 && *s2 != 0) {
		if (*s1 != *s2)
			return false;
		s1++; s2++;
	}
	if (*s1 == 0 && *s2 == 0)
		return true;
	else
		return false;

}

void strcpy(char* s1, char* s2) {
	int i;
	for (i = 0; i < 61; i++){
		s1[i] = s2[i];
		if (s2[i] == 0)
			break;
		}

}

int strlen(char* s){
	int i = 0;
	while (s[i] != 0)
		 i++;
	return i;
}

void gets(char* s) {
	int i;
	for (i = 0; i < 512; i++)
		buf[i] = 0;
	read(STDIN_NO, s, 512);
}

void puts(char* s){
	int len = strlen(s);
	write(STDOUT_NO, s, len);
}

void analysis(void) {
	int i, j, len;
	len = strlen(buf);

	command[0] = 0;
	args[0] = 0;

	for (i = 0; i <= len; i++){
		if (buf[i] == ' '){
			command[i] = 0;
			i++;
			break;
		}
		else
			command[i] = buf[i];
	}

	for (j = i; j <= len; j++)
		args[j-i] = buf[j];
}
	


int main(void){
	int fd, i;
	char* s;

	while(1) {
		puts("\n#");
		gets(buf);
		analysis();

		if (strcmp(command, "ls") == true) {
			lsdir();
		} else if (strcmp(command, "mkdir") == true) {
			mkdir(args);
		} else if (strcmp(command, "cd") == true) {
			chdir(args);
		} else if (strcmp(command, "rmdir") == true) {
			rmdir(args);
		} else if (strcmp(command, "more") == true) {
			printk("args: %s\n", args);
			fd = open(args, 0);
			if (fd == -1)
				printk("file does not exist\n");
			else {
				read(fd, buf, 543);
				buf[543] = 0;
				printk("buf; %s\n", buf);

			}
			close(fd);
		} else if (strcmp(command, "echo") == true) {
			i = fork();
			if (i == 0)
				exec("/bin/echo", args);
			else
				wait_pid(i);
		} else if (strcmp(command, "./rdwr") == true) {
			i = fork();
			if (i == 0)
				exec("/bin/rdwr", " ");
			else
				wait_pid(i);
		} else if (strcmp(command, "./exec") == true) {
			i = fork();
			if (i == 0)
				exec("/bin/exec", args);
		} else if (strcmp(command, "cat") == true) {
			i = fork();
			strcpy(buf, args);
			if (i == 0) {
				/*
				analysis();
				s = &args[2];
				if (args[0] == '>') {
					fd = open(s, 1);
					dup2(fd, 1);
					exec("/bin/cat", command);
				} else 
					exec("/bin/cat", buf);
				*/
				exec("/bin/cat", args);

			} else
				wait_pid(i);
		}

		else 
			printk("no such command: %s\n", command);


	}
}
