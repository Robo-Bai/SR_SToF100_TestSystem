/**
 * Copyright (C) 2019 Standard-robots, Inc
 *
 * @file 	old_24cxx.c
 * @author	Neethan <lijun@standard-robots.com> / <neethan@foxmail.com>
 * @version V1.0.0
 * @date 	2019å¹´ 06æœˆ 19æ—¥ æ˜ŸæœŸä¸‰ 10:50:47 CST
 *
 * @brief    at24cxx devices driver.
 */

#include "drivers\mtd\old_24cxx.h" 
	 


#define EE_DEV_ADDR	0xA0	/* 24xx02çš„è®¾å¤‡åœ°å€ */
#define I2C_WR	0		/* å†™æ§åˆ¶bit */
#define I2C_RD	1		/* è¯»æ§åˆ¶bit */

 
void at24cxx_init(void)
{
	sr_i2c_init();
}


uint8_t at24cxx_read_byte(uint16_t ReadAddr)
{				  
	uint8_t temp=0;		  	    																 
    sr_i2c_start();  
	if (EE_TYPE>AT24C16){
		sr_i2c_send_byte(0xA0);	 
		sr_i2c_wait_ack();
		sr_i2c_send_byte(ReadAddr>>8);
	}
	else {
		sr_i2c_send_byte(0XA0+((ReadAddr/256)<<1));     
		sr_i2c_wait_ack(); 
		sr_i2c_send_byte(ReadAddr%256);  
		sr_i2c_wait_ack();	    
		sr_i2c_start();  	 	   
		sr_i2c_send_byte(0xA1);             
		sr_i2c_wait_ack();	 
		temp=sr_i2c_read_byte(0);		   
		sr_i2c_stop();
	}
	return temp;
}

void at24cxx_write_byte(uint16_t WriteAddr,uint8_t DataToWrite)
{		
    sr_i2c_start();  
	if(EE_TYPE>AT24C16)
	{
		sr_i2c_send_byte(0XA0);	  
		sr_i2c_wait_ack();
		sr_i2c_send_byte(WriteAddr>>8);
	} else {
		sr_i2c_send_byte(0xA0+((WriteAddr/256)<<1));    
		sr_i2c_wait_ack();	   
	    sr_i2c_send_byte(WriteAddr%256);  
		sr_i2c_wait_ack(); 	 										  		   
		sr_i2c_send_byte(DataToWrite);    			   
		sr_i2c_wait_ack();  		    	   
	    sr_i2c_stop();
		delay_ms(10);//¸ù¾İAT24C02ÊÖ²átWR(Write Cycle Time) = 5ms max,ÖÁÉÙÑÓÊ±5ms
	}
}



void at24cxx_write_bytes(uint16_t WriteAddr,u32 DataToWrite,uint8_t Len)
{  	
	uint8_t t;
	for(t=0;t<Len;t++)
	{
		at24cxx_write_byte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
	}												    
}


uint32_t at24cxx_read_bytes(uint16_t ReadAddr,uint8_t Len)
{  	
	uint8_t t;
	u32 temp=0;
	for(t=0;t<Len;t++)
	{
		temp<<=8;
		temp+=at24cxx_read_byte(ReadAddr+Len-t-1); 	 				   
	}
	return temp;												    
}


uint8_t at24cxx_check(void)
{
	uint8_t temp;
	temp=at24cxx_read_byte(255);	   
	if (temp==0X55) return 0;		   
	else {
		at24cxx_write_byte(255,0X55);
	    temp=at24cxx_read_byte(255);	  
		if (temp == 0X55) return 0;
	}
	return 1;											  
}

void ee_delay(volatile uint32_t nCount)	
{
	for(; nCount != 0; nCount--);
}


void AT24CXX_Read(uint16_t ReadAddr,uint8_t *pBuffer,uint16_t NumToRead)
{
	while(NumToRead--)
	{
		*pBuffer = at24cxx_read_byte(ReadAddr);
		ReadAddr++;
		pBuffer++;
	}
}  

