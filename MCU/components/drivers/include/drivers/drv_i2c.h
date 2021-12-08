#ifndef __DRV_I2C_H__
#define __DRV_I2C_H__

#include "drv_common.h" 



void sr_i2c1_init(void);                			 
void sr_i2c_start(void);				
void sr_i2c1_stop(void);	  			
int32_t sr_i2c1_write(uint8_t txd);			
uint8_t sr_i2c1_read(uint8_t ack);	
int32_t sr_i2c1_ack_wait(void); 				
void sr_i2c1_ack(void);				
void sr_i2c1_nack(void);				

#endif /* __DRV_I2C_H__ */
