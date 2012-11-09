#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include "fsinfo.h"
/*
#define DEBUG
*/

#define format "format"
#define chdir "chdir"
#define mkdir "mkdir"
#define copy "copy"
#define dir "dir"

int link_block;
int disk;

char buf[6144];
char name[30];
	short block;
	struct inode root, bin, newfile, *node;
	short* tmp;
	char* base;
	struct dir_entry* list;
	long length = 0;
	short index, newind, i, j, k, sfd;
/*
int find_index(char* s, bool find){

	char name[30];
	struct inode node[8], node0;
	struct dir_entry list[16];
	int i, j, index;
	bool flag;

	int fd = open("/dev/fd0", O_RDONLY);
	i = 0;

	index = 0;
	if (*s == 0){
		return index;
		find = true;
	}

	while (true){
		s ++;
		if (*s != '/' && *s != 0)
			name[i++] = *s;
		else{
			name[i] = 0;
			i = 0;

			block = index / 8 + 194;
			blind = index % 8;

			lseek(fd, block*512, SEEK_SET);
			read(fd, node, 512);

			block = (node[blind].size + 511) / 512;
			entries = node[blind].size / 32;

			flag = false;

			for ( j = 0; j < block; j++ ) { 
				lseek(fd, node[blind].addr[j] * 512 - 512, SEEK_CUR);
				read(fd, list, 512);

				for (k = 0; k < 16; k++) {
					if (strcmp(name, list[k].name)) {
						index = list[k].inode_index;
						flag = true;
						break;
					}
					entries --;
					if (entries == 0)
						break;
				}

				if (flag) break;
			}

			if (!flag || *s == 0) break;
		}
	}

	find = flag;
	
	if (*s == 0)
		return index;
	else
		return -1;
}
*/

void do_format(void){
	short sbuf[512];
	int i, j;
	short posit = 261;
	short freeblock = 271;
	struct inode node;
	struct dir_entry list[2];
	short tmp;

	int fp = open("/dev/fd0", O_WRONLY);
	lseek(fp, 512 * 192, SEEK_SET);

	sbuf[0] = 511;
	for (i = 1; i < 512; i++)
		sbuf[i] = i;

	write(fp, sbuf, 1024);

	lseek(fp, 512 * 66, SEEK_CUR);
	/*block*/

	for (i = 0; i < 10; i++){
		sbuf[0] = 255;

		for (j = 1; j < 255; j++){
			sbuf[j] = freeblock;
			freeblock ++;
		}
		
		if (i != 9)
			sbuf[255] = posit + 1;
		else
			sbuf[255] = 0;

		write(fp, sbuf, 512);
		posit ++;
	}



	node.size = 64;
	node.addr[0] = 260;
	strcpy(node.name, "/");
	node.flags = DIR << 8 + 0;


	lseek(fp, 512 * 194, SEEK_SET);
	write(fp, &node, 512);

	lseek(fp, 512 * 258, SEEK_SET);
	sbuf[0] = 261;
	write(fp, sbuf, 512);

	strcpy( list[0].name, ".");
	list[0].inode_index = 0;
	strcpy( list[1].name, "..");
	list[1].inode_index = 1;
	write(fp, list, 512);


	close(fp);

}


void do_mkdir(char *s){
/*
	int fd = open("/dev/fd0", O_RDONLY);
	int fd2 = open("/dev/fd0", O_WRONLY);
	int inode_no;
	char* name[30];
	bool find;

	struct dir_entry list[16];
	unsigned short sbuf[512];

	inode_no = find_index(s, find);

	if (inode_no == -1)
		printf("Not a valid path\n");
	else if (find == 1)
		printf("file exits\n");
	else{
		i = 0;
		while (*s != 0){
			if (*s == '\')
				i = 0;
			else	
				name[i++] = *s;
			s++;
		}
		name[i] = 0;

		block = 194 + inode_no / 8;
		blind = inode_no % 8;
		lseed(fd, block * 512, SEEK_SET);
		read(fd, node, 512);

		node[blind].size += 32;
		if ((node[blind].size - 32) % 512 == 0)
	*/
}

