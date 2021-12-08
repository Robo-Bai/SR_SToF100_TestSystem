/**
 * @file sensor.c
 * @author BJW
 * @version V1.0.0
 * @date 2021/04/21
 * @brief ��紫�����ⲿ�ж�
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
	Sensor_LedTri_init();//����������ɫ��IO��ʼ��
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//ʹ�ܸ���ʱ��
	
	/*---------------------B1----------------
	*������B1��X������λEU1I2-EU1I2������0V���ڵ�24V
	*_IO_INPUT_PIN_2--GPIOC_Pin_10
	*BJW--2021/04/15-16:31
	*---------------------------------------
	*/
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource10);
	EXTI_InitStructure.EXTI_Line=EXTI_Line10;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	//EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//�����ش���
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//˫���ش�����LJ��2021/04/15-15:21������can.c��97-110��ע�͵���ok��
	EXTI_Init(&EXTI_InitStructure);	  	//��ʼ������EXTI

	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			    //ʹ��B1������ڵ��ⲿ�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;				//�����ȼ�0 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;							//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);  	  //��ʼ������NVIC
	
	/*---------------------B2----------------
	 *������B2��X�Ḻ��λEU1I1���ӹ�磩--EU1I3����EU100��������0V���ڵ�24V
	 *_IO_INPUT_PIN_3--GPIOC_Pin11
	 *BJW--2021/04/15-16:36
	 *---------------------------------------
	 */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource11);
	EXTI_InitStructure.EXTI_Line=EXTI_Line11;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	//EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//�����ش���
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//˫���ش�����LJ��2021/04/15-15:21������can.c��97-110��ע�͵���ok��
	EXTI_Init(&EXTI_InitStructure);	  	//��ʼ������EXTI

	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			    //ʹ��B2������ڵ��ⲿ�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;				//�����ȼ�0 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;							//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);  	  //��ʼ������NVIC
	
	
	/*---------------------B3----------------
	 *������B3��EU1I4��Z������λ���ϣ�������0V���ڵ�24V
	 *_IO_INPUT_PIN_4--GPIOC_Pin_12
	 *BJW--2021/04/15-16:18
	 *---------------------------------------
	*/
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource12);
	EXTI_InitStructure.EXTI_Line=EXTI_Line12;	
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	//EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//�����ش���
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//˫���ش���
	EXTI_Init(&EXTI_InitStructure);	  	//��ʼ������EXTI

	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			    //ʹ��B3������ڵ��ⲿ�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;				//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;							//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);  	  //��ʼ������NVIC
	 
	 /*---------------------B4----------------
	 *������B4��EU1I5--Z�Ḻ��λ���£�������0V���ڵ�24V
	 *_IO_INPUT_PIN_5--GPIOC_Pin_13
	 *BJW--2021/04/15-16:05
	 *---------------------------------------
	 */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource13);
	EXTI_InitStructure.EXTI_Line=EXTI_Line13;	
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	//EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//�����ش���
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//˫���ش���
	EXTI_Init(&EXTI_InitStructure);	  	//��ʼ������EXTI

	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			    //ʹ��B4������ڵ��ⲿ�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;				//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;							//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);  	  //��ʼ������NVIC
	
	/*---------------------B5----------------
	*������B5��X��ԭ��EU1I3���ӹ��_�Ӵ�=EU1I1����EU100��������ʱ24V���ڵ�0V
	*_IO_INPUT_PIN_1--GPIOC_Pin_9
	*BJW--2021/04/15-16:28
	*---------------------------------------
	*/
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource9);
	EXTI_InitStructure.EXTI_Line=EXTI_Line9;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	//EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//�����ش���
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//˫���ش���
	EXTI_Init(&EXTI_InitStructure);	  	//��ʼ������EXTI

	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			      //ʹ��B5������ڵ��ⲿ�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;				//�����ȼ�0 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;							//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);  	  //��ʼ������NVIC
	
	/*---------------------B6----------------
	*������B6��R��ԭ�㿪�أ�����ʱ24V���ڵ�0V
	*_IO_INPUT_PIN_6--GPIOD_Pin_2
	*BJW--2021/05/21-18:04
	*---------------------------------------
	*/
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD,GPIO_PinSource2);
	EXTI_InitStructure.EXTI_Line=EXTI_Line2;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	//EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//�����ش���
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//˫���ش���
	EXTI_Init(&EXTI_InitStructure);	  	//��ʼ������EXTI

	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;			      //ʹ��B6������ڵ��ⲿ�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;				//�����ȼ�0 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;							//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);  	  //��ʼ������NVIC
}


extern unsigned char SH200_INconfig_flag;

extern unsigned char LED_X_Origin_flag;      //X���ԭ��ɹ���־λ
extern unsigned char LED_Z_Origin_flag;      //Z���ԭ��ɹ���־λ
extern unsigned char LED_R_Origin_flag;      //R���ԭ��ɹ���־λ

extern unsigned char LED_X_backlimit_flag;   //X�����������λ��־λ
extern unsigned char LED_X_forwardlimit_flag;//X��������ǰ����λ��־λ
extern unsigned char LED_Z_bottomlimit_flag; //Z������λ��־λ


void EXTI15_10_IRQHandler(void)
{
	/*-------------B1��X������λ�ⲿ�жϺ���----------------
	 *1.LED_X_backlimit_flag��1 //X�����������λ��־λ
	 *2.���������
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
	
	/*-------------B2��X�Ḻ��λ�ⲿ�жϺ���----------------
	*1.�̵Ʒ�ת
	*2.����λ�ر�X����ʹ��
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
	
	
	
	/*-------------B3��Z������λ�ⲿ�жϺ���----------------
	 *1.Z���ԭ���־λ��1
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
	
	/*-------------B4��Z�Ḻ��λ�ⲿ�жϺ���----------------
	 *1.���еĵƷ�ת
     *2.�ر�Z����ʹ��
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
	/*-------------B5��X��ԭ���ⲿ�жϺ���----------------
	 *1.X���ԭ���־λ��1
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
	/*-------------B6��R��ԭ���ⲿ�жϺ���----------------
	*1.R���ԭ���־λ��1
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

