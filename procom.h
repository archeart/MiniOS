#include "types.h"

void sem_init(struct semaphore*, int, bool);
void P(struct semaphore*);
void V(struct semaphore*);
void ker_send(uint_8, struct message*);
void ker_int_send(uint_8, struct message*);
void ker_receive(struct message*);
