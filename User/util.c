
#include "../server.h"
void read_disk(int, int, int, int);
void write_disk(int, int, int, int);
void read_sector(int id, int seg, int offset){
	int p = (id - 1) / 18;
	int q = (id - 1) % 18;
	int head = p & 1;
	int track = p >> 1;
	int sector = q + 1;
	read_disk((track << 8) | sector, (head << 8), seg, offset);
	}

void write_sector(int id, int seg, int offset){
	unsigned int p = (id - 1) / 18;
	unsigned int q = (id - 1) % 18;
	unsigned int head = p & 1;
	unsigned int track = p >> 1;
	unsigned int sector = q + 1;
	write_disk((track << 8) | sector, (head << 8), seg, offset);
}

