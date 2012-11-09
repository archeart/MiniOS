#include "kernel.h"

#define CYCLES 11932

/* let this be PID of timer_driver */

/* 100Hz timer */
void init_timer(void) {
	out_byte(0x43, 0x34);
	out_byte(0x40, CYCLES % 256);
	out_byte(0x40, CYCLES / 256);
}

/* some variables */
static struct message tm;
static int alarm_tick = -1;
static int n = 0;
static int queue[NR_PROCESS];
static int alrm_id[NR_PROCESS];

void scheduler(void);
void irq0(void) {
/*	current_pcb->time_tick ++;  you may not implement this, this is for cpu accounting, it indicates how much time does current process used. remove it as you wish. */
	if (n > 0 && queue[0] >= 0) { /* there is a alarm */
		queue[0] --; /* decrease the alarm head by 1 */
		if (queue[0] < 0) { /* alarm fired */
			tm.type = HARDWARE_INT; /* send a timer message */
			ker_int_send(TIMER_DRIVER, (struct message*)&tm);
		}
	}


	end_of_interrupt(); /* end of interrupt */
	maintain();
/*	queue_visit();*/
	scheduler();
}

static struct message m;
void lock(void){
	cli();
	}
void unlock(void){
	sti();
	}

void timer_driver(void) {
	int i, j, sum;
	int reply;

	while (1) {
		receive((struct message*)&m);
		if (m.type == HARDWARE_INT) { /* alarm-fire message */
			lock();
			do { /* remove the alarm queue head */
				m.type = DEV_DONE;
				m.p1 = alrm_id[0];
				send(reply, (struct message*)&m); /* send the alarm handle */
				for (i = 1; i < n; i ++) {
					queue[i - 1] = queue[i];
					alrm_id[i - 1] = alrm_id[i];
				}
				n --;
			} while (n > 0 && queue[0] == 0);
			unlock();
		} else if (m.type == DEV_WRITE) { /* add a alarm into queue */
			lock();
			reply = m.source; /* this is under assumption that, only PM send messages to the timer driver */
			sum = 0;
			for (i = 0; i < n; i ++) {
				sum += queue[i];
				if (sum >= m.p1) {
					sum -= queue[i]; /* find a location to insert */
					break;
				}
			}
			for (j = n - 1; j >= i; j --) { /* clean up a position */
				queue[i + 1] = queue[i];
				alrm_id[i + 1] = alrm_id[i];
			}
			n ++;
			queue[i] = m.p1 - sum; /* enqueue this alarm */
			alrm_id[i] = m.p2;
			for (j = i + 1; j < n; j ++) /* setup relative alarm value */
				queue[j] -= queue[i];
			unlock();
		}
	}
}
