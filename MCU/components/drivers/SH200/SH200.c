/**
 * @file SH200.c
 * @author BJW
 * @version V1.0.0
 * @date 2021/04/25
 * @brief SH200驱动配置，包括：进入配置/使能主动上报/退出配置/激活设备
 */
 
 
#include "SH200.h"
#include "sys.h"
#include "string.h"

/*
 *进入配置模式
 */
int32_t SH200_INconfig(uint32_t SH200_ID)
{
	CanTxMsg TxMessage;
	uint8_t mbox = 0;
	uint32_t i = 0;
	uint8_t data[8] = {0x2f,0x01,0x20,0x01,0x01,0x00,0x00,0x00};

	TxMessage.StdId = SH200_ID;
	TxMessage.RTR = CAN_RTR_DATA; 
	TxMessage.IDE = CAN_ID_STD;  
	TxMessage.DLC = 8;  

	memcpy(TxMessage.Data, data, 8);
	mbox = CAN_Transmit(CAN1, &TxMessage);

	while((CAN_TransmitStatus(CAN1, mbox) != CAN_TxStatus_Ok) && (++i<0x0FFF));
	return 0;
}


/*
 *使能主动上报，设置上报周期为25ms--HEX：0x19
 */
int32_t SH200_transmit(uint32_t SH200_ID)
{
	CanTxMsg TxMessage;
	uint8_t mbox = 0;
	uint32_t i = 0;
	uint8_t data[8] = {0x27,0x01,0x20,0x08,0x01,0x3C,0x00,0x00};

	TxMessage.StdId = SH200_ID;
	TxMessage.RTR = CAN_RTR_DATA; 
	TxMessage.IDE = CAN_ID_STD;  
	TxMessage.DLC = 8;  

	memcpy(TxMessage.Data, data, 8);
	mbox = CAN_Transmit(CAN1, &TxMessage);

	while((CAN_TransmitStatus(CAN1, mbox) != CAN_TxStatus_Ok) && (++i<0x0FFF));
	return 0;
}


/*
 *退出配置模式
 */
int32_t SH200_OUTconfig(uint32_t SH200_ID)
{
	CanTxMsg TxMessage;
	uint8_t mbox = 0;
	uint32_t i = 0;
	uint8_t data[8] = {0x2f,0x01,0x20,0x02,0x02,0x00,0x00,0x00};

	TxMessage.StdId = SH200_ID;
	TxMessage.RTR = CAN_RTR_DATA; 
	TxMessage.IDE = CAN_ID_STD;  
	TxMessage.DLC = 8;  

	memcpy(TxMessage.Data, data, 8);
	mbox = CAN_Transmit(CAN1, &TxMessage);

	while((CAN_TransmitStatus(CAN1, mbox) != CAN_TxStatus_Ok) && (++i<0x0FFF));
	return 0;
}


/*
 *激活设备
 */
int32_t SH200_activate(uint32_t SH200_ID)
{
	CanTxMsg TxMessage;
	uint8_t mbox = 0;
	uint32_t i = 0;
	uint8_t data[8] = {0x2f,0x11,0x20,0x01,0x01,0x00,0x00,0x00};

	TxMessage.StdId = SH200_ID;
	TxMessage.RTR = CAN_RTR_DATA; 
	TxMessage.IDE = CAN_ID_STD;  
	TxMessage.DLC = 8;  

	memcpy(TxMessage.Data, data, 8);
	mbox = CAN_Transmit(CAN1, &TxMessage);

	while((CAN_TransmitStatus(CAN1, mbox) != CAN_TxStatus_Ok) && (++i<0x0FFF));
	return 0;
}


unsigned char SH200_INconfig_flag = 0; //SH200进入配置成功标志位
unsigned char SH200_transmit_flag = 0;  //SH200使能主动上报成功标志位
unsigned char SH200_OUTconfig_flag = 0;  //SH200退出配置成功标志位
unsigned char SH200_activate_flag = 0;  //SH200激活成功标志位

//启动SH200
void SH200_active()
{
	if(SH200_INconfig_flag == 1)
	{
		SH200_INconfig(0x411);
		SH200_INconfig_flag = 0;
	}else if(SH200_transmit_flag == 1)
	{
		SH200_transmit(0x411);
		SH200_transmit_flag = 0;
	}else if(SH200_OUTconfig_flag == 1)
	{
		SH200_OUTconfig(0x411);
		SH200_OUTconfig_flag = 0;
	}else if(SH200_activate_flag == 1)
	{
		SH200_activate(0x411);
		SH200_activate_flag = 0;
	}
}


