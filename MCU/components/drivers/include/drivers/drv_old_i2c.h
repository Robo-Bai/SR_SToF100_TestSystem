#ifndef __MYIIC_H__
#define __MYIIC_H__

#include "stm32f4xx.h" 
#include "bsp\include\drv_common.h"

#define SDA_IN()  {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=0<<9*2;}	
#define SDA_OUT() {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=1<<9*2;} 

#define IIC_SCL    PBout(8)
#define IIC_SDA    PBout(9) 
#define READ_SDA   PBin(9) 


void sr_i2c_init(void);               		 
void sr_i2c_start(void);				
void sr_i2c_stop(void);	  		
void sr_i2c_send_byte(uint8_t txd);			
uint8_t sr_i2c_read_byte(unsigned char ack);
uint8_t sr_i2c_wait_ack(void); 				
void sr_i2c_ack(void);					
void sr_i2c_nack(void);				

void IIC_Write_One_Byte(uint8_t daddr,uint8_t addr,uint8_t data);
uint8_t IIC_Read_One_Byte(uint8_t daddr,uint8_t addr);	 

#endif /* __MYIIC_H__ */


