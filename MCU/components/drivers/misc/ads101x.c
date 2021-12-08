/**
 * Copyright (C) 2019 Standard-robots, Inc
 *
 * @file 	ads101x.c
 * @author	Neethan <lijun@standard-robots.com> / <neethan@foxmail.com>
 * @version V1.0.0
 * @date 	2020年 2月 13日 星期四 10:50:47 CST
 *
 * @brief    driver ads101x.
 */

#include <string.h>
#include "drivers\include\drivers\ads101x.h"



#define SCL_PORT		GPIOA
#define SCL_PIN			(GPIO_Pin_8)  
#define SDA_PORT		GPIOC
#define SDA_PIN			(GPIO_Pin_9)  
#define RCC_I2C3_PORT	(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOA)


#define SCL_HIGH()       SCL_PORT->BSRRL = GPIO_Pin_8 
#define SCL_LOW()        SCL_PORT->BSRRH = GPIO_Pin_8 

#define SDA_HIGH()       SDA_PORT->BSRRL = GPIO_Pin_9 
#define SDA_LOW()        SDA_PORT->BSRRH  = GPIO_Pin_9   
#define GetSdaPinValue() (SDA_PORT->IDR  & GPIO_Pin_9)


#if 0
/* ADS1015 registers */
enum {
	ADS1015_CONVERSION = 0,
	ADS1015_CONFIG = 1,
};

/* PGA fullscale voltages in mV */
static const unsigned int fullscale_table[8] = {
	6144, 4096, 2048, 1024, 512, 256, 256, 256 };

/* Data rates in samples per second */
static const unsigned int data_rate_table_1015[8] = {
	128, 250, 490, 920, 1600, 2400, 3300, 3300
};


#define ADS1015_DEFAULT_CHANNELS 0xff
#define ADS1015_DEFAULT_PGA 2
#define ADS1015_DEFAULT_DATA_RATE 4

enum ads1015_chips {
	ads1015,
	ads1115,
};


static int ads1015_read_adc( unsigned int channel)
{

	return 0;
}

static void ads1015_get_channels_config()
{

}
#endif


static void scl_out(void) 
{   
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;		
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	

	GPIO_InitStructure.GPIO_Pin = SCL_PIN;
	GPIO_Init(SCL_PORT, &GPIO_InitStructure);
}

static void sda_out(void) 
{  
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);	
	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;		
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	

	GPIO_InitStructure.GPIO_Pin = SDA_PIN;
	GPIO_Init(SDA_PORT, &GPIO_InitStructure);
}

static void sda_in(void) 
{ 
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);	

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;		
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	

	GPIO_InitStructure.GPIO_Pin = SDA_PIN;
	GPIO_Init(SDA_PORT, &GPIO_InitStructure);
}


void ads101x_init(void) 
{
    sda_out();
    scl_out();
    SCL_HIGH();
    SDA_HIGH();
}

static void soft_i2c_start(void) 
{
    sda_out();
	SDA_HIGH();
	SCL_HIGH();
    delay_us(2);
    SDA_LOW();
    delay_us(2); 
    SCL_LOW();
}

static void soft_i2c_stop(void) 
{
    sda_out();
	SDA_LOW();
	SCL_LOW();
	delay_us(2);
	SCL_HIGH();
    delay_us(2);
    SDA_HIGH();
    delay_us(2);
}


static int soft_i2c_wait_ack(void) 
{
    uint8_t cErrTime = 255;
	
    sda_in();
	SDA_HIGH();
    delay_us(1);
	SCL_HIGH(); 
    delay_us(1);
    
    while (GetSdaPinValue()) {
		cErrTime--;
		delay_us(1);
		if (cErrTime == 0) {
			 soft_i2c_stop();
			 return -1;
		}
   }
    SCL_LOW();
    return 0;
}


static void soft_i2c_send_ack(void) 
{
    SCL_LOW();
    sda_out();
    SDA_LOW();
	
    delay_us(2);
    SCL_HIGH();
    delay_us(2);
    SCL_LOW();
}


static void soft_i2c_send_nack(void) 
{
	SCL_LOW();
	sda_out();
    SDA_HIGH();
    delay_us(100);
    SCL_HIGH();
    delay_us(2);
    SCL_LOW();
}

