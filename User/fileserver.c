
#include "types.h"
#include "../server.h"
#include "clm.h"
#include "fsinfo.h"

#define MAXFILE 512
#define STACK 1024
#define FIR_PROC_SECTOR 161 
#define SEC_PROC_SECTOR 177
#define FILE_TABLE 513


typedef struct openfile{
	int off;
	int index;
	int ref;
	bool used;
};

char buf[512];
int table[10], link_block, curdir;

int pofile[16][8];
struct openfile sofile[32];
char name[30], path[61];
struct inode* nodep;
struct dir_entry* list;

int findfile(char*, bool*);
int create_newfile(int, char*, int);
int need_block(void);
void get_inode(int, struct inode*);
void write_inode(int, struct inode*);

int main(void){
	struct header* exec;
	struct message m;
	int proc_handle;
	int i, j, index, pid, off, fd, inodeb, inodex, entries;
	char* p;
	struct inode node;

	bool valid;

	read_sector(259, FM_SEGMENT, buf);
	p = (char*)&link_block;

	for (i = 0; i < 2; i++)
		p[i] = buf[i];

	printk("this is FM\n");
	table[1] = FIR_PROC_SECTOR;
	table[2] = SEC_PROC_SECTOR;

	for (i = 0; i < 16; i++){
		for (j = 3; j < 8; j++)
			pofile[i][j] = -1;

		pofile[i][0] = 0;
		pofile[i][1] = 1;
		pofile[i][2] = 2;
	}

	for (i = 0; i < 32; i++)
		sofile[i].used = FALSE;

	sofile[0].used = TRUE;
	sofile[0].index = STDIN;
	sofile[1].used = TRUE;
	sofile[1].index = STDOUT;
	sofile[2].used = TRUE;
	sofile[2].index = STDERR;

	curdir = 0;

	while (1){
		receive(&m);
		if (m.source == PM && m.type == LOAD_PROCESS){
			/*pos_sector ; pos_off*/

				proc_handle = m.p1;

				if (table[proc_handle] == 0){
					m.type = FAILURE;
					send(PM, &m);
					}
				else{

					
					m.type = DEV_READ;
					m.p1 = table[proc_handle];
					m.p2 = (int)buf;
					invoke(FLOPPY, &m);
					m.type = LOAD_PROCESS;
					m.p1 = table[proc_handle];
					
					/*
					read_sector(table[proc_handle], FM_SEGMENT, (int)buf);
					*/
					
					exec = (void*)buf;
					m.p2 = exec->a_text + exec->a_data + 32;
					m.p3 = exec->a_bss + STACK;

					send(MM, &m);
				}
		}

		else if (m.type == FORK) {

			for (i = 0; i < 8; i++) {
				index = pofile[m.p1][i];
				pofile[m.p2][i] = index;
				if (sofile[index].index < 600)
					sofile[index].ref ++;
			}

			m.type == DONE;
			send(PM, &m);

		}

		else if (m.type == EXEC){
		/*
			printk("FM EXEC\n");
		*/

			for (i = 0; i < 60; i++)
				physics_copy(m.p3, m.p1+i, FM_SEGMENT, (int)&path[i]);

			index = findfile(path, &valid);
			if (valid == TRUE && index != -1){
				get_inode(index, &node);

				m.type = EXEC;
				/*read_sector(m.p1, FM_SEGMENT, (int)buf);
				*/
				read_sector(node.addr[0], FM_SEGMENT, (int)buf);
				exec = (void*) buf;
				m.p2 = exec->a_text + exec->a_data + 32;
				m.p3 = exec->a_bss + STACK;
				m.p1 = &node;
				send(MM, &m);
			} else {
				printk("wrong path\n");
				m.type = LOADPROC_DONE;
				send(PM, &m);
			}

		}

		else if (m.type == OPEN){
			pid = m.p3;

			for (i = 0; i < 60; i++)
				physics_copy(m.p4, m.p2+i, FM_SEGMENT, &path[i]);

			path[60] = 0;

			#ifdef DEBUG
			printk("FM open\n");
			printk("the path: %s\n", path);
			#endif

			index = findfile(path, &valid);

			#ifdef DEBUG
			if (valid == FALSE)
				printk("maybe correct\n");
				printk("index: %d valid: %d\n", index, valid);
			#endif

			if (index == -1) {
				m.p1 = -1;
				m.type = DONE;
				send(PM, &m);
				continue;
			} else if (valid == FALSE) 
				index = create_newfile(index, path, BIN);

			for (j = 0; j < 32; j++)
				if (sofile[j].used == FALSE){
					sofile[j].index = index;
					sofile[j].used = TRUE;
					sofile[j].off = 0;
					sofile[j].ref = 1;

					break;
				}
			if ( j == 32 ){
				m.p1 = -1;
				m.type = DONE;
				send(PM, &m);
				continue;
			} else {

				for (i = 3; i < 8; i++)
					if (pofile[pid][i] == -1)
						break;
				pofile[pid][i] = j;
				m.p1 = j;
				m.type = DONE;
				send(PM, &m);
			}

		}

		else if (m.type == CLOSE){
			i = pofile[m.p2][m.p1];
			if (sofile[i].index < 601){
				sofile[i].ref --;
				if (sofile[i].ref == 0)
					sofile[i].used = FALSE;
			}

			pofile[m.p2][m.p1] = -1;
			m.type = DONE;
			send(PM, &m);
		}

		else if (m.type == READ){
			pid = m.p1 >> 8;
			fd = m.p1 % 8;

			#ifdef DEBUG
			printk("fd: %d; length: %d \n", fd, m.p3);
			#endif

			index = sofile[pofile[pid][fd]].index;
			off = sofile[pofile[pid][fd]].off;


			if (index == STDIN){
				m.type = DONE;
				m.p3 = -1;
				send(PM, &m);
				continue;
			}
			else{



			get_inode(index, &node);
			/*
			printk("file name: %s; file size: %d\n", node.name, node.size);
			printk("file block: %d\n", node.addr[0]);
			*/

			read_sector(node.addr[off/512], FM_SEGMENT, buf);
			/*
			printk("buf: %s", buf);
			
			*/

			for (i = 0; i < m.p3; i++){
			 	j = off % 512;
			 	if (j == 0)
			 		read_sector(node.addr[off/512], FM_SEGMENT, buf);
			 	physics_copy(FM_SEGMENT, &buf[j], m.p4, m.p2+i);
			    off ++;
			    if (off == node.size)
			    	break;
			}

			m.p3 = i;
			sofile[pofile[pid][fd]].off = off;

			m.type = DONE;
			send(PM, &m);

			}
		}

		else if (m.type == WRITE){

			pid = m.p1 >> 8;
			fd = m.p1 % 8;

			index = sofile[pofile[pid][fd]].index;
			off = sofile[pofile[pid][fd]].off;

			#ifdef DEBUG
			printk("fm_write -- fd: %d; index: %d\n", fd, index);
			#endif

			if (index == STDOUT){
				m.type = DONE;
				m.p3 = -1;
				send(PM, &m);
				continue;
			} else {

			get_inode(index, &node);

			#ifdef DEBUG
			printk("inode.name: %s; inode.size: %d\n", node.name, node.size);
			printk("off: %d; length: %d\n", off, m.p3);
			#endif

			for (i = (node.size+511) / 512; i <= (off+m.p3-1) / 512; i++){
				node.addr[i] = need_block();
				/*
				printk("nodeaddr[%d] = %d \n", i, node.addr[i]);
				*/
			}


			for (i = 0; i < m.p3; i++)
				physics_copy(m.p4, m.p2 + i, FM_SEGMENT, &buf[i]);
				/*
			printk("buf: %s\n", buf);
			*/
			buf[20] = 0;

			read_sector(node.addr[off/512], FM_SEGMENT, buf);	

			for (i = 0; i < m.p3; i++){
				j = off % 512;
				if (j == 0)
					read_sector(node.addr[off/512], FM_SEGMENT, buf);
				physics_copy(m.p4, m.p2+i, FM_SEGMENT, &buf[j]); 

				if (j == 511)
					write_sector(node.addr[off/512], FM_SEGMENT, buf);

				off ++;
			}


/*
			printk("block: %d; cont: %s \n", node.addr[(off-1)/ 512], buf);
			*/
			write_sector(node.addr[(off-1)/512], FM_SEGMENT, buf);

			/*
			read_sector(node.addr[(off-1)/512], FM_SEGMENT, buf);
			buf[20] = 0;
			printk("new buf: %s\n", buf);
			*/

			if (node.size < off)
				node.size = off;

			sofile[pofile[pid][fd]].off = off;

			write_inode(index, &node);
			/*doubt*/

			m.type = DONE;
			send(PM, &m);

			}
		}

		else if (m.type == LSEEK) {
			pid = m.p1 >> 8;
			fd = m.p1 % 8;
			if (m.p3 == SEEK_SET)
				sofile[pofile[pid][fd]].off = m.p2;
			else if (m.p3 == SEEK_CUR)
				sofile[pofile[pid][fd]].off += m.p2;
			else if (m.p3 == SEEK_END) {
				index = sofile[pofile[pid][fd]].index;
				get_inode(index, &node);
				off = node.size + m.p2 - 1;
			}

			m.type = DONE;
			send(PM, &m);
		}

		else if (m.type == CHDIR) {
		/*	printk("fm cd\n");
		*/

			for (i = 0; i < 60; i++)
				physics_copy(m.p2, m.p1+i, FM_SEGMENT, &path[i]);
			index = findfile(path, &valid);
			if (valid == TRUE && index != -1)
				curdir = index;
			else
				printk("Can't cd to %s\n", path);

			m.type = DONE;
			send(PM, &m);
		}

		else if (m.type == MKDIR) {
			for (i = 0; i < 60; i++)
				physics_copy(m.p2, m.p1+i, FM_SEGMENT, &path[i]);

			index = findfile(path, &valid);

			if (index == -1)
				printk("No such file\n");
			else if (valid == TRUE)
				printk("File exits\n");
			else {
				i = index;
				index = create_newfile(index, path, DIR);

				get_inode(index, &node);
				node.size = 64;
				node.addr[0] = need_block();
				/*
				printk("node.addr[0]: %d\n ", node.addr[0]);
				*/

				read_sector(node.addr[0], FM_SEGMENT, buf);
				list = (struct dir_entry*) buf;
				strcpy(list[0].name, ".");
				list[0].index = index;
				strcpy(list[1].name, "..");
				list[1].index = i;
				write_sector(node.addr[0], FM_SEGMENT, buf);
				write_inode(index, &node);
			}

			m.type = DONE;
			send(PM, &m);
		}

		else if (m.type == LSDIR) {

			get_inode(curdir, &node);

			/*
			printk("curdir: %d\n", curdir);
			printk("size: %d; name: %s\n", node.size, node.name);
			*/

			i = node.size / 32;
			inodeb = (i + 15) / 16;
			/*
			printk("dircnt: %d; block: %d\n", i, inodeb);
			printk("node.addr[0]: %d\n", node.addr[0]);
			*/

			for (j = 0; j < inodeb; j++) {
				read_sector(node.addr[j], FM_SEGMENT, buf);
				list = (struct dir_entry*) buf;

				for (inodex = 0; inodex < 16; inodex++) {
					if (i > 0)
						printk("%s    ", list[inodex].name);
					else break;
					i --;
				}
			}

			m.type = DONE;
			send(PM, &m);
		}

		else if (m.type == RMDIR) {
			/*
			printk("FM rmdir\n");
			*/

			for (i = 0; i < 60; i++)
				physics_copy(m.p2, m.p1+i, FM_SEGMENT, &path[i]);

			index = findfile(path, &valid);

			if (index == -1)
				printk("No such file\n");
			else if (valid == FALSE)
				printk("No such file\n");
			else {
				get_inode(index, &node);

				if (node.flag >> 8 != DIR) {
					printk("Can't rmdir\n");
					continue;
				}

				for (i = 0; i < (node.size - 1) / 512; i++)
					return_block(node.addr[i]);
				return_inode(index);

				inodex = node.unused1;
				get_inode(node.unused1, &node);

				read_sector(node.addr[(node.size-1) / 512], FM_SEGMENT, buf);
				list = (struct dir_entry*) buf;
				j = node.size / 32;
				j = (j-1) % 16;

				strcpy(name, list[j].name);
				i = list[j].index;
				
				node.size -= 32;
				if (node.size % 512 == 0)
					return_block(node.addr[node.size / 512]);
				write_inode(inodex, &node);

				entries = node.size / 32;
				inodeb = (entries + 15) / 16;

				for (j = 0; j < inodeb; j++) {
					read_sector(node.addr[j], FM_SEGMENT, buf);
					list = (struct dir_entry*) buf;
					for (fd = 0; fd < 16; fd++) {
						if (list[fd].index == index) {
							strcpy(list[fd].name, name);
							list[fd].index = i;
							write_sector(node.addr[j], FM_SEGMENT, buf);
							entries = 1;
						}
						entries --;
						if (entries == 0)
							break;
					}
				}
			}

			m.type = DONE;			
			send(PM, &m);

		}

		else if (m.type == DUP) {
			pid = m.source;

			for (i = 3; i < 8; i++)
				if (pofile[pid][i] == -1)
					break;
			if ( i == 8 ) {
				m.p1 = -1;
				send(pid, &m);
			} else {
				pofile[pid][i] = pofile[pid][m.p1];
				m.p1 = i;
				send(pid, &m);
			}

		}
		else if (m.type == DUP2) {
			/*m.p1 fd1 m.p2 fd2*/
			
			index = sofile[pofile[m.source][m.p2]].ref --;
			if (index == 1 && sofile[pofile[m.source][m.p2]].index < 600)
				sofile[pofile[m.source][m.p2]].used = FALSE;

			pofile[m.source][m.p2] = pofile[m.source][m.p1];
			sofile[pofile[m.source][m.p2]].ref ++;

			send(m.source, &m);
		}
		else if (m.type == EXIT) {
			for (i = 0; i < 8; i++){
				index = pofile[m.p1][i];
				if (index != -1){
					if (sofile[index].index < 600 && sofile[index].ref == 1) 
						sofile[index].used = FALSE;
					sofile[index].ref --;
				}
				pofile[m.p1][i] = -1;
			}
			pofile[m.p1][0] = STDIN;
			pofile[m.p2][1] = STDOUT;
			pofile[m.p3][2] = STDERR;
			m.type = DONE;
			send(PM, &m);
		}
			
	}
}
			

