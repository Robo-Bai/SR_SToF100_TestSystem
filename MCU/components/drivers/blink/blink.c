/*
 *@Brif: file is about LED status and system watching LED configuration
 *@Author: V
 *@Date: 2017-11-09
 ------------------------------------------------*/
 
#include "blink.h"
LedTimer_TypeDef ledTimer_led1,ledTimer_led2,ledTimer_led3;
/**
 * @brief: Initialize the LED GPIO
 * @param: None
 * @return: None
 */
void BlinkInit(void)
{
	GPIO_InitTypeDef	GPIO_InitStruction;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitStruction.GPIO_Pin = LED1_PORT | LED2_PORT | LED3_PORT;
	GPIO_InitStruction.GPIO_Mode	= GPIO_Mode_Out_PP;
	GPIO_InitStruction.GPIO_Speed	= GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruction);
	
	//init off
	LED1_OFF();
	LED2_OFF();
	LED3_OFF();
}

void ledTimer(void){
	if(ledTimer_led1.CountEn == 1){
		ledTimer_led1.Count++;
		if(ledTimer_led1.Count >= ledTimer_led1.CountTop){
			ledTimer_led1.TimeOn = 1;
			ledTimer_led1.CountEn = 0;
		}
	}
	if(ledTimer_led2.CountEn == 1){
		ledTimer_led2.Count++;
		if(ledTimer_led2.Count >= ledTimer_led2.CountTop){
			ledTimer_led2.TimeOn = 1;
			ledTimer_led2.CountEn = 0;
		}
	}
	if(ledTimer_led3.CountEn == 1){
		ledTimer_led3.Count++;
		if(ledTimer_led3.Count >= ledTimer_led3.CountTop){
			ledTimer_led3.TimeOn = 1;
			ledTimer_led3.CountEn = 0;
		}
	}
}
void ledTimer_led1_start(void){
	ledTimer_led1.CountEn = 1;
	ledTimer_led1.TimeOn = 0;
	ledTimer_led1.Count = 0;
}
void ledTimer_led2_start(void){
	ledTimer_led2.CountEn = 1;
	ledTimer_led2.TimeOn = 0;
	ledTimer_led2.Count = 0;
}
void ledTimer_led3_start(void){
	ledTimer_led3.CountEn = 1;
	ledTimer_led3.TimeOn = 0;
	ledTimer_led3.Count = 0;
}
void ledBlink_led1(uint16_t period){
	ledTimer_led1.CountTop = period;
	if(ledTimer_led1.TimeOn == 1){
		LED1 = ~LED1;
		ledTimer_led1.CountEn = 1;
		ledTimer_led1.Count = 0;
		ledTimer_led1.TimeOn = 0;
	}

}
void ledBlink_led2(uint16_t period){
	ledTimer_led2.CountTop = period;
	if(ledTimer_led2.TimeOn == 1){
		LED2 = ~LED2;
		ledTimer_led2.CountEn = 1;
		ledTimer_led2.Count = 0;
		ledTimer_led2.TimeOn = 0;
	}

}
void ledBlink_led3(uint16_t period){
	ledTimer_led3.CountTop = period;
	if(ledTimer_led3.TimeOn == 1){
		LED3 = ~LED3;
		ledTimer_led3.CountEn = 1;
		ledTimer_led3.Count = 0;
		ledTimer_led3.TimeOn = 0;
	}

}


/*-------------------------------------- END LINE -----------------------------------------*/

