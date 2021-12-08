/*
 * Copyright (C) 2019 Standard-robots, Inc
 *
 * file 	drv_i2c.c
 * author	Neethan <lijun@standard-robots.com> / <neethan@foxmail.com>
 * date 	2019å¹´ 06æœˆ 20æ—¥ æ˜ŸæœŸå›› 10:50:47 CST
 *
 * brief    driver i2c 
 */

#include "bsp\include\drv_i2c.h"


#define  SR_I2C1                   	I2C1
#define  SR_I2C1_CLK               	RCC_APB1Periph_I2C1
#define  SR_I2C1_GPIO_PORT			GPIOB

#define  SR_I2C1_GPIO_CLK          	RCC_AHB1Periph_GPIOB
#define  SR_I2C1_SCL_GPIO_PORT      GPIOB   
#define  SR_I2C1_SCL_GPIO_PIN       GPIO_Pin_8
#define  SR_I2C1_SDA_GPIO_PORT      GPIOB
#define  SR_I2C1_SDA_GPIO_PIN       GPIO_Pin_9

/* PB9ÊäÈëÄ£Ê½ */
#define I2C1_SDA_IN()  {GPIOB->MODER&=~(3<<(9*2)); GPIOB->MODER|=0<<9*2;} 
/* PB9Êä³öÄ£Ê½ */
#define I2C1_SDA_OUT() {GPIOB->MODER&=~(3<<(9*2)); GPIOB->MODER|=1<<9*2;} 

/* SCL */
#define I2C1_SCL		PBout(8) 
/* SDA */
#define I2C1_SDA_WRITE	PBout(9) 	 
#define I2C1_SDA_READ	PBin(9)


void sr_i2c1_init(void)
{			
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(SR_I2C1_GPIO_CLK, ENABLE);

	GPIO_InitStructure.GPIO_Pin = SR_I2C1_SCL_GPIO_PIN | SR_I2C1_SDA_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(SR_I2C1_GPIO_PORT, &GPIO_InitStructure);
	
	I2C1_SCL = 1;
	I2C1_SDA_WRITE = 1;
}

void sr_i2c_start(void)
{
	I2C1_SDA_OUT();     //sdaÏßÊä³ö
	I2C1_SDA_WRITE = 1;	  	  
	I2C1_SCL = 1;
	//_nop_();
	delay_us(4);
 	I2C1_SDA_WRITE = 0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	I2C1_SCL = 0;//Ç¯×¡I2C×ÜÏß£¬×¼±¸·¢ËÍ»ò½ÓÊÕÊý¾Ý 
}	  

void sr_i2c1_stop(void)
{
	I2C1_SDA_OUT();//sdaÏßÊä³ö
	I2C1_SCL = 0;
	I2C1_SDA_WRITE = 0;//STOP:when CLK is high DATA change form low to high
	delay_us(4);
	I2C1_SCL = 1; 
	I2C1_SDA_WRITE = 1;//·¢ËÍI2C×ÜÏß½áÊøÐÅºÅ
	delay_us(4);							   	
}

//µÈ´ýÓ¦´ðÐÅºÅµ½À´
//·µ»ØÖµ£º1£¬½ÓÊÕÓ¦´ðÊ§°Ü
//        0£¬½ÓÊÕÓ¦´ð³É¹¦
int32_t sr_i2c1_ack_wait(void)
{
	uint8_t ucErrTime = 0;
	
	I2C1_SDA_IN();      //SDAÉèÖÃÎªÊäÈë  
	I2C1_SDA_WRITE = 1;
	delay_us(1);	   
	I2C1_SCL = 1;
	delay_us(1);
	
	while(I2C1_SDA_READ) {
		ucErrTime++;
		if(ucErrTime > 250) {
			sr_i2c1_stop();
			return 1;
		}
	}
	I2C1_SCL = 0;//Ê±ÖÓÊä³ö0 	   
	return 0;  
} 

//²úÉúACKÓ¦´ð
void sr_i2c1_ack(void)
{
	I2C1_SCL = 0;
	I2C1_SDA_OUT();
	I2C1_SDA_WRITE = 0;
	delay_us(2);
	I2C1_SCL = 1;
	delay_us(2);
	I2C1_SCL = 0;
}

//²»²úÉúACKÓ¦´ð		    
void sr_i2c1_nack(void)
{
	I2C1_SCL = 0;
	I2C1_SDA_OUT();
	I2C1_SDA_WRITE = 1;
	delay_us(2);
	I2C1_SCL = 1;
	delay_us(2);
	I2C1_SCL = 0;
}

//IIC·¢ËÍÒ»¸ö×Ö½Ú
//·µ»Ø´Ó»úÓÐÎÞÓ¦´ð
//1£¬ÓÐÓ¦´ð
//0£¬ÎÞÓ¦´ð			  
int32_t sr_i2c1_write(uint8_t txd)
{                        
    uint8_t t;   
	I2C1_SDA_OUT(); 	    
    I2C1_SCL = 0;//À­µÍÊ±ÖÓ¿ªÊ¼Êý¾Ý´«Êä
    for (t=0; t<8; t++) {              
        I2C1_SDA_WRITE=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(2);   //¶ÔTEA5767ÕâÈý¸öÑÓÊ±¶¼ÊÇ±ØÐëµÄ
		I2C1_SCL = 1;
		delay_us(2); 
		I2C1_SCL = 0;	
		delay_us(2);
    }	 
	return 0;
} 	    
//¶Á1¸ö×Ö½Ú£¬ack=1Ê±£¬·¢ËÍACK£¬ack=0£¬·¢ËÍnACK   
uint8_t sr_i2c1_read(uint8_t ack)
{
	uint8_t i, receive = 0;
	
	I2C1_SDA_IN();
    for (i=0; i<8; i++ ){
        I2C1_SCL = 0; 
        delay_us(2);
		I2C1_SCL = 1;
        receive <<= 1;
        if(I2C1_SDA_READ) receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        sr_i2c1_nack();//·¢ËÍnACK
    else
        sr_i2c1_ack(); //·¢ËÍACK   
    return receive;
}