bool strcmp(char* s1, char* s2){
	while (*s1 != 0 && *s2 != 0){
		if (*s1 == *s2){
			s1++; s2++;
			}
		else
			return FALSE;
	}
	if (*s1 == 0 && *s2 == 0)
		return TRUE;
	else
		return FALSE;
}

void strcpy(char* s1, char* s2){
	while (*s2 != 0){
		*s1 = *s2;
		s1 ++;
		s2 ++;
	}
	*s1 = 0;
}

int need_block(void){
	int *tmp, val;
	char buf[512];
	
	read_sector(link_block, FM_SEGMENT, buf);
	tmp = (int*)buf;

	if (tmp[0] > 1){
		tmp[0]--;
		val = tmp[tmp[0]];
		write_sector(link_block, FM_SEGMENT, buf);

	} else {
		val = link_block;
		link_block = tmp[255];
		buf[0] = ((char*)&link_block)[0];
		buf[1] = ((char*)&link_block)[1];
		write_sector(259, FM_SEGMENT, buf);
	}
	return val;
}

void get_inode(int index, struct inode* node){
	int inodeb, inodex;
	int i, j;

	inodeb = (index / 8) + 195;
	inodex = index % 8;

	read_sector(inodeb, FM_SEGMENT, buf);

	nodep = (struct inode*) buf;

	strcpy(node->name, nodep[inodex].name);
	node->size = nodep[inodex].size;
	node->flag = nodep[inodex].flag;
	for (i = 0; i < 13; i++)
		node->addr[i] = nodep[inodex].addr[i];

	node->unused1 = nodep[inodex].unused1;
	node->unused2 = nodep[inodex].unused2;
}

