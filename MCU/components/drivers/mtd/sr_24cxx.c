/*
 * Copyright (C) 2019 Standard-robots, Inc
 *
 * file 	drv_24cxx.c
 * author	Neethan <lijun@standard-robots.com> / <neethan@foxmail.com>
 * date 	2019年 06月 14日 星期五 10:50:47 CST
 *
 * brief    driver at24cxx 
 */

#include <stdio.h>
#include "drivers\mtd\sr_24cxx.h" 
#include "bsp\include\drv_i2c.h" 
#include "bsp\include\drv_uart.h" 
#include "srtos\srtos.h"

#include "sr\app\motion_crl\include\sr_nav_comm.h"


void sr_at24cxx_init(void)
{
	sr_i2c1_init();
}


/*
 * 在AT24CXX指定地址读数据
 *
 * @param[in]	addr:读取数据的地址
 * @param[in]	num:需要读取数据的字节数
 * @param[out]	val:读取到的数据
 *
 * @return		0成功，其他失败
 */
int32_t sr_e2p_read(uint16_t addr, uint8_t *val, uint16_t num)
{  	
	uint8_t i = 0;

	if (NULL == val) return -1;
	
	for(i=0; i<num; i++){
		sr_i2c_start();  
		
		if (EE_TYPE > AT24C16) {
			sr_i2c1_write(0xA0);	   ///< 发送命令
			sr_i2c1_ack_wait();
			sr_i2c1_write((addr+i)>>8);//���͸ߵ�ַ 	
		} else {
			sr_i2c1_write(0xA0+(((addr+i)/256)<<1));   //����������ַ0XA0,д����	   
			sr_i2c1_ack_wait(); 
			sr_i2c1_write((addr+i)%256);   //���͵͵�ַ
			sr_i2c1_ack_wait(); 	
			sr_i2c_start(); 		   
			sr_i2c1_write(0xA1);		   //�������ģʽ			   
			sr_i2c1_ack_wait();  
			*val++ = sr_i2c1_read(0); 	   
			sr_i2c1_stop();//����һ��ֹͣ����		
		}	 				   
	}
	return 0;												    
}


/*
 * 在AT24CXX指定地址写入数据
 *
 * @param[in]	addr:写入数据的地址
 * @param[in]	num:需要写入数据的字节数
 * @param[out]	val:写入的数据
 *
 * @return		0成功，其他失败
 */
int32_t sr_e2p_write(uint16_t addr, uint8_t *val, uint16_t num)
{
	uint8_t i = 0;

	if (NULL == val) return -1;
	
	for(i=0; i<num; i++){
		sr_i2c_start();  
		if (EE_TYPE > AT24C16) {
			sr_i2c1_write(0xA0);		
			sr_i2c1_ack_wait();
			sr_i2c1_write(addr>>8);	
		} 
		else {
			sr_i2c1_write(0xA0+(((addr+i)/256)<<1));   ///< 发送设备地址0xA0,写数据 
			sr_i2c1_ack_wait();    
			sr_i2c1_write((addr+i)%256);	///< 发送低地址
			sr_i2c1_ack_wait(); 													   
			sr_i2c1_write(val[i]); 						   
			sr_i2c1_ack_wait(); 				   
			sr_i2c1_stop();
			delay_ms(8);	///< 根据AT24C02手册tWR(Write Cycle Time) = 5ms max,至少 延时5ms
		}
	}
	return 0;
}


/*
 * 检查AT24CXX是否正常,使用最后一个地址存储标志位
 *
 * @param[in]	None
 * @param[out]	None
 *
 * @return		0成功，其他失败
 */
int32_t sr_at24cxx_check(void)
{
	uint8_t tmp = 0;
	uint8_t flag = 0x55;
	
	sr_e2p_read(0xFF, &tmp, 1); ///< 避免每次开机都写at24cxx
	if (tmp == 0x55){
		return 1;	
	}
	else { 
		/* 排除第一次初始化 */
		sr_e2p_write(0xFF, &flag, 1);
	    sr_e2p_read(0xFF, &tmp, 1);  
		if (tmp == flag)
			return 0;
	}
	return -1;											  
}


int32_t sr_e2p_para_read(uint16_t addr, int32_t *val, uint16_t num)
{
	int32_t val_tmp 	= 0;
	uint16_t addr_tmp 	= 0;
	uint8_t xor_t 		= 0;
	uint8_t xor_res 	= 0;
	uint16_t i, j;

	if (addr > E2P_ADDR_MAX) return -1;

	for (i=0; i<num; i++){
		val_tmp = 0;
		addr_tmp = USER_CONFIG_EEPROM_ADDR + 5 * (addr + i); 
		sr_e2p_read(addr_tmp, (uint8_t *)&val_tmp, DEV_PARA_MAX_LEN);

		//xor
		for (j=0; j<DEV_PARA_MAX_LEN; j++){
			xor_t ^= *(((uint8_t *)&val_tmp)+j);
		}
		if (addr_tmp < E2P_ADDR_MAX){
			sr_e2p_read(addr_tmp + DEV_PARA_MAX_LEN, &xor_res, 1);
			//printf("sr_e2p_para_read, addr_tmp=%x, xor_t=0x%x, xor_res=0x%x, \r\n", addr_tmp, xor_t, xor_res);	
			if (xor_t == xor_res){
				val[i] = val_tmp;
			} else {
				return -2;
			}
		} else {
			sr_e2p_read(E2P_ADDR_MAX, &xor_res, 1);
			val[i] = val_tmp; 
		}
		
	}
	return 0;
}

