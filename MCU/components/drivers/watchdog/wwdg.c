/**
 * Copyright (C) 2019 Standard-robots, Inc
 *
 * @file 	wwdg.c
 * @author	Neethan <lijun@standard-robots.com> / <neethan@foxmail.com>
 * @version V1.0.0
 * @date 	2020年 04月 29日 星期三 10:50:47 CST
 *
 * @brief   window watch dog 
 */
 
#include "drivers\include\drivers\wwdg.h"



// 窗口看门狗通常被用来监测，由外部干扰或不可预见的逻辑条件造成的
// 应用程序背离正常的运行序列而产生的软件故障。
int32_t wwdg_init(void)
{
	/* Check if the system has resumed from WWDG reset */
	if (RCC_GetFlagStatus(RCC_FLAG_WWDGRST) != RESET) { 
		/* Clear reset flags */
		RCC_ClearFlag();
	}

	/* WWDG configuration */
	/* Enable WWDG clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);

	/* WWDG clock counter = (PCLK1 (42MHz)/4096)/8 = 1281 Hz (~780 us)  */
	WWDG_SetPrescaler(WWDG_Prescaler_8);

	/* Set Window value to 80; WWDG counter should be refreshed only when the counter
	is below 80 (and greater than 64 (STM32F4参考手册RM0090 P499)) otherwise a 
	reset will be generated */
	WWDG_SetWindowValue(80);

	/* Enable WWDG and set counter value to 127, WWDG timeout = ~780 us * 64 = 49.92 ms 
	 In this case the refresh window is: 
		   ~780 * (127-80) = 36.6ms < refresh window < ~780 * 64 = 49.9ms
	*/
	WWDG_Enable(127);
}

void wwdg_feed(uint8_t Counter)
{
    /* Update WWDG counter */
    WWDG_SetCounter(Counter);
}