#define SEEK_SET 0

char buf1[512] = "hello world.\n";
char buf2[20] = "operating system.\n";

int main(void){
	int i, j, pid;
	int fd1, fd2;

	fd2 = open("/he", 1);
	fd1 = open("/he", 2);

	write(fd2, buf1, 13);
	i = read(fd1, buf2, 14);
	printk("read %d bytes\n", i);
	write(1, buf2, 13);

	close(fd2);
	close(fd1);
	exit(0);
/*
	printk("shit !\n");

	pid = 7;
	j = fork();
	if (j != 0)
		pid = j;

	j = fork();
	if (j != 0)
		pid = j;

	if (pid == 7){
		printk("proc7\n");
		idle_cpu();
		idle_cpu();
		sleep(15);
	 	exec(2, "-rt");

		printk("proc7\n");
		exit(0);
	 	}
	else if (pid == 8){
		sleep(5);
		printk("proc8\n");
		exit(0);
		}
	else if (pid == 9){
		wait_pid(8);
		exit(0);
		}
	else if (pid == 10){
		sleep(5);
		printk("proc10\n");
		exit(0);
	}

	while (1) printk("proc_%d ", pid);
	
	*/
/*	
	if (pid == 7)
		exec(2, "-r -t");
		*/

	
	exit(0);
}

	/*
	int i, a, j, pid;
	int c;
	printk("hello world!\n");

	pid = 7;

	printk("&i = %x\n\n\n", &i);

	j = fork();

	if (j == 0);
	else {
		for (a = 0; a < 10; a++){
			printk("Original8 ");
			idle_cpu();
			}
		return;

	}

	wait_pid(8);
		

	while (1){
		printk("Original_%d ", pid);
		idle_cpu();
		}
}
	*/
