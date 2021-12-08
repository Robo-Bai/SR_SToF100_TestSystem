#ifndef __EXTI_H
#define __EXTI_H	

#include "sys.h"

   	 
#define SENSOR_B5 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9)    //������B5��X��ԭ��EU1I3���ӹ��_�Ӵ�=EU1I1����EU100��������ʱ24V���ڵ�0V
#define SENSOR_B1 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_10)   //������B1��X������λEU1I2-EU1I2������0V���ڵ�24V
#define SENSOR_B2 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_11)   //������B2��X�Ḻ��λEU1I1���ӹ�磩--EU1I3����EU100��������0V���ڵ�24V
#define SENSOR_B3 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_12)   //������B3��EU1I4��Z������λ���ϣ�������0V���ڵ�24V
#define SENSOR_B4 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)   //������B4��EU1I5--Z�Ḻ��λ���£�������0V���ڵ�24V
#define SENSOR_B6 GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_2)    //������B6��R����תԭ�㿪�أ����ã���������0V���ڵ�24V


void EXTI_Sensor(void);//�ⲿ�жϳ�ʼ��
void motor_active(uint32_t Motor_ID);//�������
//void SH200_active(void);//SH200����

#endif


