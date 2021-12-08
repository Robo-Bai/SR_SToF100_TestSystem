/**
 * Copyright (C) 2019 Standard-robots, Inc
 *
 * @file 	sr_eu.c
 * @author	Neethan <lijun@standard-robots.com> / <neethan@foxmail.com>
 * @version V1.0.0
 * @date 	2019年 06月 19日 星期三 10:50:47 CST
 *
 * @brief    eu100 devices driver.
 */

#include <string.h>
#include "srtos\srtos.h"
#include "drivers\include\drivers\sr_eu100.h"



/* 此id为eu100设备上报的id，不能是下发给eu100设备的id */
static const uint16_t eu100_base_rid[] = {
	0x401, 0x402, 0x403, 0x404, 0x405, 0x406,
};

/* 此id为步科电机下发设置的id，不能是电机上报数据的id 
 * cmd , heart
 */
static const uint16_t eu100_base_sid[] = {
	0x301, 0x302, 0x303, 0x304, 0x305, 0x306,
};

static struct can_msg eu100_msg = {0};

enum obj_type {
	OBJ_JACKSOLO 	= 0x11, /* O200顶升 */
	OBJ_EUIO 		= 0x21, /* O200滚筒 */
	OBJ_HD24CAN 	= 0x31, /* O400顶升 */
	OBJ_AEROPOD1 	= 0x32, /* Aeropod伸缩 */
};


uint16_t eu100_sid_heart_map(sr_device_id_t dev)
{
	switch(dev){
		case DEV_EU100_1:
		return eu100_base_sid[0];
		
		case DEV_EU100_2:
		return eu100_base_sid[1];
		
		case DEV_EU100_3:
		return eu100_base_sid[2];
		
		case DEV_EU100_4:
		return eu100_base_sid[3];
		
		case DEV_EU100_5:
		return eu100_base_sid[4];
		
		case DEV_EU100_6:
		return eu100_base_sid[5];	
		
		default:
			return 0;
	}	
}

static int32_t eu100_msg_map(uint16_t can_id, sr_motor_msg_type_t *type)
{
	uint8_t j = 0;
	
	for (j=0; j<sizeof(eu100_base_rid)/sizeof(eu100_base_rid[0]); j++){
		if (can_id == eu100_base_rid[j]){
			*type = EU_MSG;
			return 0;
		}
	} 
	*type = MOTOR_MSG_NULL;
	return -1;
}


static int32_t eu100_write(uint32_t can_id, uint8_t* data, 
											uint8_t len, uint16_t timeout)
{	
	memset(&eu100_msg.tx_msg, 0, sizeof(eu100_msg.tx_msg));

#ifdef DEVICE_STAND_ID
	eu100_msg.tx_msg.StdId = can_id;
	eu100_msg.tx_msg.RTR	= CAN_RTR_DATA; 
	eu100_msg.tx_msg.IDE 	= CAN_ID_STD;  ///< 设定消息标识符的类型：标准帧
#elif defined(DEVICE_EXTEND_ID)
	eu100_msg.tx_msg.ExtId = DEVICE_EXTEND_ID;
	eu100_msg.tx_msg.RTR 	= CAN_RTR_DATA; 
	eu100_msg.tx_msg.IDE 	= CAN_ID_EXT; ///< 扩展帧
#endif
	eu100_msg.tx_msg.DLC 	= 8;
	memcpy(&eu100_msg.tx_msg.Data, data, 8);
	
	return sr_can_write(SR_CAN1, &eu100_msg, timeout);
}

static int32_t eu100_exceptions_cb(struct _device_manager *device, sr_motor_msg_type_t type,
											uint8_t* data)
{
	static volatile uint32_t cnt = 0;
	static struct dev_status dev_state = {NULL,0};

	/* 错误码 */			
	cnt++;
	if (cnt > 200){ // 1000 * 上报数据周期20ms = 4s
		cnt = 201;
		dev_state.dev = device;
		memcpy(dev_state.data, data, 8);
		if ( xQueueSend(get_dev_status_handle(), (void *)&dev_state, 0) != pdPASS){	
			//sr_kprintf("failed dev status to xQueueSend, %s\r\n", __FUNCTION__);
			return -1;
		}
	}
	return 0;
}

int32_t sr_eu100_start(sr_device_id_t type)
{
	uint8_t data[] = {0xF2, 0x40, 0x60, 0x00, 0x01, 0x00, 0x00, 0x00};

	return eu100_write(eu100_sid_heart_map(type), data, sizeof(data), 2);
}


/**
 * eu100初始化
 *
 * @param[in]	dev: 设备号
 * @param[out]	None
 *
 * @return		>=0成功，其他失败
 */
int32_t sr_eu100_init(struct _device_manager *device, sr_device_id_t type,
								device_rx_cb cb)
{
	int32_t ret = -1;
	
	if (device){
		device->rx_cb = cb;
		device->msg_map = eu100_msg_map;
		device->e_handler = eu100_exceptions_cb;
	}
	ret = sr_eu100_rst(type);
	vTaskDelay(1000);	
	ret = sr_eu100_start(type);
	vTaskDelay(1000);
	sr_kprintf("%s device %d\n", __FUNCTION__, type);
	return ret;
}

/**
 * eu100复位
 *
 * @param[in]	dev: 设备号
 * @param[out]	None
 *
 * @return		>=0成功，其他失败
 */
int32_t sr_eu100_rst(sr_device_id_t dev)
{
	uint8_t data[] = {0xF2, 0x40, 0x60, 0x00, 0x02, 0x00, 0x00, 0x00};
	
	return eu100_write(eu100_sid_heart_map(dev), data, sizeof(data), 2);
}

int32_t eu100_ctrl(sr_device_id_t dev, uint8_t *data, uint8_t len)
{
	return eu100_write(eu100_sid_heart_map(dev), data, sizeof(data), 2);
}


int32_t sr_eu100_heart_beat(sr_device_id_t dev, uint8_t *data1, uint16_t len)
{
	uint8_t data[] = {0xA0, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	
	return eu100_write(eu100_sid_heart_map(dev), data, sizeof(data), 2);
}
