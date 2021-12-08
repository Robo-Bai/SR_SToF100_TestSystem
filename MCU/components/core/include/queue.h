#ifndef __QUEUE_H__
#define __QUEUE_H__


#include <stdint.h>
#include "core\include\events.h"

#if __cplusplus
extern "C" {
#endif


/*
 * 创建一个消息队列
 *
 * @param[in]	size 消息队列大小
 *
 * @param[out]	None.
 *
 * @return		返回创建的消息队列或返回NULL失败
 */
qevent* qevent_init(qevent* que, uint16_t size);

int32_t qevent_append(qevent *que, msg_event* ev);

msg_event* qevent_pop(qevent *que);

int32_t qevent_destory(qevent *que);


#if __cplusplus
}
;
#endif
#endif ///< __QUEUE_H__

