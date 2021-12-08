/**
 * Copyright (C) 2021 Standard-robots, Inc
 *
 * @file 	drv_common.c
 * @author	Neethan <lijun@standard-robots.com> / <neethan@foxmail.com>
 * @version V1.0.0
 * @date 	2021年 03月 29日 星期一 10:50:47 CST
 *
 * @brief	driver common  
 */


#include "drivers\include\drivers\drv_common.h"
#include "drivers\include\drivers\drv_uart.h"




void set_soc_peripheral_state(enum soc_peripheral peripheral, bool state)
{
	#if 0
	switch (peripheral){
		case SOC_UART1:
			if (state){
				USART_Cmd(SR_USART1, ENABLE);
			} else {
				USART_Cmd(SR_USART1, DISABLE);
			}
			break;
						
		default:
			break;

	}
	#endif
}
