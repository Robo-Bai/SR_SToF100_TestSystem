/**
 * @file sensor.c
 * @author BJW
 * @version V1.0.0
 * @date 2021/04/21
 * @brief 光电传感器外部中断
 */

#include "sensor.h"
#include "exti.h"
#include "stm32f10x.h"
#include "sys.h"
#include "can_SToF100.h"
#include "motor_kinco.h"
#include "SH200.h"
#include "led.h"

void EXTI_Sensor(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	Sensor_LedTri_init();//传感器与三色灯IO初始化
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//使能复用时钟
	
	/*---------------------B1----------------
	*触发，B1：X轴正限位EU1I2-EU1I2：不挡0V，遮挡24V
	*_IO_INPUT_PIN_2--GPIOC_Pin_10
	*BJW--2021/04/15-16:31
	*---------------------------------------
	*/
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource10);
	EXTI_InitStructure.EXTI_Line=EXTI_Line10;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	//EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//上升沿触发
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//双边沿触发（LJ改2021/04/15-15:21，发现can.c的97-110行注释掉后ok）
	EXTI_Init(&EXTI_InitStructure);	  	//初始化外设EXTI

	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			    //使能B1光电所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;				//子优先级0 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;							//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);  	  //初始化外设NVIC
	
	/*---------------------B2----------------
	 *触发，B2：X轴负限位EU1I1（接光电）--EU1I3（接EU100）：不挡0V，遮挡24V
	 *_IO_INPUT_PIN_3--GPIOC_Pin11
	 *BJW--2021/04/15-16:36
	 *---------------------------------------
	 */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource11);
	EXTI_InitStructure.EXTI_Line=EXTI_Line11;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	//EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//上升沿触发
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//双边沿触发（LJ改2021/04/15-15:21，发现can.c的97-110行注释掉后ok）
	EXTI_Init(&EXTI_InitStructure);	  	//初始化外设EXTI

	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			    //使能B2光电所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;				//子优先级0 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;							//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);  	  //初始化外设NVIC
	
	
	/*---------------------B3----------------
	 *触发，B3：EU1I4：Z轴正限位（上）：不挡0V，遮挡24V
	 *_IO_INPUT_PIN_4--GPIOC_Pin_12
	 *BJW--2021/04/15-16:18
	 *---------------------------------------
	*/
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource12);
	EXTI_InitStructure.EXTI_Line=EXTI_Line12;	
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	//EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//上升沿触发
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//双边沿触发
	EXTI_Init(&EXTI_InitStructure);	  	//初始化外设EXTI

	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			    //使能B3光电所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;				//子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;							//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);  	  //初始化外设NVIC
	 
	 /*---------------------B4----------------
	 *触发，B4：EU1I5--Z轴负限位（下）：不挡0V，遮挡24V
	 *_IO_INPUT_PIN_5--GPIOC_Pin_13
	 *BJW--2021/04/15-16:05
	 *---------------------------------------
	 */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource13);
	EXTI_InitStructure.EXTI_Line=EXTI_Line13;	
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	//EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//上升沿触发
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//双边沿触发
	EXTI_Init(&EXTI_InitStructure);	  	//初始化外设EXTI

	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			    //使能B4光电所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;				//子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;							//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);  	  //初始化外设NVIC
	
	/*---------------------B5----------------
	*触发，B5：X轴原点EU1I3（接光电_接错）=EU1I1（接EU100）：不挡时24V，遮挡0V
	*_IO_INPUT_PIN_1--GPIOC_Pin_9
	*BJW--2021/04/15-16:28
	*---------------------------------------
	*/
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource9);
	EXTI_InitStructure.EXTI_Line=EXTI_Line9;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	//EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//上升沿触发
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//双边沿触发
	EXTI_Init(&EXTI_InitStructure);	  	//初始化外设EXTI

	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			      //使能B5光电所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;				//子优先级0 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;							//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);  	  //初始化外设NVIC
	
	/*---------------------B6----------------
	*触发，B6：R轴原点开关：不挡时24V，遮挡0V
	*_IO_INPUT_PIN_6--GPIOD_Pin_2
	*BJW--2021/05/21-18:04
	*---------------------------------------
	*/
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD,GPIO_PinSource2);
	EXTI_InitStructure.EXTI_Line=EXTI_Line2;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	//EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//上升沿触发
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//双边沿触发
	EXTI_Init(&EXTI_InitStructure);	  	//初始化外设EXTI

	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;			      //使能B6光电所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;				//子优先级0 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;							//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);  	  //初始化外设NVIC
}


