/**
 * Copyright (C) 2019 Standard-robots, Inc
 *
 * @file 	drv_io_pin.c
 * @author	Neethan <lijun@standard-robots.com> / <neethan@foxmail.com>
 * @version V1.0.0
 * @date 	2019年 06月 21日 星期五 10:50:47 CST
 *
 * @brief    driver pin 
 */

#include <string.h>
#include "drivers\include\drivers\pin.h"


struct gpio_maps {
	GPIO_TypeDef* gpiox; 
	uint16_t gpio_pin;
};


static void func_do_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	#if defined (SR_VC400)
	RCC_AHB1PeriphClockCmd(GPIO_CLK_FUNC_DO1, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_FUNC_DO1 | GPIO_PIN_FUNC_DO2 | 
								GPIO_PIN_FUNC_DO3 | GPIO_PIN_FUNC_DO4 |
								GPIO_PIN_FUNC_DO6 | GPIO_PIN_FUNC_DO5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIO_PORT_FUNC_DO1, &GPIO_InitStructure);

	GPIO_ResetBits(GPIO_PORT_FUNC_DO1, GPIO_PIN_FUNC_DO1 | GPIO_PIN_FUNC_DO2 | 
								GPIO_PIN_FUNC_DO3 | GPIO_PIN_FUNC_DO4 |
								GPIO_PIN_FUNC_DO6 | GPIO_PIN_FUNC_DO5);
	#elif defined (SR_VC300)
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_FUNC_DO1 | GPIO_PIN_FUNC_DO2 | GPIO_PIN_FUNC_DO3 |
									GPIO_PIN_FUNC_DO4 | GPIO_PIN_FUNC_DO5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_FUNC_DO6 | GPIO_PIN_FUNC_DO7 | GPIO_PIN_FUNC_DO8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
		
	#endif
}

static void func_di_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	#if defined (SR_VC400)
	RCC_AHB1PeriphClockCmd(GPIO_CLK_FUNC_DI1, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_FUNC_DI1 | GPIO_PIN_FUNC_DI2 | 
									GPIO_PIN_FUNC_DI3 | GPIO_PIN_FUNC_DI4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIO_PORT_FUNC_DI1, &GPIO_InitStructure);
	
	#elif defined (SR_VC300)
	RCC_AHB1PeriphClockCmd(GPIO_CLK_FUNC_DI1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_FUNC_DI1 | GPIO_PIN_FUNC_DI2 | GPIO_PIN_FUNC_DI3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIO_PORT_FUNC_DI1, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_FUNC_DI4| GPIO_PIN_FUNC_DI5 | GPIO_PIN_FUNC_DI6 | 
									GPIO_PIN_FUNC_DI8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIO_PORT_FUNC_DI4, &GPIO_InitStructure);
	#endif
}


#if defined	(SR_VC400)
static void ack_do_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(GPIO_CLK_ACK_DO1_6, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_ACK_DO1 | GPIO_PIN_ACK_DO2 | 
								GPIO_PIN_ACK_DO3 | GPIO_PIN_ACK_DO4 |
								GPIO_PIN_ACK_DO6 | GPIO_PIN_ACK_DO5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIO_PORT_ACK_DO1_6, &GPIO_InitStructure);

	//GPIO_ResetBits(GPIO_PORT_ACK_DO1_6, GPIO_PIN_ACK_DO1 | GPIO_PIN_ACK_DO2 | 
	//							GPIO_PIN_ACK_DO3 | GPIO_PIN_ACK_DO4 |
	//							GPIO_PIN_ACK_DO6 | GPIO_PIN_ACK_DO5);
}

static void power_do_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(GPIO_CLK_POWER_DO1 | GPIO_CLK_POWER_DO2 | 
						GPIO_CLK_POWER_DO2| GPIO_CLK_POWER_DO4, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_POWER_DO1 | GPIO_PIN_POWER_DO2 | 
									GPIO_PIN_POWER_DO3 | GPIO_PIN_POWER_DO4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIO_PORT_POWER_DO1_4, &GPIO_InitStructure);

	/* 关闭 */
	GPIO_ResetBits(GPIO_PORT_POWER_DO1_4, GPIO_PIN_POWER_DO1 | GPIO_PIN_POWER_DO2 | 
					GPIO_PIN_POWER_DO3 | GPIO_PIN_POWER_DO4);
}

