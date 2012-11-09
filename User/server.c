/* this is a test server,
 * run in user-space.
 *
 * it read a line from the terminal,
 * then convert all lowercase letters
 * to uppercase,
 * and write it down.
 */

#include "../types.h"
#include "tty.h"

#define TTY 1

char *hello = "May the force be with you!\n\r";
char buf[100];

int main(void) {
	int i, len;
	struct message m;
	char *str;

	show_add();

	m.type = DEV_WRITE;
	m.p1   = (int)hello;
	m.p2   = 28;
	invoke(TTY, &m);

	while (1) {
		m.type = DEV_READ;
		m.p1 = (int)buf;
		invoke(TTY, &m);

		len = m.p1;
		buf[len ++] = '\n';
		buf[len ] = 0;
		for (str = buf; *str; str ++)
			if (*str >= 'a' && *str <= 'z')
				*str = *str + 'A' - 'a';

/*		printk("SERVER%s",str);
*/
		m.type = DEV_WRITE;
		m.p1 = (int)buf;
		m.p2 = len;
		invoke(TTY, &m);
	}

}

