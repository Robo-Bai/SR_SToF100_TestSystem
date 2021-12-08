#ifndef __24CXX_H__
#define __24CXX_H__

#include <stdint.h>
#include "bsp\include\drv_old_i2c.h"   


#define AT24C01		127
#define AT24C02		255
#define AT24C04		511
#define AT24C08		1023
#define AT24C16		2047
#define AT24C32		4095
#define AT24C64	    8191
#define AT24C128	16383
#define AT24C256	32767  

#if defined (SR_VC400)
#define EE_TYPE 	AT24C32
#elif defined (SR_VC300)
#define EE_TYPE 	AT24C02
#endif

#define USER_CONFIG_EEPROM_ADDR		0
					  
uint8_t at24cxx_read_byte(u16 ReadAddr);							
void at24cxx_write_byte(u16 WriteAddr,uint8_t DataToWrite);		
void at24cxx_write_bytes(u16 WriteAddr,u32 DataToWrite,uint8_t Len);
u32 AT24CXX_ReadLenByte(u16 ReadAddr,uint8_t Len);					
void AT24CXX_Write(u16 WriteAddr,uint8_t *pBuffer,u16 NumToWrite);	
void AT24CXX_Read(u16 ReadAddr,uint8_t *pBuffer,u16 NumToRead);   	

uint8_t at24cxx_check(void);  
void at24cxx_init(void); 

uint8_t AT24CXX_DataWrite(u16 WriteAddr,uint8_t* DataToWriteAddr);
uint8_t AT24CXX_DataRead(u16 ReadAddr,uint8_t* DataToReadAddr);
void AT24CXX_DataWriteFloat(u16 WriteAddr,float* DataToWriteAddr);
void AT24CXX_DataWriteInt(u16 WriteAddr,int* DataToWriteAddr);
uint8_t AT24CXX_DataReadFloat(u16 ReadAddr,float* DataToReadAddr);
uint8_t AT24CXX_DataReadInt(u16 ReadAddr,int* DataToReadAddr);

void parameter_read(uint16_t start_addr, uint16_t count, int32_t *values);
int32_t parameter_read_one(uint16_t addr);
void parameter_write(uint16_t start_addr, uint16_t count, const int32_t *values);

int32_t sr_e2p_para_read(uint16_t addr, int32_t * values, uint16_t count);
int32_t sr_e2p_para_write(uint16_t addr, int32_t * values, uint16_t count);

#endif /* __24CXX_H__ */