void do_chdir(char *s){
}

void free_block(void){
	char buf[512];
	int fd = open("/dev/fd0", O_RDONLY);
	short* tmp;
	int i;

	lseek(fd, 512*260, SEEK_SET);
	read(fd, buf, 512);

	tmp = (short*) buf;
	for (i = 0; i < 11; i++)
		printf("tmp[%d]: %d\t", i, tmp[i]);

	
	printf("\n");
}

void check_block(int index){
	char buf[512];

	int fd = open("/dev/fd0", O_RDONLY);
	lseek(fd, index*512 - 512, SEEK_SET);
	read(fd, buf, 512);
	buf[20] = 0;
	printf("%s\n", buf);
}



void do_dir(char *s){
	int fd = open("/dev/fd0", O_RDONLY);
	int block, i, j, entries;
	struct inode node;
	struct dir_entry list[16];

	char name[30];

	lseek(fd, 512 * 194, SEEK_SET);
	read(fd, &node, 64);

	block = (node.size + 511) / 512;
	entries = node.size / 32;

	#ifdef DEBUG
		printf("size:%d block:%d\n", node.size, node.addr[0]);
	#endif

	printf("%d files in total.\n", entries);
	for (i = 0; i < block; i++){
		lseek(fd, 512 * node.addr[i] - 512, SEEK_SET);
		read(fd, list, 512);

		j = 0;
		while (entries > 0){
			printf("%s\t %d\n", list[j].name, list[j].inode_index);
			j ++;
			if (j == 16) break;
			entries --;
		}

	}
	printf("\n");
}

int info(char *s){
	int inodeb, inodex, index;
	int fd;
	char buf[512];
	struct inode* nodep;

	index = 0;
	while (*s != 0){
		index = index * 10 + *s - '0';
		s++;
		}
	printf("index : %d\n", index);

	inodeb = index / 8 + 194;
	inodex = index % 8;

	fd = open("/dev/fd0", O_RDONLY);
	lseek(fd, inodeb*512, SEEK_SET);

	read(fd, buf, 512);
	nodep = (struct inode*) buf;
	printf("name: %s, size: %d \n", nodep[inodex].name, nodep[inodex].size);

}

short need_block(void){
	short tmp[255];
	short val;
	int i;

	printf("link_block: %d\n", link_block);
	lseek(disk, link_block*512 - 512, SEEK_SET);
	read(disk, tmp, 512);

	if (tmp[0] == 1){
		val = link_block;
		link_block = tmp[255];
		lseek(disk, 258*512, link_block);
		tmp[0] = link_block;
		write(disk, tmp, 2);
	} else {
		tmp[0] --;
		val = tmp[tmp[0]];
		lseek(disk, link_block*512 - 512, SEEK_SET);
		write(disk, tmp, 512);
	}

	return val;
}

void load_inode(int index, struct inode* node){
	short inodeb, inodex, i;
	struct inode* nodep;

	inodeb = index / 8 + 194;
	inodex = index % 8;

	lseek(disk, inodeb*512, SEEK_SET);
	read(disk, nodep, 512);

	for (i = 0; i < 30; i++)
		node->name[i] = nodep[inodex].name[i];
	node->flags = nodep[inodex].flags;
	node->size = nodep[inodex].size;
	for (i = 0; i < 13; i++)
		node->addr[i] = nodep[inodex].addr[i];
	node->unused[0] = nodep[inodex].unused[0];
	node->unused[1] = nodep[inodex].unused[1];
}

void write_inode(int index, struct inode* node) {
	short inodeb, inodex, i;
	struct inode* nodep;

	inodeb = index / 8 + 194;
	inodex = index % 8;

	lseek(disk, inodeb*512, SEEK_SET);
	read(disk, nodep, 512);

	for (i = 0; i < 30; i++)
		nodep[inodex].name[i] = node->name[i];
	nodep[inodex].flags = node->flags;
	nodep[inodex].size = node->size;
	for (i = 0; i < 13; i++)
		nodep[inodex].addr[i] = node->addr[i];
	nodep[inodex].unused[0] = node->unused[0];
	nodep[inodex].unused[1] = node->unused[1];

	lseek(disk, inodeb*512, SEEK_SET);
	write(disk, nodep, 512);
	
}

