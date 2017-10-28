#include <string.h>
#include "circular_buffer.h"

void cb_init(struct channel *ch) {
	channel->head = 0;
	channel->tail = 0;
	sem_init(&channel->sem, 0, 0);
}

void cb_push(struct channel *ch, char* msg) {
	strcpy(channel->buffer[channel->tail], msg);
	channel->tail = (channel->tail + 1) % 10;
	sem_post(&channel->sem); 
}

void cb_pop(struct channel* ch, char* buf) {
	sem_wait(&channel->sem);
	strcpy(buf, channel->buffer[channel->head]);
	channel->head = (channel->head + 1) % 10;
}