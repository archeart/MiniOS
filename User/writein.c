#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

char buf[65536];

int main(void) {
	int fd;

	fd = open("memserver", O_RDONLY);
	if (fd <= 0) {
		printf("cannot open memserver\n");
		exit(1);
		}

	read(fd, buf, 32);
	read(fd, buf, 16*512);
	close(fd);

	fd = open("/dev/fd0", O_WRONLY);
	if (fd <= 0) {
		printf(" Cannot open disk\n");
		exit(1);
		}
		
	lseek(fd, 65536, SEEK_SET);
	
	write(fd, buf, 16*512);
	close(fd);

	fd = open("fileserver", O_RDONLY);
	read(fd, buf, 32);
	read(fd, buf, 16 * 512);
	close(fd);

	fd = open("/dev/fd0", O_WRONLY);
	lseek(fd, 65536 + 16*512, SEEK_SET);
	write(fd, buf, 16*512);
	close(fd);


	fd = open("shell", O_RDONLY);
	read(fd, buf, 16 * 512);
	close(fd);
	
	fd = open("/dev/fd0", O_WRONLY);
	lseek(fd, 512 * 160, SEEK_SET);
	write(fd, buf, 16*512);
	close(fd);

	fd = open("proc2", O_RDONLY);
	read(fd, buf, 16*512);
	close(fd);
	fd = open("/dev/fd0", O_WRONLY);
	lseek(fd, 512 * 176, SEEK_SET);
	write(fd, buf, 16*512);
	close(fd);

	printf("Done\n");
	exit(0);
	}
