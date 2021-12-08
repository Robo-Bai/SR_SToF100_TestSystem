/*
 * Copyright (C) 2016-2021 Standard-robots, Inc
 *
 * file 	ev_process.c
 * author	Neethan <lijun@standard-robots.com> / <neethan@foxmail.com>
 * date 	2019年 05月 14日 星期一 11:44:47 CST
 *
 * ev process
 */

#include <stdio.h>
#include <string.h>
#include "core\include\sr_system.h"
#include "drivers\include\drivers\sr_serial.h"
#include "drivers\mtd\stmflash.h"
#include "drivers\include\drivers\drv_hwtimer.h"
#include "..\app\include\ev_process.h"
#include "..\app\include\unittest_manager.h"

static int32_t ev_cmd_cb(msg_event* ev, void* cookie);
static int32_t ev_state_cb(msg_event* ev, void* cookie);
static int32_t ev_param_cb(msg_event* ev, void* cookie);

qevent qev_tx;



const event_map event_maps[] = {
	{EV_CMD, ev_cmd_cb},
	{EV_STATE, ev_state_cb},
	{EV_PARAM, ev_param_cb},
	
	{EV_INVALID, NULL}
};


int32_t ev_handle(void)
{
	uint8_t i = 0;
	
	msg_event* ev_msg = event_fetch(&qev_tx);
	
	if (ev_msg->type == EV_INVALID) return -1;
	
	for (i=0; i<sizeof(event_maps)/sizeof(event_map); ++i){
		if (event_maps[i].type == ev_msg->type) {
			if (event_maps[i].handler){
				return event_maps[i].handler(ev_msg, NULL);
			}
		}
	}
	return -1;
}

int32_t sr_event_init(void)
{
	int32_t ret = 0;
	
	ret = event_init(&qev_tx);
	if (ret < 0){
		return -1;
	}
	return ret;
}

char *eventmap2str(uint16_t type)
{
	char *str = NULL;

	switch (type){
		case EV_CMD:
			str = "cmd";
		break;
		
		case EV_STATE:
			str = "state";
		break;
		
		case EV_PARAM:
			str = "param";
		break;
		default:
			break;
	}
	return str;
}
			
int32_t ev_cmd_cb(msg_event* ev, void* cookie)
{
//	sr_kprintf("%s, ev->type=0x%04x, ev->len=0x%02x\r\n", __FUNCTION__, ev->type, ev->len);
//	sr_data_print("ev_cmd_cb", ev->data, ev->len);
	
	return sr_msg_package(ev->data, ev->len);
}

int32_t ev_state_cb(msg_event* ev, void* cookie)
{
//	sr_kprintf("%s, ev->type=0x%04x, ev->len=0x%02x\r\n", __FUNCTION__, ev->type, ev->len);
//	sr_data_print("data", ev->data, ev->len);
	
	return sr_msg_package(ev->data, ev->len);
}

int32_t ev_param_cb(msg_event* ev, void* cookie)
{
//	sr_kprintf("%s, ev->type=0x%04x, ev->len=%d\r\n", __FUNCTION__, ev->type, ev->len);
//	sr_data_print("data", ev->data, ev->len);
	
	return sr_msg_package(ev->data, ev->len);
}

int32_t request_cmd(void)
{
	msg_event msg;
	uint8_t cmd[] = {0x07, 0x10, 0x00, 0x20, 0x11, 0x00};
	
	memset(&msg, 0, sizeof(msg));
	msg.type = EV_STATE;
	msg.len = sizeof(cmd);
	memcpy(&msg.data, cmd, sizeof(cmd));
	return  event_dispatch(&qev_tx, &msg);
}


