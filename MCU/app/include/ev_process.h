#ifndef __EV_PROCESS_H__
#define __EV_PROCESS_H__


#include <stdint.h>
#include "core\include\events.h"


#if __cplusplus
extern "C" {
#endif


/* CANOPEN SDO协议 */
#define CS_CMD_W1_BYTE				0x2F
#define CS_CMD_W2_BYTES				0x2B
#define CS_CMD_W3_BYTES				0x27
#define CS_CMD_W4_BYTES				0x23
#define CS_CMD_READ					0x40
#define CS_CMD_R1_BYTE				0x4F
#define CS_CMD_R2_BYTES				0x4B
#define CS_CMD_R3_BYTES				0x47
#define CS_CMD_R4_BYTES				0x43
	
#define CS_RES_SUC					0x60
#define CS_RES_FAILE				0x80


/* 事件码定义 */
#define EV_CMD				0x0001		/* 指令事件 */
#define EV_STATE			0x0002		/* 状态事件 */
#define EV_PARAM			0x0003		/* 参数事件 */



int32_t sr_event_init(void);
int32_t ev_handle(void);


int32_t sr_report_state(void);


#if __cplusplus
}
;
#endif
#endif ///< __EV_PROCESS_H__
