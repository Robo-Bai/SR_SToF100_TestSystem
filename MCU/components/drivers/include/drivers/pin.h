#ifndef __DRV_PIN_H__
#define __DRV_PIN_H__


#include "bsp\include\drv_common.h"



/* POWER DO1-4 */
#define GPIO_PORT_POWER_DO1_4   GPIOD	
#define GPIO_PORT_POWER_DO1    	GPIOD			             
#define GPIO_CLK_POWER_DO1 	    RCC_AHB1Periph_GPIOD		
#define GPIO_PIN_POWER_DO1		GPIO_Pin_12	

#define GPIO_PORT_POWER_DO2    	GPIOD			             
#define GPIO_CLK_POWER_DO2 	    RCC_AHB1Periph_GPIOD		
#define GPIO_PIN_POWER_DO2		GPIO_Pin_13

#define GPIO_PORT_POWER_DO3  	GPIOD			             
#define GPIO_CLK_POWER_DO3		RCC_AHB1Periph_GPIOD		
#define GPIO_PIN_POWER_DO3		GPIO_Pin_14

#define GPIO_PORT_POWER_DO4 	GPIOD			             
#define GPIO_CLK_POWER_DO4   	RCC_AHB1Periph_GPIOD		
#define GPIO_PIN_POWER_DO4		GPIO_Pin_15

#if defined (SR_VC400)
/* Function DO */
#define GPIO_PORT_FUNC_DO1    	GPIOF			             
#define GPIO_CLK_FUNC_DO1 	    RCC_AHB1Periph_GPIOF		
#define GPIO_PIN_FUNC_DO1		GPIO_Pin_2	

#define GPIO_PORT_FUNC_DO2    	GPIOF			             
#define GPIO_CLK_FUNC_DO2 	    RCC_AHB1Periph_GPIOF		
#define GPIO_PIN_FUNC_DO2		GPIO_Pin_3

#define GPIO_PORT_FUNC_DO3  	GPIOF			             
#define GPIO_CLK_FUNC_DO3		RCC_AHB1Periph_GPIOF		
#define GPIO_PIN_FUNC_DO3		GPIO_Pin_4

#define GPIO_PORT_FUNC_DO4 		GPIOF			             
#define GPIO_CLK_FUNC_DO4   	RCC_AHB1Periph_GPIOF		
#define GPIO_PIN_FUNC_DO4		GPIO_Pin_5

#define GPIO_PORT_FUNC_DO5  	GPIOF			             
#define GPIO_CLK_FUNC_DO5		RCC_AHB1Periph_GPIOF		
#define GPIO_PIN_FUNC_DO5		GPIO_Pin_6

#define GPIO_PORT_FUNC_DO6 		GPIOF			             
#define GPIO_CLK_FUNC_DO6   	RCC_AHB1Periph_GPIOF		
#define GPIO_PIN_FUNC_DO6		GPIO_Pin_7

#elif defined (SR_VC300)
/* Function DO */
#define GPIO_PORT_FUNC_DO1    	GPIOE			             
#define GPIO_CLK_FUNC_DO1 	    RCC_AHB1Periph_GPIOE		
#define GPIO_PIN_FUNC_DO1		GPIO_Pin_2	

#define GPIO_PORT_FUNC_DO2    	GPIOE			             
#define GPIO_CLK_FUNC_DO2 	    RCC_AHB1Periph_GPIOE		
#define GPIO_PIN_FUNC_DO2		GPIO_Pin_3

#define GPIO_PORT_FUNC_DO3  	GPIOE			             
#define GPIO_CLK_FUNC_DO3		RCC_AHB1Periph_GPIOE		
#define GPIO_PIN_FUNC_DO3		GPIO_Pin_4

#define GPIO_PORT_FUNC_DO4 		GPIOE			             
#define GPIO_CLK_FUNC_DO4   	RCC_AHB1Periph_GPIOE		
#define GPIO_PIN_FUNC_DO4		GPIO_Pin_5

#define GPIO_PORT_FUNC_DO5  	GPIOE			             
#define GPIO_CLK_FUNC_DO5		RCC_AHB1Periph_GPIOE		
#define GPIO_PIN_FUNC_DO5		GPIO_Pin_6

#define GPIO_PORT_FUNC_DO6 		GPIOF			             
#define GPIO_CLK_FUNC_DO6   	RCC_AHB1Periph_GPIOF		
#define GPIO_PIN_FUNC_DO6		GPIO_Pin_6

#define GPIO_PORT_FUNC_DO7 		GPIOF			             
#define GPIO_CLK_FUNC_DO7   	RCC_AHB1Periph_GPIOF		
#define GPIO_PIN_FUNC_DO7		GPIO_Pin_7

#define GPIO_PORT_FUNC_DO8 		GPIOF			             
#define GPIO_CLK_FUNC_DO8   	RCC_AHB1Periph_GPIOF		
#define GPIO_PIN_FUNC_DO8		GPIO_Pin_8
#endif