void bsp_power_do_set_status(uint16_t gpio_pin, bool status)
{
	if (status){
		GPIO_SetBits(GPIO_PORT_POWER_DO1_4, gpio_pin);	
	} else {
		GPIO_ResetBits(GPIO_PORT_POWER_DO1_4, gpio_pin);	
	}
}

static void power_en_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(GPIO_CLK_POWER_EN, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_POWER_EN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIO_PORT_POWER_EN, &GPIO_InitStructure);

	GPIO_ResetBits(GPIO_PORT_POWER_EN, GPIO_PIN_POWER_EN);
}

static void wifi_mode_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(GPIO_CLK_WIFI_MODE, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_WIFI_MODE;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIO_PORT_WIFI_MODE, &GPIO_InitStructure);

	GPIO_SetBits(GPIO_PORT_WIFI_MODE, GPIO_PIN_WIFI_MODE);
}

static void notice_nxp_sys_status_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(GPIO_CLK_SYS_STATUS, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_SYS_STATUS;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIO_PORT_SYS_STATUS, &GPIO_InitStructure);

	GPIO_SetBits(GPIO_PORT_SYS_STATUS, GPIO_PIN_SYS_STATUS);
}

static void nxp_estop_status_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(GPIO_CLK_ESTOP_STATUS, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_ESTOP_STATUS;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIO_PORT_ESTOP_STATUS, &GPIO_InitStructure);
}

static void st_estop_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(GPIO_CLK_ST_ESTOP, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_ST_ESTOP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIO_PORT_ST_ESTOP, &GPIO_InitStructure);

	GPIO_SetBits(GPIO_PORT_ST_ESTOP, GPIO_PIN_ST_ESTOP);
}

static void st_estop_status_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(GPIO_CLK_ST_ESTOP_STATUS, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_ST_ESTOP_STATUS;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIO_PORT_ST_ESTOP_STATUS, &GPIO_InitStructure);

	GPIO_ResetBits(GPIO_PORT_ST_ESTOP_STATUS, GPIO_PIN_ST_ESTOP_STATUS);
}

static void estop_di_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(GPIO_CLK_ESTOP_DI1_4, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_ESTOP_DI1 | GPIO_PIN_ESTOP_DI2 |
								GPIO_PIN_ESTOP_DI3 | GPIO_PIN_ESTOP_DI4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	GPIO_Init(GPIO_PORT_ESTOP_DI1_4, &GPIO_InitStructure);
}

static void st_estop_status_ack_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(GPIO_CLK_ST_ESTOP_STATUS_ACK, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_ST_ESTOP_STATUS_ACK;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIO_PORT_ST_ESTOP_STATUS_ACK, &GPIO_InitStructure);
}

// rest - 0 ，tk1正常工作
static void comm2tk1_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_RST_TK1 | GPIO_PIN_MCU2TK1_INIT | GPIO_PIN_SYS_IO_TBD;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIO_PORT_COMM_TK1, &GPIO_InitStructure);

	GPIO_ResetBits(GPIO_PORT_COMM_TK1, GPIO_PIN_RST_TK1);
	GPIO_ResetBits(GPIOC,  GPIO_PIN_MCU2TK1_INIT | GPIO_PIN_SYS_IO_TBD);
	
	/* tk1 to mcu init */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_TK1TOMCU_INIT;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIO_PORT_TK1TOMCU_INIT, &GPIO_InitStructure);
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource2);

	/* 配置EXTI_Line2 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line2;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; // 上升沿触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


void EXTI2_IRQHandler(void)
{		 
	sr_kprintf("EXTI2_IRQ\r\n");
	EXTI_ClearITPendingBit(EXTI_Line2);
}

int32_t gpio_pin_test(void)
{
	static uint8_t tag = 0;
	
	sr_kprintf("\r\ngpio_pin_irq_test\r\n");;
	
	tag = !tag;
	if (tag == 0x01){
		GPIO_ResetBits(GPIO_PORT_MCU2TK1_INIT, GPIO_PIN_MCU2TK1_INIT);
	} else if(tag == 0x0){
		GPIO_SetBits(GPIO_PORT_MCU2TK1_INIT, GPIO_PIN_MCU2TK1_INIT);
	}
	return 0;
}

/* io测试项配置 */
static void bsp_do1_2_ack_items_init(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	GPIO_InitTypeDef   GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(GPIO_CLK_ACK_DO1_6, ENABLE);
	/* Enable SYSCFG clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/* Configure DO_ACK pin as input */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_ACK_DO1 | GPIO_PIN_ACK_DO2;
	GPIO_Init(GPIO_PORT_ACK_DO1_6, &GPIO_InitStructure);

	/* Connect EXTI Line8~13 to PF8~PF13 pin */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource8);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource9);
	
	/* Configure EXTI Line8~13 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line8 | EXTI_Line9;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Enable and set EXTI Line8~13 Interrupt to the lowest priority */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x04;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

