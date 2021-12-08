/*
 * Copyright (C) 2016-2021 Standard-robots, Inc
 *
 * file 	queue.c
 * author	Neethan <lijun@standard-robots.com> / <neethan@foxmail.com>
 * date 	2019年 05月 18日 星期六 11:09:06 CST
 *
 *  queue
 */

#include "core\include\sr_system.h"
#include "core\include\queue.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


static msg_event MSG_EV_NULL[] = {
    0, NULL, NULL
};


qevent* qevent_init(qevent* que, uint16_t size)
{	
	memset(que, 0x00, sizeof(qevent));
	que->max_size = size;
	que->front = 0;
	que->rear = 0;

	return que;
}

int32_t qevent_append(qevent *que, msg_event* ev)
{
	if ((que->rear+1)%que->max_size == que->front) 
		return -1;	

	__disable_irq();
	que->ev[que->rear].type = ev->type;
	que->ev[que->rear].len = ev->len;
	memcpy(que->ev[que->rear].data, ev->data, ev->len);	
	que->rear = (que->rear+1)%que->max_size;
	__enable_irq();

	return 0;
}

msg_event* qevent_pop(qevent *que)
{
	static msg_event ev;

	if (que->front == que->rear) 
		return MSG_EV_NULL;

	memset(&ev, 0, sizeof(ev));
	__disable_irq(); ///< __set_PRIMASK(1); 关中断,无法使用??
	ev.type = que->ev[que->front].type;
	ev.len = que->ev[que->front].len;
	memcpy(&ev.data, que->ev[que->front].data, que->ev[que->front].len);

//	sr_kprintf("ev.type=%x, ev.len=0x%x\r\n", ev.type, ev.len);
//	sr_data_print("pop ev.data", ev.data, ev.len);
	que->front = (que->front+1)%que->max_size;
	__enable_irq();

	return &ev;
}

// 如果没有使用动态内存分配，不需要释放内存
int32_t qevent_destory(qevent *que)
{
	if (!que) return 1;

	free(que);
	que = NULL;
	
	return 0;
}
