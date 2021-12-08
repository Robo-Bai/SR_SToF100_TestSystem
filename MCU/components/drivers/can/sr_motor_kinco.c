/**
 * Copyright (C) 2019 Standard-robots, Inc
 *
 * @file 	sr_motor_kinco.c
 * @author	Neethan <lijun@standard-robots.com> / <neethan@foxmail.com>
 * @version V1.0.0
 * @date 	2019年 07月 24日 星期三 10:50:47 CST
 *
 * @breif	srtos kinco motor drivers. 
 */

#include <string.h>
#include "drivers\include\device.h"
#include "drivers\include\drivers\sr_motor_kinco.h"


// 转化为速度
#define MOTOR_KINCO_SPD_CONVER(x) ((int32_t)((float)(x)*512*10000/1875))

kinco_rx_handler kinco_rx_handle = NULL;
static struct can_msg kinco_msg = {0};


/* 此id为步科电机上报的id，不能是下发给电机设备的id 
 *
 * 0x181 ~ 0x198为18轴电机1#输入口-8字节 (LSB)
	 6063-0020 32bit 位置 data[0-3]		||
	 6041-0010 16bit 状态字 data[4-5]   \/
	 6078-0010 16bit 实际电流 data[6-7]
   0x281 ~ 0x298为18轴电机2#输入口-8字节  
	 606C-0020 32bit 速度 data[0-3]
	 2601-0010 16bit 错误码1 data[4-5]
	 2602-0010 16bit 错误码2 data[6-7]
   0x381 ~ 0x398为18轴电机3#输入口-8字节  
	 6040-0010 16bit 控制字
	 2010-0A10 16bit Din实际输入
	 60F7-0B10 16bit 驱动器实际温度
	 6077-0010 16bit 实际扭矩
   0x481 ~ 0x498为18轴电机4#输入口-5字节  
	 6060-0008 8bit  工作模式

	0x581 ~ 0x598为18轴电机4#输入口-x字节	
	  ack
*/
static uint16_t kinco_motor_base_rid[][6] = {
/* TPDO1,TPDO2,TPDO3,TPDO4 */
	
{0x181, 0x281, 0x381, 0x481, 0x581, 0x701},
{0x182, 0x282, 0x382, 0x482, 0x582, 0x702},
{0x183, 0x283, 0x383, 0x483, 0x583, 0x703},
{0x184, 0x284, 0x384, 0x484, 0x584, 0x704},
{0x185, 0x285, 0x385, 0x485, 0x585, 0x705},
{0x186, 0x286, 0x386, 0x486, 0x586, 0x706},
};

/* 此id为步科电机下发SDO设置的id，不能是电机上报数据的id 
 * cmd , heart
 */
static uint16_t kinco_base_sid[][2] = {
/* ID = 0x600+右1，左2 */
{0x601, 0x701},
{0x602, 0x702},
{0x603, 0x703},
{0x604, 0x704},
{0x605, 0x705},
{0x606, 0x706},
};


static uint8_t kinco_row = sizeof(kinco_motor_base_rid)/sizeof(kinco_motor_base_rid[0]); //行数
static uint8_t kinco_col = sizeof(kinco_motor_base_rid[0])/sizeof(kinco_motor_base_rid[0][0]); 

int32_t kinco_map(uint16_t can_id, sr_motor_msg_type_t *type)
{
	uint16_t index = 0;
	uint8_t j = 0;
	
	while (index < kinco_col){  // 列
		for (j=0; j<kinco_row; j++){
			if (can_id == kinco_motor_base_rid[index][j]){
				*type = (sr_motor_msg_type_t)(j + 1);
				return 0;
			}
		} 
		index++;
	}
	*type = MOTOR_MSG_NULL;
	return -1;
}