static void bsp_do3_6_ack_items_init(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	GPIO_InitTypeDef   GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(GPIO_CLK_ACK_DO1_6, ENABLE);
	/* Enable SYSCFG clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/* Configure DO_ACK pin as input */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_ACK_DO3 | GPIO_PIN_ACK_DO4 |
								GPIO_PIN_ACK_DO6 | GPIO_PIN_ACK_DO5;
	GPIO_Init(GPIO_PORT_ACK_DO1_6, &GPIO_InitStructure);

	/* Connect EXTI Line10~13 to PF8~PF13 pin */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource10);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource11);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource12);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource13);
	
	/* Configure EXTI Line10~13 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line10 | EXTI_Line11 | EXTI_Line12 | 
									EXTI_Line13;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Enable and set EXTI Line10~13 Interrupt to the lowest priority */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x04;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void items_do_set(uint16_t GPIO_Pin, uint8_t value)
{
	if (value) {
		GPIO_SetBits(GPIO_PORT_FUNC_DO1, GPIO_Pin);	
	} else {
		GPIO_ResetBits(GPIO_PORT_FUNC_DO1, GPIO_Pin);	
	}
}

static int32_t items_do_ack_init(void)
{
	bsp_do1_2_ack_items_init();
	bsp_do3_6_ack_items_init();
	
	return 0;
}


/**
  * @brief  This function handles External line 9 to 5 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI9_5_IRQHandler(void)
{	
	if (EXTI_GetITStatus(EXTI_Line8) != RESET) {
		/* Clear the EXTI line 8 pending bit */
		EXTI_ClearITPendingBit(EXTI_Line8);
		sr_kprintf("EXTI_8\r\n");
		
	} else if (EXTI_GetITStatus(EXTI_Line9) != RESET) {
		/* Clear the EXTI line 9 pending bit */
		EXTI_ClearITPendingBit(EXTI_Line9);
		sr_kprintf("EXTI_9\r\n");
	}
}

/**
  * @brief  This function handles External lines 15 to 10 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI15_10_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line10) != RESET){
		/* Clear the EXTI line 10 pending bit */
		EXTI_ClearITPendingBit(EXTI_Line10);
		sr_kprintf("EXTI_10\r\n");
	} else if (EXTI_GetITStatus(EXTI_Line11) != RESET){
		/* Clear the EXTI line 11 pending bit */
		EXTI_ClearITPendingBit(EXTI_Line11);
		sr_kprintf("EXTI_11\r\n");
	}else if (EXTI_GetITStatus(EXTI_Line12) != RESET){

		/* Clear the EXTI line 12 pending bit */
		EXTI_ClearITPendingBit(EXTI_Line12);
		sr_kprintf("EXTI_12\r\n");
	}
	else if(EXTI_GetITStatus(EXTI_Line13) != RESET)
	{
		/* Clear the EXTI line 13 pending bit */
		EXTI_ClearITPendingBit(EXTI_Line13);
		sr_kprintf("EXTI_13\r\n");
	}
}

#endif

struct gpio_maps* pin_maps(enum pin_func_t type, enum pin_io_t pin)
{
	static struct gpio_maps g_map;
	
	g_map.gpiox = NULL;
	