/* ACK DO */
#define GPIO_PORT_ACK_DO1_6   	GPIOF
#define GPIO_CLK_ACK_DO1_6 		RCC_AHB1Periph_GPIOF
#define GPIO_PORT_ACK_DO1    	GPIOF			             
#define GPIO_CLK_ACK_DO1 	    RCC_AHB1Periph_GPIOF		
#define GPIO_PIN_ACK_DO1		GPIO_Pin_8	

#define GPIO_PORT_ACK_DO2    	GPIOF			             
#define GPIO_CLK_ACK_DO2 	    RCC_AHB1Periph_GPIOF		
#define GPIO_PIN_ACK_DO2		GPIO_Pin_9

#define GPIO_PORT_ACK_DO3  		GPIOF			             
#define GPIO_CLK_ACK_DO3		RCC_AHB1Periph_GPIOF		
#define GPIO_PIN_ACK_DO3		GPIO_Pin_10

#define GPIO_PORT_ACK_DO4 		GPIOF			             
#define GPIO_CLK_ACK_DO4   		RCC_AHB1Periph_GPIOF		
#define GPIO_PIN_ACK_DO4		GPIO_Pin_11

#define GPIO_PORT_ACK_DO5  		GPIOF			             
#define GPIO_CLK_ACK_DO5		RCC_AHB1Periph_GPIOF		
#define GPIO_PIN_ACK_DO5		GPIO_Pin_12

#define GPIO_PORT_ACK_DO6 		GPIOF			             
#define GPIO_CLK_ACK_DO6   		RCC_AHB1Periph_GPIOF		
#define GPIO_PIN_ACK_DO6		GPIO_Pin_13

#if defined (SR_VC400)
/* FUNCTION DI1-4 */
#define GPIO_PORT_FUNC_DI1    	GPIOE			             
#define GPIO_CLK_FUNC_DI1 	    RCC_AHB1Periph_GPIOE		
#define GPIO_PIN_FUNC_DI1		GPIO_Pin_10	

#define GPIO_PORT_FUNC_DI2    	GPIOE			             
#define GPIO_CLK_FUNC_DI2 	    RCC_AHB1Periph_GPIOE		
#define GPIO_PIN_FUNC_DI2		GPIO_Pin_11

#define GPIO_PORT_FUNC_DI3  	GPIOE			             
#define GPIO_CLK_FUNC_DI3		RCC_AHB1Periph_GPIOE		
#define GPIO_PIN_FUNC_DI3		GPIO_Pin_12

#define GPIO_PORT_FUNC_DI4 		GPIOE			             
#define GPIO_CLK_FUNC_DI4   	RCC_AHB1Periph_GPIOE		
#define GPIO_PIN_FUNC_DI4		GPIO_Pin_13

#elif defined (SR_VC300)
#define GPIO_PORT_FUNC_DI1    	GPIOF			             
#define GPIO_CLK_FUNC_DI1 	    RCC_AHB1Periph_GPIOF		
#define GPIO_PIN_FUNC_DI1		GPIO_Pin_9	

#define GPIO_PORT_FUNC_DI2    	GPIOF			             
#define GPIO_CLK_FUNC_DI2 	    RCC_AHB1Periph_GPIOF		
#define GPIO_PIN_FUNC_DI2		GPIO_Pin_10

#define GPIO_PORT_FUNC_DI3  	GPIOF			             
#define GPIO_CLK_FUNC_DI3		RCC_AHB1Periph_GPIOF		
#define GPIO_PIN_FUNC_DI3		GPIO_Pin_11

#define GPIO_PORT_FUNC_DI4 		GPIOG			             
#define GPIO_CLK_FUNC_DI4   	RCC_AHB1Periph_GPIOG		
#define GPIO_PIN_FUNC_DI4		GPIO_Pin_6

#define GPIO_PORT_FUNC_DI5 		GPIOG			             
#define GPIO_CLK_FUNC_DI5   	RCC_AHB1Periph_GPIOG		
#define GPIO_PIN_FUNC_DI5		GPIO_Pin_7

#define GPIO_PORT_FUNC_DI6 		GPIOG			             
#define GPIO_CLK_FUNC_DI6   	RCC_AHB1Periph_GPIOG		
#define GPIO_PIN_FUNC_DI6		GPIO_Pin_8

#define GPIO_PORT_FUNC_DI7 		GPIOG			             
#define GPIO_CLK_FUNC_DI7   	RCC_AHB1Periph_GPIOG		
#define GPIO_PIN_FUNC_DI7		GPIO_Pin_9

#define GPIO_PORT_FUNC_DI8 		GPIOG			             
#define GPIO_CLK_FUNC_DI8   	RCC_AHB1Periph_GPIOG		
#define GPIO_PIN_FUNC_DI8		GPIO_Pin_12
#endif

/* POWER EN */	
#define GPIO_PORT_POWER_EN    	GPIOG			             
#define GPIO_CLK_POWER_EN 	    RCC_AHB1Periph_GPIOG		
#define GPIO_PIN_POWER_EN		GPIO_Pin_0

