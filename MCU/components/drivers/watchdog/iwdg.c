/**
 * Copyright (C) 2019 Standard-robots, Inc
 *
 * @file 	iwdg.c
 * @author	Neethan <lijun@standard-robots.com> / <neethan@foxmail.com>
 * @version V1.0.0
 * @date 	2020年 04月 29日 星期三 10:50:47 CST
 *
 * @brief   indepdent watch dog 
 */

#include "drivers\include\drivers\iwdg.h"


// 独立看门狗 (IWDG) 由其专用低速时钟 (LSI) 驱动，因此即便在主时钟发生故障时仍然保持 工作状态。
// 窗口看门狗 (WWDG) 时钟由 APB1 时钟经预分频后提供，通过可配置的时间窗 口来检测应用程序非正常的过迟或过早的操作。
// 32 kHz (LSI)
// Tout=((4*2^prer)*rlr)/32 (ms)
// MCU 内部的 RC 频率会在 30kHz 到 60kHz 之间变化,导致RC周期不完全准确
void iwdg_init(uint8_t prescaler, uint16_t reload)
{
	/* Check if the system has resumed from IWDG reset */
	if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET){
		/* IWDGRST flag set */
		/* Clear reset flags */
		RCC_ClearFlag();
	}

	/* Enable write access to IWDG_PR and IWDG_RLR registers */
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

	/* IWDG counter clock: LSI/32 */
	IWDG_SetPrescaler(prescaler);

	/* Set counter reload value to obtain 250ms IWDG TimeOut */   
	IWDG_SetReload(reload);

	/* Reload IWDG counter */
	IWDG_ReloadCounter();

	/* Enable IWDG (the LSI oscillator will be enabled by hardware) */
	IWDG_Enable();
}

void iwdg_feed(void)
{
	IWDG_ReloadCounter();
}

