#define DIR 0
#define BIN 1
#define DEV 2

#define STDIN 601
#define STDOUT 602
#define STDERR 603

#define STDIN_NO 0
#define STDOUT_NO 1
#define STDERR_NO 2

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

typedef struct inode{
	char name[30];
	int addr[13];
	int flag, size, unused1, unused2;
};

typedef struct dir_entry{
	int index;
	char name[30];
};