void do_copy(char* src, char* dst, char* size) {

	link_block = 261;

	for (i = 0; i < 30; i++)
		name[i] = dst[i];

	for (i = 0; i < 30; i++)
		if (size[i] == 0)
			break;
		else
			length = length * 10 + size[i] - '0';

	for (i = 0; i < 30; i++)
		newfile.name[i] = name[i];
	printf("name: %s %s\n", name, newfile.name);
	newfile.size = length;
	newfile.flags = DIR << 8;
	sfd = open(src, O_RDONLY);
	if (sfd == -1) {
		printf("can't open\n");
		exit(1) ;
	}
	read(sfd, buf, length);

	base = buf;
	printf("base1: %s\n" , base);
	for (i = 0; i <= (newfile.size-1) / 512; i++) {
		newfile.addr[i] = need_block();
		printf("newfile.addr[%d]: %d\n", i, newfile.addr[i]);

		lseek(disk, newfile.addr[i]*512 - 512, SEEK_SET);
		write(disk, base, 512);
		base += 512;
		if (i == 0) printf("base:%s\n", base);
	}
	printf("link_block: %d\n", link_block);

	index = 0;
	lseek(disk, 194 * 512, SEEK_SET);
	read(disk, &root, 64);
	lseek(disk, root.addr[0]*512 - 512, SEEK_SET);

	read(disk, buf, 512);
	list = (struct dir_entry*) buf;
	for (i = 0; i < 16; i++) {
		if (strcmp(list[i].name, "bin") == 0)
			break;
		else printf("%s \n", list[i].name);
	}


	index = list[i].inode_index;
	load_inode(index, &bin);
	if (bin.size % 512 == 0)
		bin.addr[bin.size/512] = need_block();
	bin.size += 32;
	write_inode(index, &bin);

	lseek(disk, 192*512, SEEK_SET);
	read(disk, buf, 1024);
	tmp = (short*) buf;
	newind = tmp[tmp[0]];
	printf("newind: %d\n", newind);
	tmp[0] --;
	lseek(disk, 192*512, SEEK_SET);
	write(disk, buf, 512);

	i = (bin.size - 1) / 512;
	i = bin.addr[i] - 1;
	j = bin.size / 32;
	j = (j - 1) % 16;
	printf(" block: %d; list[%d]\n", i, j);

	lseek(disk, i*512, SEEK_SET);
	read(disk, buf, 512);
	list = (struct dir_entry*) buf;
	printf("%s %s\n", list[0].name, list[1].name);

	for (k = 0; k < 30; k++)
		list[j].name[k] = newfile.name[k];

	printf("%s %s\n", list[j].name, newfile.name);
	list[j].inode_index = newind;
	lseek(disk, i*512, SEEK_SET);
	write(disk, buf, 512);


	newfile.unused[0] = index;
	
	write_inode(newind, &newfile);
	printf("copy over\n");

}

int main(int argv, char* args[]){

	disk = open("/dev/fd0",O_RDWR);
	if (argv > 1){
		if (strcmp(format, args[1]) == 0)
			do_format();
		else if (strcmp(mkdir, args[1]) == 0){
			if (argv > 2)
				do_mkdir(args[2]);
			else 
				printf("Usage: mkdir dir\n");
		}
		else if (strcmp(chdir, args[1]) == 0){
			if (argv > 2)
				do_chdir(args[2]);
			else 
				printf("Usage: chdir dir\n");
		}
		else if (strcmp(copy, args[1]) == 0) {
			do_copy(args[2], args[3], args[4]);
		}
		else if (strcmp(dir, args[1]) == 0)
			do_dir(args[2]);
		else if (strcmp("info", args[1]) == 0){
			info(args[2]);
			}
		else if (strcmp("freeblock", args[1]) == 0)
			free_block();
		else if (strcmp("checkblock", args[1]) == 0)
			check_block(args[2]);



		else
			printf("Unknown dtool command.\n");



	}


	return 0;
}
