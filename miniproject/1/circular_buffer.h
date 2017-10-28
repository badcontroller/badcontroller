/*
	Circular buffer as mailboxes for threads.
*/

#ifndef _CHANNEL_H
#define _CHANNEL_H

typedef struct circular_buffer {
	char data[16][32];
	size_t head;
	size_t tail;
	sem_t sem;
}

void cb_init(channel *ch);
void cb_push(channel *ch, char *msg);
void cb_pop(channel *ch, char *buf);

#endif // _CHANNEL_H