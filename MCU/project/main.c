#include "stm32f10x.h"
#include "sensor.h"
#include "delay.h"
#include "blink.h"
#include "exti.h"
#include "can_SToF100.h"
#include "motor_kinco.h"
#include "SH200.h"


int main(void)
{
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����NVIC�ж����ȼ�����2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	
	Sensor_LedTri_init();
	BlinkInit();
	EXTI_Sensor();
	LED_YELLOW=1;
	

	CAN_Config();
	
	while(1)
	{
		LED2_ON();
	}
}