static void soft_i2c_send_byte(uint8_t byte) 
{
    uint8_t i = 8;
    sda_out();
    SCL_LOW();
    
    for (i = 0; i < 8; i++) {
	      
		if (byte & 0x80) {
		  SDA_HIGH();
		} 
		else {
		  SDA_LOW();	
		}
		delay_us(2);
		byte <<= 1;
		SCL_HIGH();
		delay_us(2);
		SCL_LOW();
		delay_us(2);
    }
    SCL_LOW();
}

static uint8_t soft_i2c_recv_byte(void) 
{
	uint8_t i = 0;
	uint8_t byte = 0;

	sda_in();
	SDA_HIGH();

	for (i = 0; i < 8; i++) {
		byte <<= 1;
		SCL_LOW();
		delay_us(2);
		SCL_HIGH();
		delay_us(1);
		if (GetSdaPinValue()) {
			byte++;
		}   
	}
	SCL_LOW();
	return byte;
}


void SoftwareI2C_Init(void) 
{
    sda_out();
    scl_out();
    SCL_HIGH();
    SDA_HIGH();
}

bool soft_i2c_write(uint8_t devAddr, uint8_t *writeBuff, uint8_t writeCount)
{
	uint8_t i;
	
	soft_i2c_start();
	
	 soft_i2c_send_byte(devAddr);
    if (soft_i2c_wait_ack())
    {
        soft_i2c_stop();
        return false;
    }
	
	for(i = 0; i < writeCount; i++)
	{
		soft_i2c_send_byte(writeBuff[i]);
		if (soft_i2c_wait_ack())
		{
			soft_i2c_stop();
		    return false;
		}
	}
	
	soft_i2c_stop();
	return true;
}


bool soft_i2c_write_reg(uint8_t devAddr, uint8_t regAddr, uint8_t *writeBuff, uint8_t writeCount)
{
	uint8_t i = 0;
	
	soft_i2c_start();

    soft_i2c_send_byte(devAddr);
    if (soft_i2c_wait_ack()) {
        soft_i2c_stop();
        return false;
    }
	
	soft_i2c_send_byte(regAddr);
    if (soft_i2c_wait_ack()){
        soft_i2c_stop();
        return false;
    }

	for(i = 0; i < writeCount; i++) {
		soft_i2c_send_byte(writeBuff[i]);
		if (soft_i2c_wait_ack())
		{
			soft_i2c_stop();
		    return false;
		}
	}
	
	soft_i2c_stop();
	return true;
}

bool soft_i2c_read(uint8_t devAddr, uint8_t *readBuff, uint8_t readCount)
{
	uint8_t i = 0;
	
	soft_i2c_start();
    soft_i2c_send_byte(devAddr);
    if (soft_i2c_wait_ack())
    {
        soft_i2c_stop();
        return false;
    }
	soft_i2c_stop();
	
	soft_i2c_start();
	soft_i2c_send_byte(devAddr | 0x01);
    if (soft_i2c_wait_ack())
    {
        soft_i2c_stop();
        return false;
    }
	
	for(i = 0; i < (readCount - 1); i++)
	{
		readBuff[i] = soft_i2c_recv_byte();
		soft_i2c_send_ack();
	}
	
	readBuff[i] = soft_i2c_recv_byte();
    soft_i2c_send_nack();
	delay_us(2);
	soft_i2c_stop();

    return true;
}

bool soft_i2c_read_reg(uint8_t devAddr, uint8_t regAddr, uint8_t *readBuff, uint8_t readCount)
{
	uint8_t i = 0;
	
	soft_i2c_start();
	
    soft_i2c_send_byte(devAddr);
    if (soft_i2c_wait_ack())
    {
        soft_i2c_stop();
        return false;
    }
	
	soft_i2c_send_byte(regAddr);
    if (soft_i2c_wait_ack())
    {
        soft_i2c_stop();
        return false;
    }
	soft_i2c_stop();
	
	soft_i2c_start();
	soft_i2c_send_byte(devAddr | 0x01);
    if (soft_i2c_wait_ack())
    {
        soft_i2c_stop();
        return false;
    }
	
	for(i = 0; i < (readCount - 1); i++)
	{
		readBuff[i] = soft_i2c_recv_byte();
		soft_i2c_send_ack();
	}
	
	readBuff[i] = soft_i2c_recv_byte();
    soft_i2c_send_nack();
	soft_i2c_stop();

    return true;
}