uint16_t* kinco_sid_heart_map(sr_device_id_t dev, sr_sid_heart_t type)
{
	uint8_t index = 0;
	
	if (SR_SID == type){
		switch(dev){
			case DEV_MOTOR_MC_1:
			return &kinco_base_sid[0][index];
			case DEV_MOTOR_MC_2:
			return &kinco_base_sid[1][index];
			case DEV_MOTOR_MC_3:
			return	&kinco_base_sid[2][index];
			case DEV_MOTOR_MC_4:
			return	&kinco_base_sid[3][index];
			case DEV_MOTOR_AC_1:
			return	&kinco_base_sid[4][index];
			case DEV_MOTOR_AC_2:
			return	&kinco_base_sid[5][index];

			default:
				return NULL;	
		}
	} else if(SR_HEART == type){
		index = 1;
		switch(dev){
			case DEV_MOTOR_MC_1:
			return &kinco_base_sid[0][index];
			case DEV_MOTOR_MC_2:
			return &kinco_base_sid[1][index];
			case DEV_MOTOR_MC_3:
			return	&kinco_base_sid[2][index];
			case DEV_MOTOR_MC_4:
			return	&kinco_base_sid[3][index];
			case DEV_MOTOR_AC_1:
			return	&kinco_base_sid[4][index];
			case DEV_MOTOR_AC_2:
			return	&kinco_base_sid[5][index];

			default:
				return NULL;	
		}
	}
	return NULL;	
}

int32_t kinco_register(sr_motor_vendor_t vendor, kinco_rx_handler handle)
{
	if (NULL == handle) 
		return -1;
	else if (NULL == kinco_rx_handle){
		//sr_m_type = m_type;
		kinco_rx_handle = handle;
	} else {
		kinco_rx_handle = handle;
		return 1;
	}
	
	return 0;
}

uint16_t* kinco_motor_id_get(void)
{
	return kinco_motor_base_rid[0];
}

static int32_t kinco_can_write(uint32_t can_id, uint8_t* data, 
											uint8_t len, uint16_t timeout)
{	
	memset(&kinco_msg.tx_msg, 0, sizeof(kinco_msg.tx_msg));

#ifdef DEVICE_STAND_ID
	kinco_msg.tx_msg.StdId = can_id;
	kinco_msg.tx_msg.RTR	= CAN_RTR_DATA; 
	kinco_msg.tx_msg.IDE 	= CAN_ID_STD;  ///< 设定消息标识符的类型：标准帧
#elif defined(DEVICE_EXTEND_ID)
	kinco_msg.tx_msg.ExtId = DEVICE_EXTEND_ID;
	kinco_msg.tx_msg.RTR 	= CAN_RTR_DATA; 
	kinco_msg.tx_msg.IDE 	= CAN_ID_EXT; ///< 扩展帧
#endif
	kinco_msg.tx_msg.DLC 	= 8;
	memcpy(&kinco_msg.tx_msg.Data, data, 8);
	
	return sr_can_write(SR_CAN1, &kinco_msg, timeout);
}


/**
 * kinco节点保护，需要按照驱动器设置的时间内下发
 *
 * @param[in]	pos: 电机所在位置
 * @param[out]	None
 *
 * @return		>=0成功，其他失败
 */
int32_t kinco_node_protect(sr_device_id_t dev, uint8_t *data1, uint16_t len)
{
	int32_t ret = -1;
	uint8_t data[8];
	struct can_msg heart_msg = {0};
	
	memset(data, 0, sizeof(data));
	memset(&heart_msg.tx_msg, 0, sizeof(heart_msg.tx_msg));

	heart_msg.tx_msg.IDE 	= CAN_ID_STD;  ///< 设定消息标识符的类型：标准帧
	heart_msg.tx_msg.StdId 	= *kinco_sid_heart_map(dev, SR_HEART);
	heart_msg.tx_msg.RTR 	= CAN_RTR_REMOTE; 
	heart_msg.tx_msg.DLC 	= 8;
	memcpy(&heart_msg.tx_msg.Data, data, heart_msg.tx_msg.DLC);
	ret = sr_can_write(SR_CAN1, &heart_msg, 2);
	vTaskDelay(SYSTEM_TICK_MS);  // 小于1ms给电机发心跳会出现某一个电机数据发不下去??
	return ret;
}

	

