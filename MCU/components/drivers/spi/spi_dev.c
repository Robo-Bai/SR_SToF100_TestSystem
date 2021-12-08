/**
 * Copyright (C) 2019 Standard-robots, Inc
 *
 * @file 	spi_dev.c
 * @author	Neethan <lijun@standard-robots.com> / <neethan@foxmail.com>
 * @version V1.0.0
 * @date 	2020年 2月 12日 星期三 10:50:47 CST
 *
 * @brief    driver spi device driver 
 */

#include <string.h>
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "timers.h"

#include "srtos\err_code.h"
#include "bsp\include\drv_spi.h"
#include "drivers\include\drivers\spi_dev.h"



extern int32_t kservice_fifo_out(struct kservice_manager *manager, uint8_t *data, uint16_t len);



/*
 * 读/写一个字节
 *
 * @param[in]	tx_data: 要写入的数据
 * @param[out]	None
 *
 * @return		读取到的一个字节数据
 */
uint8_t spi1_rw_byte(uint8_t tx_data)
{	
	// 等待发送空闲
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){} 
	SPI_I2S_SendData(SPI1, tx_data); 
	// 等待接收空闲
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET){} 
	return SPI_I2S_ReceiveData(SPI1);     
}


void spi_irq_handler(void)
{
	uint8_t data = 0;
	uint8_t out = 0;
	
	/* 接收中断 */
	if (SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_RXNE) == SET) { 

		data = SPI_I2S_ReceiveData(SPI1);
		//SPI_I2S_SendData(SPI1, 'A');
		SPI_I2S_ClearITPendingBit(SPI1, SPI_I2S_IT_RXNE);
	} 
	
	/* SPI in Transmitter mode */
	if (SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_TXE) == SET){

		kservice_fifo_out(&kservice_manage, &out, 1);
		/* Send Transaction data */
		SPI_I2S_SendData(SPI1, out);
		SPI_I2S_ClearITPendingBit(SPI1, SPI_I2S_IT_TXE);
	}
 }