extern unsigned char SH200_INconfig_flag;

extern unsigned char LED_X_Origin_flag;      //X轴回原点成功标志位
extern unsigned char LED_Z_Origin_flag;      //Z轴回原点成功标志位
extern unsigned char LED_R_Origin_flag;      //R轴回原点成功标志位

extern unsigned char LED_X_backlimit_flag;   //X轴正（向后）限位标志位
extern unsigned char LED_X_forwardlimit_flag;//X轴正（向前）限位标志位
extern unsigned char LED_Z_bottomlimit_flag; //Z轴下限位标志位


void EXTI15_10_IRQHandler(void)
{
	/*-------------B1：X轴正限位外部中断函数----------------
	 *1.LED_X_backlimit_flag置1 //X轴正（向后）限位标志位
	 *2.红灯闪三下
	 *--------------------------------------------------
	*/
	if(EXTI_GetITStatus(EXTI_Line10) != RESET)
	{
		if(SENSOR_B1==0)
		{
			LED_X_backlimit_flag = 1;
			LED_limit_active();
		}
		EXTI_ClearITPendingBit(EXTI_Line10);
	}  
	
	/*-------------B2：X轴负限位外部中断函数----------------
	*1.绿灯翻转
	*2.负限位关闭X轴电机使能
	*--------------------------------------------------
	*/
	if(EXTI_GetITStatus(EXTI_Line11) != RESET)
	{
		if(SENSOR_B2==0)
		{
			LED_X_forwardlimit_flag = 1;
			LED_limit_active();
		}
		EXTI_ClearITPendingBit(EXTI_Line11);
	}  
	
	
	
	/*-------------B3：Z轴正限位外部中断函数----------------
	 *1.Z轴回原点标志位置1
	 *2.
	 */
	if(EXTI_GetITStatus(EXTI_Line12) != RESET)
	{
		if(SENSOR_B3==0)
		{
			LED_Z_Origin_flag = 1;
			LED_active();
		}
		EXTI_ClearITPendingBit(EXTI_Line12);
	}
	
	/*-------------B4：Z轴负限位外部中断函数----------------
	 *1.所有的灯翻转
     *2.关闭Z轴电机使能
     *3.
	 */
	if(EXTI_GetITStatus(EXTI_Line13) != RESET)
	{
		if(SENSOR_B4==0)
		{
			LED_Z_bottomlimit_flag = 1;
			LED_limit_active();
		}
		EXTI_ClearITPendingBit(EXTI_Line13);
	}
}

void EXTI9_5_IRQHandler(void)
{
	/*-------------B5：X轴原点外部中断函数----------------
	 *1.X轴回原点标志位置1
	 *--------------------------------------------------
	*/
	if(EXTI_GetITStatus(EXTI_Line9) != RESET)
	{
		if(SENSOR_B5==1)
		{
			LED_X_Origin_flag = 1;
			LED_active();
		}
		EXTI_ClearITPendingBit(EXTI_Line9);
	}
}

void EXTI2_IRQHandler(void)
	/*-------------B6：R轴原点外部中断函数----------------
	*1.R轴回原点标志位置1
	*--------------------------------------------------
	*/
{
	if(EXTI_GetITStatus(EXTI_Line2) != RESET)
	{
		if(SENSOR_B6==0)
		{
			LED_R_Origin_flag = 1;
			LED_active();
		}
	EXTI_ClearITPendingBit(EXTI_Line2);
	}
}