/**
 * 查询驱动器编号操作A，CAN_ID：60x其中x表示驱动器地址编号，
 * 反馈58x，x驱动器地址
 *
 * @param[in]	pos: 电机所在位置
 * @param[out]	None
 *
 * @return		>=0成功，其他失败
 */
int32_t kinco_num_a_check(sr_device_id_t dev)
{
	uint8_t data[8] = {0x60, 0xf8, 0x2f,0,0,0,0,0};
	return kinco_can_write(*kinco_sid_heart_map(dev, SR_SID), data, sizeof(data), 2);
}

/**
 * 查询驱动器编号操作B，CAN_ID：60x其中x表示驱动器地址编号，
 * 反馈58x，x驱动器地址
 *
 * @param[in]	pos: 电机所在位置
 * @param[out]	None
 *
 * @return		>=0成功，其他失败
 */
int32_t kinco_num_b_check(sr_device_id_t dev)
{
	uint8_t data[8] = {0x70, 0xf8, 0x2f,0,0,0,0,0};
	return kinco_can_write(*kinco_sid_heart_map(dev, SR_SID), data, sizeof(data), 2);
}

int32_t kinco_fd122_write(sr_device_id_t dev, uint8_t* data, 
							uint8_t len, uint16_t timeout)
{
	return kinco_can_write(*kinco_sid_heart_map(dev, SR_SID), data, len, timeout);
}


/**
 * kinco fd122 清除指定电机错误状态
 *
 * @param[in]	pos: 电机位置
 * @param[out]	None
 *
 * @return		>=0成功，其他失败
 */
int32_t kinco_fd122_err_clr(sr_device_id_t dev)
{
	uint8_t data[] = {0x2B, 0x40, 0x60, 0x00, 0x86, 0x00, 0x00, 0x00};
	
	return kinco_can_write(*kinco_sid_heart_map(dev, SR_SID), data, sizeof(data), 2);
}