void write_inode(int index, struct inode* node){
	int inodeb, inodex;
	int i, j;
	inodeb = (index / 8) + 195;
	inodex = index % 8;

	read_sector(inodeb, FM_SEGMENT, buf);
	nodep = (struct inode*)buf;
	strcpy(nodep[inodex].name, node->name);
	nodep[inodex].flag = node->flag;
	nodep[inodex].size = node->size;
	nodep[inodex].unused1 = node->unused1;

	for (i = 0; i < 13; i++)	
		nodep[inodex].addr[i] = node->addr[i];

	write_sector(inodeb, FM_SEGMENT, buf);
}

void return_inode(int index){
	int *tmp, j;
	
	read_sector(193, FM_SEGMENT, buf);
	tmp = (int*) buf;
	tmp[0] ++;
	j = tmp[0];

	if (j < 256) {
		tmp[j] = index;
		write_sector(193, FM_SEGMENT, buf);
	} 
	else {
		write_sector(193, FM_SEGMENT, buf);
		read_sector(194, FM_SEGMENT, buf);
		tmp = (int*) buf;
		tmp[j-256] = index;
		write_sector(194, FM_SEGMENT, buf);
	}
}

void return_block(int block){
	int* tmp;
	read_sector(link_block, FM_SEGMENT, buf);
	tmp = (int*) buf;
	if (tmp[0] == 255) {
		tmp[0] = 1;
		tmp[255] = link_block;
		link_block = block;
		write_sector(259, FM_SEGMENT, buf);
	} else {
		tmp[tmp[0]] = block;
		tmp[0] ++;
		write_sector(link_block, FM_SEGMENT, buf);
	}
}

