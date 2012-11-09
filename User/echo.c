

int main(char* argv){
	int j;

	j = 0; 
	while (argv[j] != 0) j++;
	write(1, argv, j);
}
