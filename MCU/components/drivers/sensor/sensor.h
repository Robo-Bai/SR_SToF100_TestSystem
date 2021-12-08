#ifndef __SENSOR_H
#define __SENSOR_H

/*-------------------- Output IO define -----------------------*/
#define _IO_OUTPUT_PIN_1                GPIO_Pin_0
#define _IO_OUTPUT_PIN_2                GPIO_Pin_1
#define _IO_OUTPUT_PIN_3                GPIO_Pin_2
#define _IO_OUTPUT_PIN_4                GPIO_Pin_3
#define _IO_OUTPUT_PIN_5                GPIO_Pin_4
#define _IO_OUTPUT_PIN_6                GPIO_Pin_5


/*-------------------- Input IO define -----------------------*/
#define _IO_INPUT_PIN_1                GPIO_Pin_9
#define _IO_INPUT_PIN_2                GPIO_Pin_10
#define _IO_INPUT_PIN_3                GPIO_Pin_11
#define _IO_INPUT_PIN_4                GPIO_Pin_12
#define _IO_INPUT_PIN_5                GPIO_Pin_13
#define _IO_INPUT_PIN_6                GPIO_Pin_2


//#define LED_RED  PCout(0)

void Sensor_LedTri_init(void);


#endif