	if (PIN_FUNC_DI == type){
		switch (pin) {
			case PIN_IO_1:
				g_map.gpiox = GPIO_PORT_FUNC_DI1;
				g_map.gpio_pin = GPIO_PIN_FUNC_DI1;
				break;
			case PIN_IO_2:
				g_map.gpiox = GPIO_PORT_FUNC_DI2;
				g_map.gpio_pin = GPIO_PIN_FUNC_DI2;
				break;
			case PIN_IO_3:
				g_map.gpiox = GPIO_PORT_FUNC_DI3;
				g_map.gpio_pin = GPIO_PIN_FUNC_DI3;
				break;
			case PIN_IO_4:
				g_map.gpiox = GPIO_PORT_FUNC_DI4;
				g_map.gpio_pin = GPIO_PIN_FUNC_DI4;
				break;
			case PIN_IO_5:
			#if defined (SR_VC300)
				g_map.gpiox = GPIO_PORT_FUNC_DI5;
				g_map.gpio_pin = GPIO_PIN_FUNC_DI5;
			#endif
				break;
			case PIN_IO_6:
			#if defined (SR_VC300)
				g_map.gpiox = GPIO_PORT_FUNC_DI6;
				g_map.gpio_pin = GPIO_PIN_FUNC_DI6;
			#endif
				break;
			
			default:
				break;
		}
	} else if (PIN_FUNC_DO == type){
		switch (pin) {
			case PIN_IO_1:
				g_map.gpiox = GPIO_PORT_FUNC_DO1;
				g_map.gpio_pin = GPIO_PIN_FUNC_DO1;
				break;
			case PIN_IO_2:
				g_map.gpiox = GPIO_PORT_FUNC_DO2;
				g_map.gpio_pin = GPIO_PIN_FUNC_DO2;
				break;
			case PIN_IO_3:
				g_map.gpiox = GPIO_PORT_FUNC_DO3;
				g_map.gpio_pin = GPIO_PIN_FUNC_DO3;
				break;
			case PIN_IO_4:
				g_map.gpiox = GPIO_PORT_FUNC_DO4;
				g_map.gpio_pin = GPIO_PIN_FUNC_DO4;
				break;
			case PIN_IO_5:
				g_map.gpiox = GPIO_PORT_FUNC_DO5;
				g_map.gpio_pin = GPIO_PIN_FUNC_DO5;
				break;
			case PIN_IO_6:
				g_map.gpiox = GPIO_PORT_FUNC_DO6;
				g_map.gpio_pin = GPIO_PIN_FUNC_DO6;
				break;
			
			#if defined (SR_VC300)
			case PIN_IO_7:
				g_map.gpiox = GPIO_PORT_FUNC_DO7;
				g_map.gpio_pin = GPIO_PIN_FUNC_DO7;
				break;
			case PIN_IO_8:
				g_map.gpiox = GPIO_PORT_FUNC_DO8;
				g_map.gpio_pin = GPIO_PIN_FUNC_DO8;
				break;
			#endif
			
			default:
				break;
		}	
	}
	return &g_map;
}


int32_t sr_pin_write(enum pin_func_t type, enum pin_io_t pin, uint8_t val)
{
	int16_t ret = -1;
	struct gpio_maps *maps = NULL;
	
	maps = pin_maps(type, pin);

	if (maps->gpiox){
		if(val > 0){
			digitalHi(maps->gpiox, maps->gpio_pin)
		} else {
			digitalLo(maps->gpiox, maps->gpio_pin)
		}

		if(GPIO_ReadOutputDataBit(maps->gpiox, maps->gpio_pin) == val){
			return 0;
		} else {
			return -1;
		}
	}
	
	return ret;
}

int32_t sr_pin_read(enum pin_func_t type, enum pin_io_t pin, uint8_t* value)
{
	struct gpio_maps *maps = NULL;
	
	maps = pin_maps(type, pin);
	
	if (PIN_FUNC_DO == type) {
		*value = GPIO_ReadOutputDataBit(maps->gpiox, maps->gpio_pin);
	
	} else if(PIN_FUNC_DI == type) {
		*value = GPIO_ReadInputDataBit(maps->gpiox, maps->gpio_pin);	
	}
	return 0;
}

