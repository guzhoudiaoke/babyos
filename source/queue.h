/*
 * guzhoudiaoke@126.com
 * 2015-09-12 16:16
 */

#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <types.h>

#define BUFFER_SIZE		0x100		/* 存储扫描码的缓冲区大小 */

/* 循环队列 */
typedef struct queue_t {
	u32 front;					    /* 指向待处理的第一个元素 */
	u32 rear;					    /* 指向一个空位置，可从此处进入缓冲区 */
	u8  buffer[BUFFER_SIZE];	    /* 循环队列缓冲区 */
} queue_t;

void init_queue(queue_t *p_queue);
BOOL is_queue_empty(queue_t *p_queue);
BOOL is_queue_full(queue_t *p_queue);
BOOL en_queue(queue_t *p_queue, u8 val);
BOOL de_queue(queue_t *p_queue, u8 *p_val);

#endif
