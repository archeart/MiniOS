void send(uint_8, struct message*);
void receive(struct message*);
void invoke(uint_8, struct message*);

void physics_copy(uint_16, uint_16, uint_16, uint_16);
void read_sector(int, int, int);
void write_sector(int, int, int);


void printk(char*, ...);
void read_disk(int, int, int, int);
void write_disk(int, int, int, int);


