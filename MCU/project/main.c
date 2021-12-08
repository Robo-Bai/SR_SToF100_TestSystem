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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置NVIC中断优先级分组2:2位抢占优先级，2位响应优先级
	
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