/*
 * 写设备参数
 *
 * @param[in]	addr:要写入设备参数的地址
 * @param[in]	val: 参数值
 * @param[in]	num: 参数个数，以4B为单位
 * @param[out]	None
 *
 * @return		0成功，其他失败
 */
int32_t sr_e2p_para_write(uint16_t addr, int32_t *val, uint16_t num)
{
	int32_t val_tmp 	= 0;
	uint16_t addr_tmp 	= 0;
	uint16_t i,j 		= 0;
	int32_t res 		= 0;
	uint8_t xor_t 		= 0;

	if (addr > E2P_ADDR_MAX) return -1;
	
	for (i=0; i<num; i++){
		val_tmp = val[i];
		addr_tmp = USER_CONFIG_EEPROM_ADDR + 5 * (addr + i); ///< USER_CONFIG_EEPROM_ADDR + 5 * (addr + i)
		sr_e2p_write(addr_tmp, (uint8_t *)&val_tmp, DEV_PARA_MAX_LEN);
		for (j=0; j<DEV_PARA_MAX_LEN; j++){
			xor_t ^= *(((uint8_t *)(&val_tmp))+j); //< data  +j
		}
		sr_e2p_write(addr_tmp + DEV_PARA_MAX_LEN, &xor_t, 1); ///< 每个参数后一个字节为xor值，用于校验		

		sr_e2p_read(addr_tmp, (uint8_t *)&res, DEV_PARA_MAX_LEN); ///< check
		xor_t = 0;

		if (res == val_tmp){
			xor_t = 0;
			continue;
		} else {
			return -2;
		}
	}
	return 0;
}


#if 0
int32_t test_e2p_para_rw(void)
{
	uint8_t i = 0;
	uint8_t msg[8] = {0x55, 0xaa, 0x11, 0xab};
	uint8_t val[8] = {0};
	int32_t val_tmp[2] = {0};
	int32_t msg_tmp[] = {0x88ccff55, 0x12345678};
	int32_t ret = -1;
	
	sr_e2p_write(0xff, msg, 1);
	sr_e2p_read(0xff, val, 1);
	printf("0x%x\n", val[i]);

	//ret = sr_e2p_para_write(ADDR_ACC_UP, msg_tmp, 2);
	//printf("sr_e2p_para_write, ret=%d \r\n", ret);

	//ret = sr_e2p_para_read(ADDR_ACC_UP, val_tmp, 2);
	//printf("sr_e2p_para_read, ret=%d, val1=0x%x, val2=0x%x\r\n", ret, val_tmp[0], val_tmp[1]);
	
	//	for(i=0; i<4; i++)
	//		printf("0x%x \n", val[i]);

	return 0;
}
#endif

#if 0
//写4个uint8_t数据到EEPROM中
//写4个uint8_t数据(AT24CXX_DataWrite;AT24CXX_DataRead;)的时间为67ms
//EEPROM uint8_t data write
//WriteAddr: EEPROM的地址(以字节为单位),取值范围0-255间能被5整除的整数
//DataToWriteAddr: 要写入到EEPROM中的数据的指针，必须为float类型指针
//返回值：1-写入成功，0-写入失败
int32_t AT24CXX_DataWrite(uint16_t WriteAddr,uint8_t* DataToWriteAddr)
{
	uint8_t i;
	float x;
	//xor
	uint8_t* addr = DataToWriteAddr;
	uint8_t xor_t = 0;
	for(i=0;i<4;i++)
	{
		xor_t ^= *(addr+i);
	}
	//write
	//AT24CXX_Write(WriteAddr,(uint8_t*)(DataToWriteAddr),4);
	//AT24CXX_WriteOneByte(WriteAddr+4,xor_t);
	//check
//	AT24CXX_Read(WriteAddr,(uint8_t*)(&x),4);
	return (x == (*DataToWriteAddr)) ? 0 : -1;
}

//写入float
void AT24CXX_DataWriteFloat(uint16_t WriteAddr,float* DataToWriteAddr)
{
	 if(!AT24CXX_DataWrite(WriteAddr, (uint8_t*)DataToWriteAddr)){
		 AT24CXX_DataWrite(WriteAddr, (uint8_t*)DataToWriteAddr);
	 }
}
#endif

