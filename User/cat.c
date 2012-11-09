
char buf[512];
char info[17] = "Can't open file\n";

int main(char* args){
	int i;
	int fd = open(args, 1);

	if (fd != -1) {
		i = read(fd, buf, 512);
		while (i == 512){
			write(1, buf, 512);
			i = read(fd, buf, 512);
		}
		write(1, buf, i);
	}
	else
		write(1, buf, 16);

	close(fd);

}

