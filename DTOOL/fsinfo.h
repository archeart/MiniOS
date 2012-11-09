#define DIR 0
#define BIN 1
#define DEV 2

typedef struct dir_entry{
	unsigned short inode_index;
	char name[30];
};

typedef struct inode{
	char name[30];
	unsigned short addr[13];
	short flags, size;
	short unused[2];
};
	


