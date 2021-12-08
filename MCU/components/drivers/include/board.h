/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-5      SummerGift   first version
 */

#ifndef __BOARD_H__
#define __BOARD_H__

#include <stdio.h>
#include <string.h>
#include "drivers\include\drivers\drv_common.h"



#ifdef __cplusplus
extern "C" {
#endif



/**< hw major version number */
#define GPIO_PORT_HW_VERSION    	GPIOG			             
#define GPIO_CLK_HW_VERSION 	    RCC_AHB1Periph_GPIOG		
#define GPIO_PIN_HW_VERSION			GPIO_Pin_2

/**< hw minor version number */
#define GPIO_PORT_HW_SUBVERSION    	GPIOG			             
#define GPIO_CLK_HW_SUBVERSION 	    RCC_AHB1Periph_GPIOG		
#define GPIO_PIN_HW_SUBVERSION		GPIO_Pin_3

/**< hw revise version number */
#define GPIO_PORT_HW_REVISION    	GPIOG			             
#define GPIO_CLK_HW_REVISION 	    RCC_AHB1Periph_GPIOG		
#define GPIO_PIN_HW_REVISION		GPIO_Pin_4

	

typedef enum {
	SR_LED_RUN = 0,
	SR_LED_CMD,
	SR_FAN,
} sr_bsp_dev_t;


#define LED0 		PFout(9)   	//LED0
#define LED1 		PFout(10)   	//LED1

/*
 * 打开设备，如LED、风扇
 *
 * @param[in]	None
 * @param[out]	None
 *
 * @return		0成功，其他失败
 */
void sr_bsp_dev_open(uint8_t dev);

/*
 * 关闭设备，如LED、风扇
 *
 * @param[in]	None
 * @param[out]	None
 *
 * @return		0成功，其他失败
 */
void sr_bsp_dev_close(uint8_t dev);

/*
 * 设备状态取反，如LED
 *
 * @param[in]	None
 * @param[out]	None
 *
 * @return		0成功，其他失败
 */
void sr_bsp_dev_toggle(sr_bsp_dev_t dev);


int32_t sr_startup(void);
void sr_start_scheduler(void);


#ifdef __cplusplus
}
#endif

#endif
