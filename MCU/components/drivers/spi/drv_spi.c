/**
 * Copyright (C) 2019 Standard-robots, Inc
 *
 * @file 	drv_spi.c
 * @author	Neethan <lijun@standard-robots.com> / <neethan@foxmail.com>
 * @version V1.0.0
 * @date 	2020年 2月 12日 星期三 10:50:47 CST
 *
 * @brief    driver spi core 
 */

#include "bsp\include\drv_spi.h"



extern int32_t spi_irq_handler(void);


///< spi1
#define  SR_SPI1                   	SPI1
#define  SR_SPI1_CLK               	RCC_APB2Periph_SPI1
#define  SR_SPI1_GPIO_PORT		 	GPIOB
#define  SR_SPI1_GPIO_CLK          (RCC_AHB1Periph_GPIOB) 
#define  SR_SPI1_SCK_GPIO_PIN       GPIO_Pin_3
#define  SR_SPI1_MISO_GPIO_PIN      GPIO_Pin_4
#define  SR_SPI1_MOSI_GPIO_PIN      GPIO_Pin_5
#define  SR_SPI1_IRQHandler         SPI1_IRQHandler


int32_t spi_bus_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	SPI_I2S_DeInit(SPI1);
	RCC_AHB1PeriphClockCmd(SR_SPI1_GPIO_CLK, ENABLE);
	RCC_APB2PeriphClockCmd(SR_SPI1_CLK, ENABLE);

	GPIO_InitStructure.GPIO_Pin = SR_SPI1_SCK_GPIO_PIN| SR_SPI1_MISO_GPIO_PIN | SR_SPI1_MOSI_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(SR_SPI1_GPIO_PORT, &GPIO_InitStructure);

	GPIO_PinAFConfig(SR_SPI1_GPIO_PORT,GPIO_PinSource3,GPIO_AF_SPI1); //PB3 SPI1
	GPIO_PinAFConfig(SR_SPI1_GPIO_PORT,GPIO_PinSource4,GPIO_AF_SPI1); //PB4 SPI1
	GPIO_PinAFConfig(SR_SPI1_GPIO_PORT,GPIO_PinSource5,GPIO_AF_SPI1); //PB5 SPI1

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  
	SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;		//设置为从机模式
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;	
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;	// 设置为42Mhz	<= 168M/SPI_BaudRatePrescaler_4
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	
	SPI_InitStructure.SPI_CRCPolynomial = 7;	// crc值计算方式
	SPI_Init(SPI1, &SPI_InitStructure);  

	/* 设置SPI1中断 */
	NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Enable the Rx buffer not empty interrupt */
	SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_RXNE, ENABLE);

	/* Enable the Tx empty interrupt */
	SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_TXE, ENABLE);
	
	SPI_Cmd(SPI1, DISABLE); 

    return 0;
}

/*
 * spi1设置传输速度，spi速度=fAPB2/分频系数
 *
 * @param[in]	baudrate_prescaler: SPI_BaudRatePrescaler_2~SPI_BaudRatePrescaler_256 
 * @param[out]	None
 *
 * @return		None
 */
void spi1_set_speed(uint8_t baudrate_prescaler)
{
	assert_param(IS_SPI_BAUDRATE_PRESCALER(baudrate_prescaler));//Ɛ׏ԐЧє
	SPI1->CR1 &= 0XFFC7; // 位3~5清零，用来设置 波特率
	SPI1->CR1 |= baudrate_prescaler;	
	SPI_Cmd(SPI1,ENABLE);
}

void SR_SPI1_IRQHandler(void)       
{
	spi_irq_handler();
} 