int create_newfile(int index, char* path, int type){

	int i, j, inodeb, inodex;
	int newind;
	struct inode node, *nodep;
	struct dir_entry *list;
	int *tmp;

	/*
	printk("create file path : %s\n", path);
	*/

	i = 0;
	while (*path != 0){
		if (*path == '/')
			i = 0;
		else {
			name[i] = *path;
			i ++;
		}
		path ++;

	}
	name[i] = 0;
	/*
	printk("name: %s \n ", name);
	*/


	read_sector(193, FM_SEGMENT, buf);
	tmp = (int*) buf;
	j = tmp[0];

	if (tmp[0] == 0) /*exceed maxfile*/
		return -1;

	tmp[0] --;
	write_sector(193, FM_SEGMENT, buf);

	if (j < 256)
		newind = tmp[j];
	else {
		read_sector(194, FM_SEGMENT, buf);
		tmp = (int*) buf;
		newind = tmp[j - 256];
	}


	inodeb = (newind / 8) + 195;
	inodex = newind % 8;
	read_sector(inodeb, FM_SEGMENT, buf);
	nodep = (struct inode*) buf;

	nodep[inodex].size = 0;
	nodep[inodex].flag = type << 8;
	nodep[inodex].unused1 = index;
	for (i = 0; i < 30; i++)
		nodep[inodex].name[i] = name[i];
	write_sector(inodeb, FM_SEGMENT, buf);
		
	inodeb = (index / 8) + 195;
	inodex = index % 8;
	read_sector(inodeb, FM_SEGMENT, buf);

	nodep = (struct inode*) buf;

	if (nodep[inodex].size % 512 == 0)
		nodep[inodex].addr[nodep[inodex].size / 512] = need_block();
	nodep[inodex].size += 32;

	write_sector(inodeb, FM_SEGMENT, buf);
	read_sector(inodeb, FM_SEGMENT, buf);
	nodep = (struct inode*) buf;
	/*
	printk("size: %d \n", nodep[0].size);
	*/

	i = nodep[inodex].size;
	j = nodep[inodex].addr[(i-1) / 512];
	if (i != 512) i = i % 512;
	i = i / 32 - 1;
	read_sector(j, FM_SEGMENT, buf);

	list = (struct dir_entry*) buf;
	strcpy(list[i].name, name);
	list[i].index = newind;

	write_sector(j, FM_SEGMENT, buf);

	return newind;
}

