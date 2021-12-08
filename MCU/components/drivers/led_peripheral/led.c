#include "led.h"
#include "sensor.h"
#include "stm32f10x.h"
#include "sys.h"
#include "exti.h"
#include "can_SToF100.h"
#include "delay.h"

unsigned char LED_X_Origin_flag = 0;  //X���ԭ��ɹ���־λ
unsigned char LED_Z_Origin_flag = 0;  //Z���ԭ��ɹ���־λ
unsigned char LED_R_Origin_flag = 0;  //R���ԭ��ɹ���־λ

unsigned char LED_X_backlimit_flag    = 0;  //X�����������λ��־λ
unsigned char LED_X_forwardlimit_flag = 0;  //X��������ǰ����λ��־λ
unsigned char LED_Z_bottomlimit_flag  = 0;  //Z������λ��־λ



void LED_active()
{
	int num_1;
//	delay_init();//���������ѳ�ʼ��
	Sensor_LedTri_init();
	if ((LED_X_Origin_flag == 1)&&(LED_Z_Origin_flag == 1)&&(LED_R_Origin_flag == 1))
		{
			for(num_1 = 0; num_1 < 3; num_1++)
			{
				delay_ms(500);
				LED_GREEN = !LED_GREEN;
				delay_ms(500);
				LED_GREEN = !LED_GREEN;		
			}
			LED_X_Origin_flag = 0;
			LED_Z_Origin_flag = 0;
			LED_R_Origin_flag = 0;			
		}
}

void LED_limit_active()
{
	int num_2;
	Sensor_LedTri_init();
	if ((LED_X_backlimit_flag == 1)||(LED_X_forwardlimit_flag == 1)||(LED_Z_bottomlimit_flag == 1))
	{
		for(num_2 = 0; num_2 < 3; num_2++)
		{
			delay_ms(250);
			LED_RED = !LED_RED;
			delay_ms(250);
			LED_RED = !LED_RED;		
		}
		LED_X_backlimit_flag    = 0;
		LED_X_forwardlimit_flag = 0;
		LED_Z_bottomlimit_flag  = 0;
	}
}


//void LED_Init()
//{
// /*-------------- Output Configuration ----------------*/
//	  //DO��LED��ɫ��
//		GPIO_InitTypeDef  GPIO_InitStruction;
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);//ʹ��GPIOCʱ��
//    //DO����
//    GPIO_InitStruction.GPIO_Pin   = _IO_OUTPUT_PIN_1 | _IO_OUTPUT_PIN_2 | _IO_OUTPUT_PIN_3 | _IO_OUTPUT_PIN_4 | _IO_OUTPUT_PIN_5 | _IO_OUTPUT_PIN_6;
//    GPIO_InitStruction.GPIO_Mode  = GPIO_Mode_Out_PP;
//    GPIO_InitStruction.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_Init(GPIOC, &GPIO_InitStruction);
//}

