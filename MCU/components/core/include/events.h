#ifndef __EVENTS_H__
#define __EVENTS_H__

#include <stdint.h>

#if __cplusplus
extern "C" {
#endif

#define EVENTS_MAX_SIZE		10
#define EV_INVALID   		0x0000  	/* 无效事件 */

typedef struct event
{
	uint16_t type;
	uint16_t len;
	uint8_t data[256];
} msg_event;

typedef int32_t (*event_msg_handler_t)(msg_event* ev, void* cookie);

typedef struct ev_map
{
	uint16_t type;
	event_msg_handler_t handler;
} event_map;


typedef struct queue {
	msg_event ev[EVENTS_MAX_SIZE];
	uint16_t max_size;
	uint16_t front;
	uint16_t rear;
} qevent;



/*
 * 初始化消息事件
 *
 * @param[in]	None.
 * @param[out]	None.
 *
 * @return		0表示成功,其它返回失败
 */
int32_t event_init(qevent* qev);

msg_event* event_fetch(qevent* qev) ;

int32_t event_dispatch(qevent* qev, msg_event* msg);



#if __cplusplus
}
;
#endif
#endif ///< __EVENTS_H__