void AT24CXX_Write(uint16_t WriteAddr,uint8_t *pBuffer,uint16_t NumToWrite)
{
	while(NumToWrite--)
	{
		at24cxx_write_byte(WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}


uint8_t AT24CXX_DataWrite(uint16_t WriteAddr,uint8_t* DataToWriteAddr)
{
	uint8_t i;
	//float x = 0;
	int32_t x = 0;
	uint8_t* addr = DataToWriteAddr;
	uint8_t xor_t = 0;
	for(i=0;i<4;i++)
	{
		xor_t ^= *(addr+i);
	}
	//write
	AT24CXX_Write(WriteAddr,(uint8_t*)(DataToWriteAddr),4);
	at24cxx_write_byte(WriteAddr+4,xor_t);
	//check
	AT24CXX_Read(WriteAddr,(uint8_t*)(&x),4);
	return (*((uint32_t*)&x) == (*DataToWriteAddr)) ? 1 : 0;
}


void AT24CXX_DataWriteFloat(uint16_t WriteAddr,float* DataToWriteAddr)
{
	 if(!AT24CXX_DataWrite(WriteAddr, (uint8_t*)DataToWriteAddr)){
		 AT24CXX_DataWrite(WriteAddr, (uint8_t*)DataToWriteAddr);
	 }
}

void AT24CXX_DataWriteInt(uint16_t WriteAddr,int* DataToWriteAddr)
{
	 if(!AT24CXX_DataWrite(WriteAddr, (uint8_t*)DataToWriteAddr)){
		  AT24CXX_DataWrite(WriteAddr, (uint8_t*)DataToWriteAddr);
	 }
}

uint8_t AT24CXX_DataRead(uint16_t ReadAddr,uint8_t* DataToReadAddr)
{
	uint8_t i;
	uint8_t* addr = (uint8_t*)DataToReadAddr;
	uint8_t xor_t = 0;
	
	AT24CXX_Read(ReadAddr,(uint8_t*)(DataToReadAddr),4);
	for(i=0;i<4;i++)
	{
		xor_t ^= *(addr+i);
	}
	return (xor_t == at24cxx_read_byte(ReadAddr+4)) ? 1 : 0;

}

uint8_t AT24CXX_DataReadFloat(uint16_t ReadAddr,float* DataToReadAddr)
{
	return AT24CXX_DataRead(ReadAddr, (uint8_t*)DataToReadAddr);
}

uint8_t AT24CXX_DataReadInt(uint16_t ReadAddr,int* DataToReadAddr)
{
	return AT24CXX_DataRead(ReadAddr, (uint8_t*)DataToReadAddr);
}



void parameter_read(uint16_t start_addr, uint16_t count, int32_t *values) 
{
	int32_t val = 0;
	uint16_t addr = 0;
	int32_t i = 0;

	// sr_kprintf("parameter read: %d, %d\r\n", start_addr, count);
	for (i = 0; i < count; i++) {
		val = 0;
		addr = USER_CONFIG_EEPROM_ADDR + 5 * (start_addr + i);	
		AT24CXX_DataReadInt(addr, &val);

		//printf("read from addr: %d -> %d\r\n", addr, val);
		values[i] = val;
	}
}

int32_t parameter_read_one(uint16_t addr) 
{
	int32_t value;
	parameter_read(addr, 1, &value);
	return value;
}

// int32_t parameter_write_one(uint16_t addr,) {
//	int32_t value;

//	parameter_read(addr, 1, &value);

//	return value;
//}

void parameter_write(uint16_t start_addr, uint16_t count, const int32_t *values) 
{
	int32_t val = 0;
	uint16_t addr = 0;
	int32_t i = 0;

	// printf("parameter write: %d, %d\r\n", start_addr, count);
	for (i = 0; i < count; i++) {
		val = values[i];
		addr = USER_CONFIG_EEPROM_ADDR + 5 * (start_addr + i);

		//printf("write to addr: %d -> %d\r\n", addr, val);
		AT24CXX_DataWriteInt(addr, &val);
	}	
}


int32_t sr_e2p_para_read(uint16_t addr, int32_t * values, uint16_t count)
{
	parameter_read(addr, count, values);
	return 0;
}

int32_t sr_e2p_para_write(uint16_t addr, int32_t * values, uint16_t count)
{
	parameter_write(addr, count, values);
	return 0;
}
