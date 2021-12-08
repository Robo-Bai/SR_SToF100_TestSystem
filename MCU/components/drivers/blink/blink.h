#ifndef __BLINK_H
#define __BLINK_H

#include "stm32f10x.h"
#include "sys.h"


#define LED1_PORT 		GPIO_Pin_15
#define LED2_PORT 		GPIO_Pin_14
#define LED3_PORT 		GPIO_Pin_5

#define LED1_OFF() 		GPIO_SetBits(GPIOB, LED1_PORT)
#define LED1_ON() 		GPIO_ResetBits(GPIOB, LED1_PORT)

#define LED2_OFF() 		GPIO_SetBits(GPIOB, LED2_PORT)
#define LED2_ON() 		GPIO_ResetBits(GPIOB, LED2_PORT)

#define LED3_OFF() 		GPIO_SetBits(GPIOB, LED3_PORT)
#define LED3_ON() 		GPIO_ResetBits(GPIOB, LED3_PORT)

#define LED1 PBout(15)
#define LED2 PBout(14)
#define LED3 PBout(5)




typedef struct
{
	uint8_t 	CountEn; 
	uint16_t 	Count; 
	uint16_t 	CountTop;
	uint8_t 	TimeOn; 
}LedTimer_TypeDef;

void BlinkInit(void);
void ledTimer(void);
void ledTimer_led1_start(void);
void ledBlink_led1(uint16_t period);
void ledTimer_led2_start(void);
void ledBlink_led2(uint16_t period);
void ledTimer_led3_start(void);
void ledBlink_led3(uint16_t period);
#endif
