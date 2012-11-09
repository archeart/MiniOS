#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

	char buffer[65536];
int main(void){
	int input = open("kernel", O_RDONLY);
	int output = open("/dev/fd0", O_WRONLY);

	char ch;
	int i = 0;

	memset(buffer, 0, sizeof(buffer));

	read(input, buffer, 32);
	read(input, buffer, 65536 - 512);
	close(input);

	lseek(output, 512, 0);
	write(output, buffer, 65536 - 512);
	close(output);
	}
