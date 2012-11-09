#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

char buf[65536];

int main(int argv, char* args[]) {
	int fd;
	int sector, i;
	char *s;

	/*args[1] wenjianming
	args[2] sector
	*/

	fd = open(args[1], O_RDONLY);
	read(fd, buf, 16*512); 
	if (fd <= 0){
		printf(" CAN'T OPEN FILE %s\n");
		exit(1);
		}

	sector = 0;
	s = args[2];
	while (*s){
		sector = sector * 10 + *s - '0';
		s++;
	}
	sector--;
	close(fd);
	
	fd = open("/dev/fd0", O_WRONLY);
	lseek(fd, 512 * sector, SEEK_SET);
	write(fd, buf, 16*512);
	close(fd);

	printf("Done\n");
	exit(0);
	}
