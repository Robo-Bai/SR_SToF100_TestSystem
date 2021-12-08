/*
 * Copyright (C) 2016-2021 Standard-robots, Inc
 *
 * file 	events.c
 * author	Neethan <lijun@standard-robots.com> / <neethan@foxmail.com>
 * date 	2021年 04月 01日 星期五 11:09:06 CST
 *
 * event
 */

#include <stdio.h>
#include "core\include\sr_system.h"
#include "core\include\events.h"
#include "core\include\queue.h"



int32_t event_init(qevent* qev)
{
	qevent_init(qev, EVENTS_MAX_SIZE);
	
//	qev = qevent_init(qev, EVENTS_MAX_SIZE);
//	if (!qev) {
//		sr_kprintf("Failed to %s\r\n", __FUNCTION__);
//		return -1;
//	} 
	return 0;
}

msg_event* event_fetch(qevent* qev) 
{
	if (!qev) return NULL;
	
    return qevent_pop(qev);
}

int32_t event_dispatch(qevent* qev, msg_event* msg)
{
	if (!qev) return -1;
	
	return qevent_append(qev, msg);
}

