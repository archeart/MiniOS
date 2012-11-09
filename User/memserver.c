#include "types.h"
#include "../server.h"
#include "clm.h"
#include "fsinfo.h"

#define mask 127

unsigned char label[4][128];
uint_8 buf[512], buf2[512];
struct inode node;

int find_memory(int need_page){
	int i, j, k, tmp;

	for (k = 0; k < 4; k++){

	j = 0;
	for (i = 0; i < 128; i++){
		if (label[k][i] == 0)
			j++;
		else 
			j = 0;

		if (j == need_page){
			tmp = k * 128 + i+1-need_page;
			return tmp;
			}
	}
	}

	return -1;
}

void apply_memory(int k, int off, int p){
	int i;
	for (i = 0; i < p; i++)
		label[k][off + i] = 1;
}

void release_memory(int k, int off, int p){
	int i;
	for (i = 0; i < p; i++)
		label[k][off + i] = 0;
}

void write_block(int seg, uint_8* buf){
	int i;
	for (i = 0; i < 512; i++)
		physics_copy(MM_SEGMENT, (int)&buf[i], seg, i);
	}
		
int main(void){

	int need_page, room, pid, sector, i, j;
	uint_16 seg, base, seg1;
	struct message m;
	uint_8 zero = 0;
	uint_8 *p;


	printk("this is MM\n");
	release_memory(0, 0, 128);
	release_memory(1, 0, 128);
	release_memory(2, 0, 128);
	release_memory(3, 0, 128);
	/*
	while (1) {printc('a'); idle_cpu();}
	*/

	while (1){
		receive(&m);
		/*suppose all the request comes from PM*/

		if (m.type == LOAD_PROCESS){
			need_page = (m.p2 + m.p3 + 511) / 512;

			room = find_memory(need_page); 
			if (room == -1){
				m.type = FAILURE;
				send(FM, &m);
				printk("failure?\n");
			}
			else{
				apply_memory(room >> 7, room & mask, need_page);

				seg = (room << 5) + 0x3000;

				base = seg;
				pid = m.p4;
				sector = m.p1;
				j = (m.p2 + 511) / 512;

				/*	
				for (i = 0; i < (m.p2 + 511) / 512; i++){
					read_sector(i+m.p1, base, 0);
					base += 32;
				}
				*/
			
				for (i = 0; i < j; i++){
					m.type = DEV_READ;
					m.p1 = sector + i;
					m.p2 = (int)buf;
					invoke(FLOPPY, &m);
					write_block(seg+ i*32, (uint_8*)buf);
					base += 32;
				}
			

				for (i = 0; i < m.p3; i++)		
					physics_copy(MM_SEGMENT, (uint_16)&zero, seg, m.p2 + i);

				m.type = LOADPROC_DONE;
				m.p1 = room; /* cs,ds,ss */
				m.p2 = room + need_page - 1;
				m.p4 = pid;

				send(PM, &m);

			}
		}
		
		if (m.type == EXEC){
			/*printk("MM EXEC\n");
			*/
			need_page = (m.p2 + m.p3 + 511) / 512;

			room = find_memory(need_page); 
			if (room == -1){
				m.type = FAILURE;
				send(FM, &m);
				printk("failure?\n");
			}
			else{
				apply_memory(room >> 7, room & mask, need_page);

				seg = (room << 5) + 0x3000;

				base = seg;
				pid = m.p4;
				j = (m.p2 + 511) / 512;

			
				for (i = 0; i < 64; i++)
					physics_copy(FM_SEGMENT, m.p1+i, MM_SEGMENT, (int)&node + i);
				/*printk("MM name:%s, size:%d, addr[0]:%d\n", node.name, node.size, node.addr[0]);

				for (i = 0; i < 4; i++)
					printk("addr[%d]: %d  ", i, node.addr[i]);

				*/
				/*	
				for (i = 0; i < (m.p2 + 511) / 512; i++){
					read_sector(i+m.p1, base, 0);
					base += 32;
				}
				*/
			
				for (i = 0; i < j; i++){
					m.type = DEV_READ;
					m.p1 = node.addr[i];
					m.p2 = (int)buf;
					invoke(FLOPPY, &m);
					write_block(seg+ i*32, (uint_8*)buf);
					base += 32;
				}
			
				/*
				for (i = 0; i < m.p3; i++)		
					physics_copy(MM_SEGMENT, (uint_16)&zero, seg, m.p2 + i);
				*/


				m.type = LOADPROC_DONE;
				m.p1 = room; /* cs,ds,ss */
				m.p2 = room + need_page - 1;
				m.p4 = pid;

				send(PM, &m);

			}
		}
		
		else if(m.type == FORK){
			need_page = m.p2 - m.p1 + 1;
			room = find_memory(need_page);
			if (room == -1){
				m.type = FORK_FAILURE;
				send(PM, &m);
			}
			else{

				apply_memory(room >> 7, room & mask, need_page);
				seg = m.p1 * 32 + 0x3000;
				seg1 = room * 32 + 0x3000;
				m.p1 = room;
				m.p2 = room + need_page -1;


				for (i = 0; i < (need_page * 512); i++)
					physics_copy(seg, i, seg1, i);

			/*	
				printk("%x bytes copied\n", need_page*512);
				*/


				/*
				i = 0;
				physics_copy(seg+2, 0x7D8, MM_SEGMENT, &i);
				physics_copy(seg+2, 0x7D9, MM_SEGMENT, (&i)+1);
				printk("[%x:%x] = %d ", seg+2, 0x9D0, i);
				physics_copy(seg1+2, 0x7D8, MM_SEGMENT, &i);
				physics_copy(seg1+2, 0x7D9, MM_SEGMENT, (&i)+1);
				printk("[%x:%x] = %d ", seg1+2, 0x9D0, i);
				*/
				

				m.type = FORK_DONE;
				send(PM, &m);
				}

		}

		else if (m.type == RELEASE_MEMORY){
			release_memory(m.p1>> 7, m.p1 & mask, m.p2 - m.p1 + 1);
			send(m.source, &m);
		}

		else if (m.type == LOAD_BUF){
			for (i = 0; i < m.p3; i++)
				physics_copy(m.p1, m.p2+i, MM_SEGMENT, &buf2[i]);

			m.type = DONE;
			send(m.source, &m);
		}

		else if (m.type == PUSH_BUF){
			for (i = 0; i < m.p3; i++)
				physics_copy(MM_SEGMENT, &buf2[i], m.p1, m.p2+i);

			p = (uint_8 *)&m.p2;
			
			physics_copy(MM_SEGMENT, p++, m.p1, m.p2+1024);
			physics_copy(MM_SEGMENT, p--, m.p1, m.p2+1025);

			p = 0;
			physics_copy(m.p1, m.p2+1024, MM_SEGMENT, p);
			physics_copy(m.p1, m.p2+1025, MM_SEGMENT, p+1);

			m.type = DONE;
			send(m.source, &m);
		}

	}
}