bool ads1015_point_set(uint8_t devAddr, uint8_t write_address) 
{  
	soft_i2c_start();
	soft_i2c_send_byte(devAddr);
    if (soft_i2c_wait_ack())
    {
        soft_i2c_stop();
        return false;
    }
	soft_i2c_send_byte(write_address);
    if (soft_i2c_wait_ack())
    {
        soft_i2c_stop();
        return false;
    }  
	soft_i2c_stop();   
	return true;
} 

int32_t adc_cur_read(uint8_t ain, uint16_t* current)
{
	/* 单次转换
	 * ain0 - 0xc1
	 * ain1 - 0xd1
	 * ain2 - 0xe1
	 * ain3 - 0xf1
	 */

	/* 连续转换
	 * ain0 - 0x42
	 * ain1 - 0x52
	 * ain2 - 0x62
	 * ain3 - 0x72
	 */

	// STM32 MCU 大端存储
	uint16_t cmd_ain[4] = {0x8342, 0x8352, 0x8362, 0x8372};
	bool ret = 0;
	uint8_t read_buf[5];
	uint16_t tmp = 0;
	
	if (ain == 0){
		ret = soft_i2c_write_reg(0x90, 0x01, (uint8_t*)&cmd_ain[0], 2);
	} else if (ain == 1) {
		ret = soft_i2c_write_reg(0x90, 0x01, (uint8_t*)&cmd_ain[1], 2);
	}  else if (ain == 2) {
		ret = soft_i2c_write_reg(0x90, 0x01, (uint8_t*)&cmd_ain[2], 2);
	}  else if (ain == 3) {
		ret = soft_i2c_write_reg(0x90, 0x01, (uint8_t*)&cmd_ain[3], 2);
	} else {
		sr_kprintf("adsa0ax no this reg\r\n");
	}
	sr_kprintf("ret=%d, soft_i2c_write_reg\r\n", ret);
	memset(read_buf, 0, sizeof(read_buf));
	ret = soft_i2c_read_reg(0x90, 0x01, read_buf, 2);
	tmp = (read_buf[0]<< 8 | read_buf[1]);
	sr_kprintf("\r\nret=%d, cfg=0x%x\r\n", ret, tmp);
		

	memset(read_buf, 0, sizeof(read_buf));
	ret = soft_i2c_read_reg(0x90, 0x00, read_buf, 2);
	tmp = (uint16_t)(((uint16_t)(read_buf[0]<< 8 | read_buf[1]) & 0xfff0)>>4)*2; // 配置分辨率2mv
	
	tmp *= 5; /* 换算成实际电流mA */
	*current = tmp;
	sr_kprintf("ret=%d, adc=>val=%d mv\r\n", ret, tmp);
	
	return 0;
}

int32_t adc_ads101x_test(void)
{
	/* 单次转换
	 * ain0 - 0xc1
	 * ain1 - 0xd1
	 * ain2 - 0xe1
	 * ain3 - 0xf1
	 */

	/* 连续转换
	 * ain0 - 0x42
	 * ain1 - 0x52
	 * ain2 - 0x62
	 * ain3 - 0x72
	 */

	// STM32 MCU 大端存储
//	uint16_t writeBuff = 0x8372;
	bool ret = 0;
	uint8_t read_buf[5];
	uint16_t tmp = 0;

	sr_kprintf("\r\nadc_ads101x_test\r\n");

	//ret =  soft_i2c_write_reg(0x90, 0x01, (uint8_t*)&writeBuff, 2);
	//sr_kprintf("ret=%d, soft_i2c_write_reg\r\n", ret);
	
	memset(read_buf, 0, sizeof(read_buf));
	ret = soft_i2c_read_reg(0x90, 0x01, read_buf, 2);
	tmp = (read_buf[0]<< 8 | read_buf[1]);
	sr_kprintf("\r\nret=%d, cfg=0x%x\r\n", ret, tmp);

	memset(read_buf, 0, sizeof(read_buf));
	ret = soft_i2c_read_reg(0x90, 0x00, read_buf, 2);
	
	tmp = (uint16_t)(((uint16_t)(read_buf[0]<< 8 | read_buf[1]) & 0xfff0)>>4)*2; // 配置分辨率2mv
	sr_kprintf("ret=%d, adc=>val=%d mv\r\n", ret, tmp);

	return 0;
}

