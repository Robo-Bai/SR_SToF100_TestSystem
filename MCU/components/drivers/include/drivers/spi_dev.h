#ifndef __SPI_DEV_H__
#define __SPI_DEV_H__

#include "stm32f4xx.h" 
#include "bsp\include\drv_common.h"
#include "drivers\include\sr_kfifo.h"


	

/*
 * 读/写一个字节
 *
 * @param[in]	tx_data: 要写入的数据
 * @param[out]	None
 *
 * @return		读取到的一个字节数据
 */
uint8_t spi1_rw_byte(uint8_t tx_data);


#endif /* __SPI_DEV_H__ */