/* WIFI MODE */	
#define GPIO_PORT_WIFI_MODE    	GPIOG			             
#define GPIO_CLK_WIFI_MODE 	    RCC_AHB1Periph_GPIOG		
#define GPIO_PIN_WIFI_MODE		GPIO_Pin_1

	
#define GPIO_PORT_SYS_STATUS    GPIOE			             
#define GPIO_CLK_SYS_STATUS 	RCC_AHB1Periph_GPIOE		
#define GPIO_PIN_SYS_STATUS		GPIO_Pin_7


#define GPIO_PORT_ESTOP_STATUS  GPIOE			             
#define GPIO_CLK_ESTOP_STATUS 	RCC_AHB1Periph_GPIOE		
#define GPIO_PIN_ESTOP_STATUS	GPIO_Pin_6


#define GPIO_PORT_ST_ESTOP  	GPIOE			             
#define GPIO_CLK_ST_ESTOP 		RCC_AHB1Periph_GPIOE		
#define GPIO_PIN_ST_ESTOP		GPIO_Pin_5


#define GPIO_PORT_ST_ESTOP_STATUS  	GPIOE			             
#define GPIO_CLK_ST_ESTOP_STATUS 	RCC_AHB1Periph_GPIOE		
#define GPIO_PIN_ST_ESTOP_STATUS	GPIO_Pin_0


#define GPIO_PORT_ESTOP_DI1_4  	GPIOE
#define GPIO_CLK_ESTOP_DI1_4 	RCC_AHB1Periph_GPIOE
#define GPIO_PORT_ESTOP_DI1  	GPIOE			             
#define GPIO_CLK_ESTOP_DI1 		RCC_AHB1Periph_GPIOE		
#define GPIO_PIN_ESTOP_DI1		GPIO_Pin_1

#define GPIO_PORT_ESTOP_DI2  	GPIOE			             
#define GPIO_CLK_ESTOP_DI2 		RCC_AHB1Periph_GPIOE		
#define GPIO_PIN_ESTOP_DI2		GPIO_Pin_2

#define GPIO_PORT_ESTOP_DI3  	GPIOE			             
#define GPIO_CLK_ESTOP_DI3 		RCC_AHB1Periph_GPIOE		
#define GPIO_PIN_ESTOP_DI3		GPIO_Pin_3

#define GPIO_PORT_ESTOP_DI4  	GPIOE			             
#define GPIO_CLK_ESTOP_DI4 		RCC_AHB1Periph_GPIOE		
#define GPIO_PIN_ESTOP_DI4		GPIO_Pin_4


#define GPIO_PORT_ST_ESTOP_STATUS_ACK  	GPIOE			             
#define GPIO_CLK_ST_ESTOP_STATUS_ACK 	RCC_AHB1Periph_GPIOE		
#define GPIO_PIN_ST_ESTOP_STATUS_ACK	GPIO_Pin_14


#define GPIO_PORT_COMM_TK1  	GPIOC			             
#define GPIO_CLK_COMM_TK1 		RCC_AHB1Periph_GPIOC

#define GPIO_PORT_RST_TK1  		GPIOC			             
#define GPIO_CLK_RST_TK1 		RCC_AHB1Periph_GPIOC		
#define GPIO_PIN_RST_TK1		GPIO_Pin_0

#define GPIO_PORT_MCU2TK1_INIT  GPIOC			             
#define GPIO_CLK_MCU2TK1_INIT 	RCC_AHB1Periph_GPIOC		
#define GPIO_PIN_MCU2TK1_INIT	GPIO_Pin_1

#define GPIO_PORT_TK1TOMCU_INIT  GPIOC			             
#define GPIO_CLK_TK1TOMCU_INIT 	RCC_AHB1Periph_GPIOC		
#define GPIO_PIN_TK1TOMCU_INIT	GPIO_Pin_2

#define GPIO_PORT_SYS_IO_TBD  	GPIOC			             
#define GPIO_CLK_SYS_IO_TBD 	RCC_AHB1Periph_GPIOC		
#define GPIO_PIN_SYS_IO_TBD		GPIO_Pin_3

enum pin_func_t {
	PIN_FUNC_DI,
	PIN_FUNC_DO
};

enum pin_io_t {
	PIN_IO_1,
	PIN_IO_2,
	PIN_IO_3,
	PIN_IO_4,
	PIN_IO_5,
	PIN_IO_6,
	PIN_IO_7,
	PIN_IO_8,
};

int32_t bsp_pin_init(void);
void bsp_power_do_set_status(uint16_t gpio_pin, bool status);
int32_t gpio_pin_test(void);
void items_do_set(uint16_t GPIO_Pin, uint8_t value);



int32_t sr_pin_write(enum pin_func_t type, enum pin_io_t pin, uint8_t value);


int32_t sr_pin_read(enum pin_func_t type, enum pin_io_t pin, uint8_t* value);

uint8_t do_get_status(void);
uint8_t di_get_status(void);

#endif /* __DRV_PIN_H__ */

