#ifndef __SR_DRV_24CXX_H__
#define __SR_DRV_24CXX_H__

#include "bsp\include\drv_common.h" 	


#define AT24C01		127
#define AT24C02		255
#define AT24C04		511
#define AT24C08		1023
#define AT24C16		2047
#define AT24C32		4095
#define AT24C64	    8191
#define AT24C128	16383
#define AT24C256	32767  
//Mini STM32开发板使用的是24c02，所以定义EE_TYPE为AT24C02
#define EE_TYPE AT24C02
#define USER_CONFIG_EEPROM_ADDR	0
#define E2P_ADDR_MAX			0xFF
#define DEV_PARA_MAX_LEN		4				  

void sr_at24cxx_init(void);

/*
 * 鍦ˋT24CXX鎸囧畾鍦板潃璇绘暟鎹?
 *
 * @param[in]	addr:璇诲彇鏁版嵁鐨勫湴鍧?
 * @param[in]	num:闇?瑕佽鍙栨暟鎹殑瀛楄妭鏁?
 * @param[out]	val:璇诲彇鍒扮殑鏁版嵁
 *
 * @return		0鎴愬姛锛屽叾浠栧け璐?
 */
int32_t sr_e2p_read(uint16_t addr, uint8_t *val, uint16_t num);
int32_t sr_e2p_write(uint16_t addr, uint8_t *val, uint16_t num);
int32_t sr_e2p_para_read(uint16_t addr, int32_t *val, uint16_t num);

/*
 * 鍐欒澶囧弬鏁?
 *
 * @param[in]	addr:瑕佸啓鍏ヨ澶囧弬鏁扮殑鍦板潃
 * @param[in]	val: 鍙傛暟鍊?
 * @param[in]	num: 鍙傛暟涓暟锛屼互4B涓哄崟浣?
 * @param[out]	None
 *
 * @return		0鎴愬姛锛屽叾浠栧け璐?
 */
int32_t sr_e2p_para_write(uint16_t addr, int32_t *val, uint16_t num);
int32_t test_e2p_para_rw(void);




//void AT24CXX_Write(uint16_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite);	//从指定地址开始写入指定长度的数据
void AT24CXX_Read(uint16_t ReadAddr,uint16_t *pBuffer,uint16_t NumToRead);   	//从指定地址开始读出指定长度的数据

int32_t sr_at24cxx_check(void);  //检查器件

int32_t AT24CXX_DataWrite(uint16_t WriteAddr, uint8_t* DataToWriteAddr);

void AT24CXX_DataWriteFloat(uint16_t WriteAddr, float* DataToWriteAddr);
//void AT24CXX_DataWriteInt(uint16_t WriteAddr,int32_t* DataToWriteAddr);
uint16_t AT24CXX_DataReadFloat(uint16_t ReadAddr,float* DataToReadAddr);
uint16_t AT24CXX_DataReadInt(uint16_t ReadAddr,int32_t* DataToReadAddr);


#endif /* __SR_DRV_24CXX_H__ */

