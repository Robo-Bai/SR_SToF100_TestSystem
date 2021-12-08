#ifndef __DRV_COMMON_H__
#define __DRV_COMMON_H__


#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include "stm32f4xx.h"
#include "core\include\err_code.h"
#include "core\include\srtos.h"


enum soc_peripheral {
	SOC_UART1,
	SOC_UART2,
	SOC_UART3,
	SOC_UART4,
	SOC_UART5,
	SOC_CAN1,
	SOC_CAN2,
	SOC_SPI,
	SOC_I2C,
	SOC_ADC,
};


#define	digitalHi(p,i)		 	{p->BSRRL=i;}	 	
#define digitalLo(p,i)		 	{p->BSRRH=i;}	 
#define digitalToggle(p,i) 		{p->ODR ^=i;} 

	 


void set_soc_peripheral_state(enum soc_peripheral, bool state);



#ifdef __cplusplus
}
#endif
#endif
