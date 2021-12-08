#ifndef __DRV_CAN_H__
#define __DRV_CAN_H__
	

#include "stm32f4xx.h"

typedef CAN_TypeDef can_typedef_t;

#define	DEVICE_STAND_ID			
//#define	DEVICE_EXTEND_ID	


#ifndef SR_CAN1
#define  SR_CAN1                   CAN1
#endif

struct can_msg {
	CanRxMsg rx_msg;
	CanTxMsg tx_msg;
};

typedef int32_t (*can_msg_event_handler_t)(CAN_TypeDef* can_x, struct can_msg *msg, 
	uint8_t mode, uint32_t timeout);

struct sr_can_ops {
	const int8_t name[16];
	int32_t (*init)(void);
	int32_t (*write)(CanTxMsg tx_msg, void *cookie); 
	int32_t (*read)(CanRxMsg rx_msg, void *cookie); 
    int32_t (*register_event_listener)(const int8_t* name,
            can_msg_event_handler_t handler);

	int32_t (*deinit)(void);
	struct sr_can_ops* next;
};

int32_t register_bsp_can(void);
void drv_can_state_set(uint8_t status);
QueueHandle_t* can_que_handler_get(void);
void can_tx_precfg(CanTxMsg *tx_msg, uint32_t can_id, uint8_t* data);

/*
 * 通过can接口发送数据
 *
 * @param[in]	None
 * @param[out]	None
 *
 * @return		0成功，其他失败
 */
int32_t sr_can_write(can_typedef_t *can_x, struct can_msg *msg, uint16_t timeout);



#endif /* __DRV_CAN_H__ */