uint8_t do_get_status(void)
{
	uint8_t status = 0;
	uint8_t i;
	uint8_t bits_array[8] = {0};

	bits_array[0] = GPIO_ReadOutputDataBit(GPIO_PORT_FUNC_DO1, GPIO_PIN_FUNC_DO1);
	bits_array[1] = GPIO_ReadOutputDataBit(GPIO_PORT_FUNC_DO2, GPIO_PIN_FUNC_DO2);
	bits_array[2] = GPIO_ReadOutputDataBit(GPIO_PORT_FUNC_DO3, GPIO_PIN_FUNC_DO3);
	bits_array[3] = GPIO_ReadOutputDataBit(GPIO_PORT_FUNC_DO4, GPIO_PIN_FUNC_DO4);
	bits_array[4] = GPIO_ReadOutputDataBit(GPIO_PORT_FUNC_DO5, GPIO_PIN_FUNC_DO5);
	bits_array[5] = GPIO_ReadOutputDataBit(GPIO_PORT_FUNC_DO6, GPIO_PIN_FUNC_DO6);
	
	#if defined (SR_VC400)
	bits_array[6] = 0;
	bits_array[7] = 0;
	#elif defined (SR_VC300)
	bits_array[6] = GPIO_ReadOutputDataBit(GPIO_PORT_FUNC_DO7, GPIO_PIN_FUNC_DO7);
	bits_array[7] = GPIO_ReadOutputDataBit(GPIO_PORT_FUNC_DO8, GPIO_PIN_FUNC_DO8);
	#endif

	for(i=0; i<8; i++){
		status |= ((bits_array[i]&0x01)<<i);
	}
	return status;
}

uint8_t in_verse(uint8_t in)
{
	return (in>0?0:1);
}
uint8_t digital_IN0 = 0;
uint8_t digital_IN0_num = 0;

void digital_IN0_Filter(void)
{
	if(GPIO_ReadInputDataBit(GPIO_PORT_FUNC_DI1, GPIO_PIN_FUNC_DI1) == 1){
		digital_IN0_num++;
	} else{
		digital_IN0_num = 0;
	}
	if(digital_IN0_num >= 100){digital_IN0_num = 100;}
	if(digital_IN0_num >=5){
		digital_IN0 = 1;
	} else{
		digital_IN0 = 0;
	}
}

uint8_t di_get_status(void)
{
	uint8_t value[8] = {0};
	uint8_t index = 0;
	uint8_t di_status = 0;

	#if defined (SR_VC400)
	value[index++] = GPIO_ReadInputDataBit(GPIO_PORT_FUNC_DI1, GPIO_PIN_FUNC_DI1);
	value[index++] = GPIO_ReadInputDataBit(GPIO_PORT_FUNC_DI2, GPIO_PIN_FUNC_DI2);
	value[index++] = GPIO_ReadInputDataBit(GPIO_PORT_FUNC_DI3, GPIO_PIN_FUNC_DI3);
	value[index++] = GPIO_ReadInputDataBit(GPIO_PORT_FUNC_DI4, GPIO_PIN_FUNC_DI4);
	memset(&value[index], 0, sizeof(value)-index);
	#elif defined (SR_VC300)
	value[index++] = in_verse(digital_IN0);//GULF_L14??????д???
	value[index++] = in_verse(GPIO_ReadInputDataBit(GPIO_PORT_FUNC_DI2, GPIO_PIN_FUNC_DI2));
	value[index++] = in_verse(GPIO_ReadInputDataBit(GPIO_PORT_FUNC_DI3, GPIO_PIN_FUNC_DI3));
	value[index++] = in_verse(GPIO_ReadInputDataBit(GPIO_PORT_FUNC_DI4, GPIO_PIN_FUNC_DI4));
	value[index++] = in_verse(GPIO_ReadInputDataBit(GPIO_PORT_FUNC_DI5, GPIO_PIN_FUNC_DI5));
	value[index++] = in_verse(GPIO_ReadInputDataBit(GPIO_PORT_FUNC_DI6, GPIO_PIN_FUNC_DI6));
	value[index++] = in_verse(GPIO_ReadInputDataBit(GPIO_PORT_FUNC_DI7, GPIO_PIN_FUNC_DI7));
	value[index++] = in_verse(GPIO_ReadInputDataBit(GPIO_PORT_FUNC_DI8, GPIO_PIN_FUNC_DI8));
	#endif
	
	for (uint8_t i = 0; i<sizeof(value); i++){
		di_status |= ((value[i]&0x01)<<i);
	}
	
	return di_status;
}

int32_t bsp_pin_init(void)
{
	#if defined	(SR_VC400)
	power_do_init(); /* 工厂测试程序电流负载能力有限，不能同时开启 */
	ack_do_init();
	power_en_init();
	wifi_mode_init();
	notice_nxp_sys_status_init();
	nxp_estop_status_init();
	st_estop_init();
	st_estop_status_init(); 
	estop_di_init();
	st_estop_status_ack_init();
	comm2tk1_init();

	items_do_ack_init();
	#endif
	
	func_do_init();
	func_di_init();
	
	return 0;
}