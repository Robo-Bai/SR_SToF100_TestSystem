#ifndef __EXTI_H
#define __EXTI_H	

#include "sys.h"

   	 
#define SENSOR_B5 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9)    //触发，B5：X轴原点EU1I3（接光电_接错）=EU1I1（接EU100）：不挡时24V，遮挡0V
#define SENSOR_B1 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_10)   //触发，B1：X轴正限位EU1I2-EU1I2：不挡0V，遮挡24V
#define SENSOR_B2 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_11)   //触发，B2：X轴负限位EU1I1（接光电）--EU1I3（接EU100）：不挡0V，遮挡24V
#define SENSOR_B3 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_12)   //触发，B3：EU1I4：Z轴正限位（上）：不挡0V，遮挡24V
#define SENSOR_B4 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)   //触发，B4：EU1I5--Z轴负限位（下）：不挡0V，遮挡24V
#define SENSOR_B6 GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_2)    //触发，B6：R轴旋转原点开关（内置）：？不挡0V，遮挡24V


void EXTI_Sensor(void);//外部中断初始化
void motor_active(uint32_t Motor_ID);//电机动作
//void SH200_active(void);//SH200动作

#endif


