#ifndef __BSP_VC400_H__
#define __BSP_VC400_H__

#include <stdio.h>
#include <string.h>
#include "bsp\include\drv_common.h"



#ifdef __cplusplus
extern "C" {
#endif


#define  SR_CAN_HMI_SWITCH_GPIO_CLK    			RCC_AHB1Periph_GPIOB
#define  SR_CAN_HMI_SWITCH_GPIO_PORT   			GPIOB   
#define  SR_CAN_HMI_SWITCH_GPIO_PIN    			GPIO_Pin_14

#define  PWR_EN_GPIO_CLK    					RCC_AHB1Periph_GPIOG
#define  PWR_EN_GPIO_PORT  	 					GPIOG   
#define  PWR_EN_GPIO_PIN    					GPIO_Pin_0

/* 急停输出, 初始化默认置高 */	
#define GPIO_PORT_ST_ESTOP  					GPIOE			             
#define GPIO_CLK_ST_ESTOP 						RCC_AHB1Periph_GPIOE		
#define GPIO_PIN_ST_ESTOP						GPIO_Pin_5





#ifdef __cplusplus
}
#endif

#endif /* __BSP_VC400_H__ */
