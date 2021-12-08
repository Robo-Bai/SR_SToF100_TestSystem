/**
 * Copyright (C) 2019 Standard-robots, Inc
 *
 * @file 	adc.c
 * @author	Neethan <lijun@standard-robots.com> / <neethan@foxmail.com>
 * @version V1.0.0
 * @date 	2020年 2月 12日 星期三 10:50:47 CST
 *
 * @brief    misc driver adc 
 */

#include "drivers\include\drivers\adc.h"


#define SR_ADC_TEMP_GPIO_PORT		GPIOA
#define SR_ADC_TEMP_GPIO_CLK       (RCC_AHB1Periph_GPIOA) 
#define SR_ADC_TEMP_GPIO_PIN       GPIO_Pin_0

#define SR_ADC_PWR_3V3_GPIO_PORT	GPIOA
#define SR_ADC_PWR_3V3_GPIO_CLK     (RCC_AHB1Periph_GPIOA) 
#define SR_ADC_PWR_3V3_GPIO_PIN     GPIO_Pin_1

#define SR_ADC_PWR_GPIO_PORT		GPIOA
#define SR_ADC_PWR_GPIO_CLK       	(RCC_AHB1Periph_GPIOA) 
#define SR_ADC_PWR_GPIO_PIN       	GPIO_Pin_2

#define SR_ADC_PWR_5V_GPIO_PORT		GPIOA
#define SR_ADC_PWR_5V_GPIO_CLK      (RCC_AHB1Periph_GPIOA) 
#define SR_ADC_PWR_5V_GPIO_PIN      GPIO_Pin_3

#define SR_ADC_PWR_AIN_GPIO_PORT	GPIOA
#define SR_ADC_PWR_AIN_GPIO_CLK      (RCC_AHB1Periph_GPIOA) 
#define SR_ADC_PWR_AIN_GPIO_PIN      GPIO_Pin_4

#define SR_ADC_TEMP_CH				ADC_Channel_0
#define SR_ADC_PWR_3V3_CH			ADC_Channel_1
#define SR_ADC_PWR_CH				ADC_Channel_2
#define SR_ADC_PWR_5V_CH			ADC_Channel_3
#define SR_ADC_PWR_AIN_CH			ADC_Channel_4


void adc_init(void)
{    
	GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;

	RCC_AHB1PeriphClockCmd(SR_ADC_TEMP_GPIO_CLK, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	GPIO_InitStructure.GPIO_Pin = SR_ADC_TEMP_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(SR_ADC_TEMP_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = SR_ADC_PWR_3V3_GPIO_PIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = SR_ADC_PWR_GPIO_PIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = SR_ADC_PWR_5V_GPIO_PIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = SR_ADC_PWR_AIN_GPIO_PIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	

	ADC_TempSensorVrefintCmd(ENABLE);

	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; 
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4; //ADCCLK=PCLK2/4=84/4=21Mhz, 建议ADC < 36Mhz
	ADC_CommonInit(&ADC_CommonInitStructure);

	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_Init(ADC1, &ADC_InitStructure);

	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_480Cycles);	// 板载热敏电阻转换温度检测	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1, ADC_SampleTime_480Cycles); // mcu 内部温度传感器检测	

	ADC_RegularChannelConfig(ADC1, SR_ADC_PWR_3V3_CH, 1, ADC_SampleTime_480Cycles);
	ADC_RegularChannelConfig(ADC1, SR_ADC_PWR_CH, 1, ADC_SampleTime_480Cycles);
	ADC_RegularChannelConfig(ADC1, SR_ADC_PWR_5V_CH, 1, ADC_SampleTime_480Cycles);
	ADC_RegularChannelConfig(ADC1, SR_ADC_PWR_AIN_CH, 1, ADC_SampleTime_480Cycles);

	ADC_Cmd(ADC1, ENABLE); 			
}

uint16_t get_adc(uint8_t ch)   
{
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_480Cycles);		    
	ADC_SoftwareStartConv(ADC1);		
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));

	return ADC_GetConversionValue(ADC1);
}

uint16_t get_adc_average(uint8_t ch,uint8_t times)
{
	uint32_t temp_val=0;
	uint8_t t;

	for (t=0; t<times; t++) {
		temp_val += get_adc(ch);
		delay_us(5);
	}
	return temp_val/times;
}

float get_temprate(void)
{
	uint32_t adcx;
 	double temperate;

	adcx = get_adc_average(ADC_Channel_16,10);	
	temperate = (float)adcx*(3.3/4096);	// 电压值
	sr_kprintf("temp_ad_val=%fV  \n", temperate);
	temperate = (temperate-0.76)/0.0025 + 25; // 转换为温度值
		
	return temperate;
}

int32_t pwr_adc_read(enum adc_items item, uint16_t* volt_mv)
{
	uint32_t adcx;
 	float temperate;
	uint16_t tmp_volt_mv = 0;
	
	switch (item) {
		case ITEM_ADC_TEMP_CH:
		{
			adcx = get_adc_average(ADC_Channel_0, 10);	
			temperate = (float)adcx*(3.3/4096);	// 电压值
			tmp_volt_mv = temperate*1000; 
			tmp_volt_mv -= 500; /* MCP9700T-E/TT 0℃时，为500mV	 */
			tmp_volt_mv /= 10; /* 换算成温度，10.0 mV/°C (typical) MCP9700/9700A */
			sr_kprintf("\r\ntemp_val=%d mV\r\n", tmp_volt_mv);
		}
		break;	
		case ITEM_ADC_PWR_3V3:
		{
			adcx = get_adc_average(SR_ADC_PWR_3V3_CH, 10);	
			temperate = (float)adcx*(3.3/4096);	// 电压值
			tmp_volt_mv = temperate*1000; /* 分压值 */
			tmp_volt_mv *= 2; /* 换算实际电压 */
			sr_kprintf("PWR_3V3_val=%d mV\r\n", tmp_volt_mv);
		}
		break;
		case ITEM_ADC_PWR:
		{
			adcx = get_adc_average(SR_ADC_PWR_CH, 10);	
			temperate = (float)adcx*(3.3/4096);	// 电压值
			tmp_volt_mv = temperate*1000; /* 分压值 */
			tmp_volt_mv *= 11; /* 换算实际电压 */
			sr_kprintf("PWR_val=%d mV\r\n", tmp_volt_mv);		
		}
		break;
		case ITEM_ADC_PWR_5V:
		{
			adcx = get_adc_average(SR_ADC_PWR_5V_CH, 10);	
			temperate = (float)adcx*(3.3/4096);	// 电压值
			tmp_volt_mv = temperate*1000; /* 分压值 */
			tmp_volt_mv *= 2; /* 换算实际电压 */
			sr_kprintf("PWR_5V_val=%d mV\r\n", tmp_volt_mv);		
		}
		break;
		case ITEM_ADC_PWR_AIN:
		{
			adcx = get_adc_average(SR_ADC_PWR_AIN_CH, 10);	
			temperate = (float)adcx*(3.3/4096);	// 电压值
			tmp_volt_mv = temperate*1000; 
			sr_kprintf("PWR_AIN_val=%d mV\r\n\r\n", tmp_volt_mv);
		}
		break;
	}
	return 0;
}

#if 0
int32_t rt_adc_read(rt_adc_device_t dev, uint32_t channel)
{

    return 0;
}

int32_t rt_adc_enable(rt_adc_device_t dev, uint32_t channel)
{

    return 0;
}

int32_t rt_adc_disable(rt_adc_device_t dev, uint32_t channel)
{

    return 0;
}
#endif
