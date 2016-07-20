/*
 * guzhoudiaoke@126.com
 * 2015-09-12 16:16
 */

#include "queue.h"

void init_queue(queue_t *p_queue)
{
	p_queue->front = 0;
	p_queue->rear = 0;
}

BOOL is_queue_empty(queue_t *p_queue)
{
	if (p_queue->rear == p_queue->front)
		return TRUE;
	
	return FALSE;
}

BOOL is_queue_full(queue_t *p_queue)
{
	if ((p_queue->rear + 1) % BUFFER_SIZE == p_queue->front)
		return TRUE;
	
	return FALSE;
}

BOOL en_queue(queue_t *p_queue, u8 val)
{
	if (is_queue_full(p_queue))
		return FALSE;

	p_queue->buffer[p_queue->rear] = val;
	p_queue->rear = (p_queue->rear + 1) % BUFFER_SIZE;

	return TRUE;
}

/* 队头元素出队，即将p_head指向的元素拿出来，队头后移 */
BOOL de_queue(queue_t *p_queue, u8* val)
{
	if (is_queue_empty(p_queue))
		return FALSE;

	*val = p_queue->buffer[p_queue->front];
	p_queue->front = (p_queue->front + 1) % BUFFER_SIZE;
	
	return TRUE;
}

