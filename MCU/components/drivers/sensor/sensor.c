/**
 * @file sensor.c
 * @author BJW
 * @version V1.0.0
 * @date 2021/04/20
 * @brief 光电传感器
 */



#include "sensor.h"
#include "stm32f10x.h"
#include "sys.h"
#include "sensor.h"


void Sensor_LedTri_init()
{
    GPIO_InitTypeDef GPIO_InitStruction;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);
    
    /*-------------- Input Configuration ----------------*/
    GPIO_InitStruction.GPIO_Pin   = _IO_INPUT_PIN_1 | _IO_INPUT_PIN_2 | _IO_INPUT_PIN_3 |\
                                    _IO_INPUT_PIN_4 | _IO_INPUT_PIN_5;//DI：X轴和Z轴光电传感器DI
    GPIO_InitStruction.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	 // GPIO_InitStruction.GPIO_Mode  = GPIO_Mode_IPU;//上拉输入2021/04/15-14:00
    GPIO_InitStruction.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStruction);

    GPIO_InitStruction.GPIO_Pin = _IO_INPUT_PIN_6;//DI：R轴光电传感器DI
    GPIO_Init(GPIOD, &GPIO_InitStruction);

    /*-------------- Output Configuration ----------------*/
    GPIO_InitStruction.GPIO_Pin   = _IO_OUTPUT_PIN_1 | _IO_OUTPUT_PIN_2 | _IO_OUTPUT_PIN_3 |\
                                    _IO_OUTPUT_PIN_4 | _IO_OUTPUT_PIN_5 | _IO_OUTPUT_PIN_6;//DO：LED三色灯
    GPIO_InitStruction.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStruction.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStruction);
}





