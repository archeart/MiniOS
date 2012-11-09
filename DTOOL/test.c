#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>


int main(int argv, char* args[]){
	char name;
	int fd = open("hello", O_WRONLY);
	int fd2, i;
	char buf[512];
	char str[10] = "hiads";

	
	printf("buf: %s\n", buf);
	exit(0);
	
	fd2 = open(args[1], O_RDWR);
	printf("%s \n", args[1]);

	for (i = 0; i < 34; i++)
		read(fd2, buf, 1);

	buf[34] = 0;

	printf("%s \n", buf);


	
	/*
	if (strcmp(str, "hi") == 0)
		printf("eque\n");
	else
		printf("ne\n");
	write(fd, buf, 6);
	lseek(fd, 10, SEEK_CUR);
	write(fd, buf, 6);
	*/

	return 0;

	}
