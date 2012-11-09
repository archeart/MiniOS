#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

#include "types.h"

int disk,fd, fd2;
char buf[1024];

int main(void){
	struct header* exec;
	disk = open("/dev/fd0", O_RDONLY);
	fd = open("proc2", O_RDONLY);
	fd2 = open("sh_copy", O_WRONLY);

	read(fd, buf, 512);

	lseek(disk, 512*520, SEEK_SET);
	read(disk, buf, 512);
	exec = (struct header*) buf;
	printf("%d %d %d\n", exec->a_text, exec->a_data, exec->a_bss);
	write(fd2, buf, 512);
	lseek(disk, 512*519, SEEK_SET);
	read(disk, buf, 512);
	write(fd2, buf, 512);
	lseek(disk, 512*518, SEEK_SET);
	read(disk, buf, 512);
	write(fd2, buf, 512);
	lseek(disk, 512*517, SEEK_SET);
	read(disk, buf, 512);
	write(fd2, buf, 512);


	close(fd);

}
	