// 开启节点， 开始 PDO 传输
// 在使用 PDO 功能传输数据前， 务必发送管理报文报文开启节点。
// 以开启 2 号从站为例
// COBID DLC Message
// 000   02  01 02
int32_t kinco_fd122_pdo_enable(struct _device_manager *device)
{
	uint16_t motor_sid = 0x0;
	uint8_t data[] = {0x01, (uint8_t)device->minor, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	
	return kinco_can_write(motor_sid, data, 8, 2);
}

// 复位节点
// 以开启 2 号从站为例
// COBID DLC Message
// 000   02  81 02
int32_t kinco_fd122_node_rst(struct _device_manager *device)
{
	uint16_t motor_sid = 0x0;
	uint8_t data[] = {0x81, (uint8_t)device->minor, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	
	return kinco_can_write(motor_sid, data, 8, 2);
}

// 复位通信
// 以开启 2 号从站为例
// COBID DLC Message
// 000   02  82 02
int32_t kinco_fd122_comm_rst(struct _device_manager *device)
{
	uint16_t motor_sid = 0x0;
	uint8_t data[] = {0x82, (uint8_t)device->minor, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	
	return kinco_can_write(motor_sid, data, 8, 2);
}

/**
 * 控制电机速度
 * 0x2F 写一个字节， 0x2B 写两个字节
 * 0x27 写三个字节， 0x23 写四个字节
 * 0x60 写应答成功
 *
 * @param[in]	dev:
 * @param[in]	speed:
 * @param[out]	None.
 *
 * @return		0表示成功,其它返回失败
 */
int32_t kinco_fd122_speed_contorl(sr_device_id_t dev, int16_t speed)
{
	// CMD(1B) + 索引(2B) + 子索引(1B) + 数据(4B) 
	uint8_t data[8] = {0x23, 0xFF, 0x60, 0x00};
	int32_t tmp = 0;
	
	if (speed < -3000){
		speed = -3000;
	}
	else if (speed > 3000){
		speed = 3000;
	}
	
	tmp = MOTOR_KINCO_SPD_CONVER(speed);
	data[4] = (uint8_t)(tmp&0xFF);
	data[5] = (uint8_t)((tmp>>8)&0xFF);
	data[6] = (uint8_t)((tmp>>16)&0xFF);
	data[7] = (uint8_t)((tmp>>24)&0xFF);
		
	return kinco_can_write(*kinco_sid_heart_map(dev, SR_SID), data, sizeof(data), 2);
}

/**
* kinco使能/关闭电机
 *
 * @param[in]	pos: 电机位置
 * @param[in]	state: 使能或关闭
 * @param[out]	None
 *
 * @return		>=0成功，其他失败
 */
int32_t kinco_fd122_enable(sr_device_id_t dev, bool state)
{
	// 控制字 0x06:关闭电机, 0x0F:使能电机
	
	if (state){
		uint8_t data[] = {0x2B, 0x40, 0x60, 0x00, 0x0F, 0x00, 0x00, 0x00};
		return kinco_can_write(*kinco_sid_heart_map(dev, SR_SID), data, sizeof(data), 2);
		
	} else {
		uint8_t data[] = {0x2B, 0x40, 0x60, 0x00, 0x06, 0x00, 0x00, 0x00};
		return kinco_can_write(*kinco_sid_heart_map(dev, SR_SID), data, sizeof(data), 2);
	}
}

// 工作模式
int32_t kinco_fd122_mode_select(sr_device_id_t dev)
{
	uint8_t data[] = {0x2F, 0x60, 0x60, 0x00, 0xfd, 0x00, 0x00, 0x00};
	
	return kinco_can_write(*kinco_sid_heart_map(dev, SR_SID), data, sizeof(data), 2);
}

// 设置节点保护时间50ms,节点保护时间系数靠驱动器配置保证为3
int32_t kinco_fd122_node_protect_set(sr_device_id_t dev)
{
	uint8_t data[] = {0x2B, 0x0C, 0x10, 0x00, 50, 0x00, 0x00, 0x00};
	
	return kinco_can_write(*kinco_sid_heart_map(dev, SR_SID), data, sizeof(data), 2);
}

int32_t kinco_fd122_prohibited_event_time(sr_device_id_t dev)
{
	int32_t ret = 0;
	uint32_t can_id = *kinco_sid_heart_map(dev, SR_SID);
	
	// 设定PDO1禁止时间为0ms
	uint8_t pt1_data[8] = {0x2B, 0x0, 0x18, 0x03, 0, 0x00, 0x00, 0x00};
	ret = kinco_can_write(can_id, pt1_data, 8, 2);
	vTaskDelay(10*SYSTEM_TICK_MS);
	
	// 设定PDO2禁止时间为0ms
	uint8_t pt2_data[8] = {0x2B, 0x01, 0x18, 0x03, 0, 0x00, 0x00, 0x00};
	ret = kinco_can_write(can_id, pt2_data, 8, 2);
	vTaskDelay(10*SYSTEM_TICK_MS);
	
	// 设定PDO1事件时间为10ms
	uint8_t et1_data[8] = {0x2B, 0x00, 0x18, 0x05, 0x0a, 0x00, 0x00, 0x00};
	ret = kinco_can_write(can_id, et1_data, 8, 2);
	vTaskDelay(10*SYSTEM_TICK_MS);
	
	// 设定PDO2事件时间为20ms
	uint8_t et2_data[8] = {0x2B, 0x01, 0x18, 0x05, 0x14, 0x00, 0x00, 0x00};
	ret = kinco_can_write(can_id, et2_data, 8, 2);
	vTaskDelay(10*SYSTEM_TICK_MS);
	return ret;
}


/*
 * 开启通信中断模式 设置为1,当通信中断时，触发CAN故障
 * CAN 通讯中断模式， 决定驱动器在超过节点保护时间*节点保
 * 护系数的时间后仍然没有收到节点保护报文的动作逻辑
 * 0： 不处理
 * 1： 报错
 *
 * @param[in]	dev: 设备
 * @param[out]	None
 *
 * @return		>=0成功，其他失败
 */
int32_t kinco_fd122_comm_disconnect(sr_device_id_t dev)
{
	uint8_t data[8] = {0x2B, 0x07, 0x60, 0x00, 0x01, 0, 0x00, 0x00};
	return kinco_can_write(*kinco_sid_heart_map(dev, SR_SID), data, 8, 2);
}

int32_t kinco_emergency_stop(sr_device_id_t dev) 
{
	vTaskDelay(5*SYSTEM_TICK_MS);
	kinco_fd122_speed_contorl(dev, 0);
	vTaskDelay(5*SYSTEM_TICK_MS);
	kinco_fd122_enable(dev, DISABLE);

	return 0;
}

int32_t kinco_emergency_cancel(sr_device_id_t dev) 
{

	// Martix重启系统后，由于没有与VSC直接通信，当用户立即点击解除急停时，
	// 不能保证电机动力电已经恢复到48V，故暂时使用延时处理。
	vTaskDelay(5*SYSTEM_TICK_MS);
	kinco_fd122_err_clr(dev);
	vTaskDelay(10*SYSTEM_TICK_MS);
	kinco_fd122_speed_contorl(dev, 0);
	vTaskDelay(10*SYSTEM_TICK_MS);
	kinco_fd122_mode_select(dev);
	vTaskDelay(10*SYSTEM_TICK_MS);
	kinco_fd122_enable(dev, ENABLE);
	
	return 0;
}

static int32_t kinco_exceptions_cb(struct _device_manager *device, 
				sr_motor_msg_type_t type,uint8_t* data)
{
	static struct dev_status dev_state = {NULL,0};

	/* MOTOR_MSG_VEE 速度/错误码1/错误码2 */			
	dev_state.dev = device;
	memcpy(dev_state.data, data, 8);
	if ( xQueueSend(get_dev_status_handle(), (void *)&dev_state, 0) != pdPASS){	
		//sr_kprintf("failed dev status to xQueueSend, %s\r\n", __FUNCTION__);
		return -1;
	}
	
	return 0;
}
											

int32_t kinco_fd122_init(struct _device_manager *device, sr_device_id_t type,
								device_rx_cb cb)
{
	if (device){
		device->rx_cb = cb;
		device->msg_map = kinco_map;
		device->e_handler = kinco_exceptions_cb;
	}

	// Martix重启系统后，由于没有与VSC直接通信，当用户立即点击解除急停时，
	// 不能保证电机动力电已经恢复到48V，会导致错误码清除不了。	
	
	kinco_fd122_prohibited_event_time(type);
	vTaskDelay(20*SYSTEM_TICK_MS);
	
	kinco_fd122_speed_contorl(type, 0);
	vTaskDelay(20*SYSTEM_TICK_MS);
	
	kinco_fd122_mode_select(type);
	vTaskDelay(20*SYSTEM_TICK_MS);	
	
	kinco_fd122_node_protect_set(type);
	vTaskDelay(20*SYSTEM_TICK_MS);

	kinco_fd122_comm_disconnect(type);
	vTaskDelay(20*SYSTEM_TICK_MS);

	kinco_fd122_pdo_enable(device);
	vTaskDelay(10*SYSTEM_TICK_MS);
	
	kinco_fd122_err_clr(type);
	vTaskDelay(20*SYSTEM_TICK_MS);
	
	kinco_fd122_enable(type, ENABLE);
	vTaskDelay(20*SYSTEM_TICK_MS);	
	sr_kprintf("%s device %d\n", __FUNCTION__, type);
	return 0;
}



