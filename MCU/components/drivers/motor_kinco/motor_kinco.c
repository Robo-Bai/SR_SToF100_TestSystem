/**
 * @file motor_kinco.c
 * @author BJW
 * @version V1.0.0
 * @date 2021/04/22
 * @brief kinco������-������ã�������ʹ��/�ر�/ģʽѡ��/�ٶ�
 */

#include "motor_kinco.h"
#include "sys.h"
#include "string.h"
#include "exti.h"
//#include "stdbool.h"


//ʹ�ܵ��
int32_t kinco_enable(uint32_t motor_kinco_ID)
{
	//  if(state == 1){
	CanTxMsg TxMessage;
	uint8_t mbox = 0;
	uint32_t i = 0;
	uint8_t data[8] = {0x2B,0x40,0x60,0x00,0x0F,0x00,0x00,0x00};

	TxMessage.StdId = motor_kinco_ID;
	TxMessage.RTR = CAN_RTR_DATA; 
	TxMessage.IDE = CAN_ID_STD;  
	TxMessage.DLC = 8;  

	memcpy(TxMessage.Data, data, 8);
	mbox = CAN_Transmit(CAN1, &TxMessage);

	while((CAN_TransmitStatus(CAN1, mbox) != CAN_TxStatus_Ok) && (++i<0x0FFF));
	return 0;
}


//�ر�ʹ��
int32_t kinco_disable(uint32_t motor_kinco_ID)
{
	CanTxMsg TxMessage;
	uint8_t mbox = 0;
	uint32_t i = 0;
	uint8_t data[8] = {0x2B,0x40,0x60,0x00,0x06,0x00,0x00,0x00};

	TxMessage.StdId = motor_kinco_ID;
	TxMessage.RTR = CAN_RTR_DATA; 
	TxMessage.IDE = CAN_ID_STD;  
	TxMessage.DLC = 8;  

	memcpy(TxMessage.Data, data, 8);
	mbox = CAN_Transmit(CAN1, &TxMessage);

	while((CAN_TransmitStatus(CAN1, mbox) != CAN_TxStatus_Ok) && (++i<0x0FFF));
	return 0;
}

//ѡ��ģʽ---3.�ٶ�ģʽ
int32_t kinco_motor_mode(uint32_t motor_kinco_ID)
{
	CanTxMsg TxMessage;
	uint8_t mbox = 0;
	uint32_t i = 0;
	uint8_t data[8] = {0x2F,0x60,0x60,0x00,0x03,0x00,0x00,0x00};

	TxMessage.StdId = motor_kinco_ID;
	TxMessage.RTR = CAN_RTR_DATA; 
	TxMessage.IDE = CAN_ID_STD;  
	TxMessage.DLC = 8;  

	memcpy(TxMessage.Data, data, 8);
	mbox = CAN_Transmit(CAN1, &TxMessage);

	while((CAN_TransmitStatus(CAN1, mbox) != CAN_TxStatus_Ok) && (++i<0x0FFF));
	return 0;
}


//����������ٶ�5rpm
int32_t kinco_motor_speed(uint32_t motor_kinco_ID)
{
	CanTxMsg TxMessage;
	uint8_t mbox = 0;
	uint32_t i = 0;
	uint8_t data[8] = {0x23,0xFF,0x60,0x00,0x86,0x5D,0x01,0x00};

	TxMessage.StdId = motor_kinco_ID;
	TxMessage.RTR = CAN_RTR_DATA; 
	TxMessage.IDE = CAN_ID_STD;  
	TxMessage.DLC = 8;  

	memcpy(TxMessage.Data, data, 8);
	mbox = CAN_Transmit(CAN1, &TxMessage);

	while((CAN_TransmitStatus(CAN1, mbox) != CAN_TxStatus_Ok) && (++i<0x0FFF));
	return 0;
}

unsigned char kinco_enable_flag = 0; //ʹ�ܱ�־λ
unsigned char kinco_disable_flag = 0;  //�ر�ʹ�ܱ�־λ
unsigned char kinco_motor_speed_flag = 0;  //����ٶȸ�ֵ��־λ
unsigned char kinco_motor_mode_flag = 0;  //���ģʽ��ֵ��־λ
 

//�������
void motor_active(uint32_t Motor_ID)
{
	switch(Motor_ID){
		case 0x601:{
		if( kinco_enable_flag == 1)
		{
			kinco_enable(0x601);
			kinco_enable_flag = 0;
		}else if(kinco_disable_flag == 1)
		{
			kinco_disable(0x601);
			kinco_disable_flag = 0;
		}else if(kinco_motor_speed_flag == 1)
		{
			kinco_motor_speed(0x601);
			kinco_motor_speed_flag = 0;		
		}else if(kinco_motor_mode_flag == 1)
			{
				kinco_motor_mode(0x601);
				kinco_motor_mode_flag = 0;		
			}
		}
		
		case 0x602:{
			if( kinco_enable_flag == 1)
			{
			kinco_enable(0x602);
			kinco_enable_flag = 0;
			}else if(kinco_disable_flag == 1)
			{
			kinco_disable(0x602);
			kinco_disable_flag = 0;
			}else if(kinco_motor_speed_flag == 1)
			{
			kinco_motor_speed(0x602);
			kinco_motor_speed_flag = 0;		
			}else if(kinco_motor_mode_flag == 1)
			{
			kinco_motor_mode(0x602);
			kinco_motor_mode_flag = 0;	
			}
		}		
	}
}