int findfile(char *path, bool* valid){
	char name[30];
	int index, i, j; 
	int inodeb, inodex, block, entries;
	struct inode node;
	struct dir_entry* list;
	bool find;
	/*
	printk("find file path: %s\n", path);
	*/

	if (path[0] != '/'){
		index = curdir;
		path --;
	}
	else
		index = 0;
	/*
	printk("index: %d\n", index);
	*/


	i = 0;
	while (1) {
		path ++;

		if (*path != '/' && *path != 0){
			name[i] = *path;
			i++;
		}
		else {
			inodeb = (index / 8) + 195;
			inodex = index % 8;

			read_sector(inodeb, FM_SEGMENT, buf);
			name[i] = 0;

			/*printk("name : %s \n", name);
			*/

			j = inodex * 64;
			for (i = 0; i < 64; i++)
				((char*)&node)[i] = buf[j+i];
			nodep = (struct inode*) buf;

			if (node.flag >> 8 != DIR) {
				*valid = 0;
				return -1;
			}

			entries = node.size / 32;
			block = (node.size + 511) / 512;

			/*
			printk("entries : %d \n", entries);
			*/

			find = FALSE;
			for (i = 0; i < block; i++) {
				read_sector(node.addr[i], FM_SEGMENT, buf);
				j = 0;

				list = (struct dir_entry*) buf;
				while (entries > 0 && j < 16){
				/*	
					printk("dir_name: %s \n", list[j].name);
					*/

					if (strcmp(list[j].name, name)){
						index = list[j].index;
						find = TRUE;
						break;
					}
					entries --;
					j ++;
				}
				if (find == TRUE) break;
			}

			if (!find && *path != 0){
				*valid = 0;
				return -1;
			}
			else if (!find && *path == 0){
				*valid = 0;
				return index;
			} else if (*path == 0){
				*valid = 1;
				return index;
			}

			i = 0;
		}	

	}
	*valid = 1;
	return index;
	
}
