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
//Mini STM32¿ª·¢°åÊ¹ÓÃµÄÊÇ24c02£¬ËùÒÔ¶¨ÒåEE_TYPEÎªAT24C02
#define EE_TYPE AT24C02
#define USER_CONFIG_EEPROM_ADDR	0
#define E2P_ADDR_MAX			0xFF
#define DEV_PARA_MAX_LEN		4				  

void sr_at24cxx_init(void);

/*
 * åœ¨AT24CXXæŒ‡å®šåœ°å€è¯»æ•°æ®
 *
 * @param[in]	addr:è¯»å–æ•°æ®çš„åœ°å€
 * @param[in]	num:éœ€è¦è¯»å–æ•°æ®çš„å­—èŠ‚æ•°
 * @param[out]	val:è¯»å–åˆ°çš„æ•°æ®
 *
 * @return		0æˆåŠŸï¼Œå…¶ä»–å¤±è´¥
 */
int32_t sr_e2p_read(uint16_t addr, uint8_t *val, uint16_t num);
int32_t sr_e2p_write(uint16_t addr, uint8_t *val, uint16_t num);
int32_t sr_e2p_para_read(uint16_t addr, int32_t *val, uint16_t num);

/*
 * å†™è®¾å¤‡å‚æ•°
 *
 * @param[in]	addr:è¦å†™å…¥è®¾å¤‡å‚æ•°çš„åœ°å€
 * @param[in]	val: å‚æ•°å€¼
 * @param[in]	num: å‚æ•°ä¸ªæ•°ï¼Œä»¥4Bä¸ºå•ä½
 * @param[out]	None
 *
 * @return		0æˆåŠŸï¼Œå…¶ä»–å¤±è´¥
 */
int32_t sr_e2p_para_write(uint16_t addr, int32_t *val, uint16_t num);
int32_t test_e2p_para_rw(void);




//void AT24CXX_Write(uint16_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite);	//´ÓÖ¸¶¨µØÖ·¿ªÊ¼Ğ´ÈëÖ¸¶¨³¤¶ÈµÄÊı¾İ
void AT24CXX_Read(uint16_t ReadAddr,uint16_t *pBuffer,uint16_t NumToRead);   	//´ÓÖ¸¶¨µØÖ·¿ªÊ¼¶Á³öÖ¸¶¨³¤¶ÈµÄÊı¾İ

int32_t sr_at24cxx_check(void);  //¼ì²éÆ÷¼ş

int32_t AT24CXX_DataWrite(uint16_t WriteAddr, uint8_t* DataToWriteAddr);

void AT24CXX_DataWriteFloat(uint16_t WriteAddr, float* DataToWriteAddr);
//void AT24CXX_DataWriteInt(uint16_t WriteAddr,int32_t* DataToWriteAddr);
uint16_t AT24CXX_DataReadFloat(uint16_t ReadAddr,float* DataToReadAddr);
uint16_t AT24CXX_DataReadInt(uint16_t ReadAddr,int32_t* DataToReadAddr);


#endif /* __SR_DRV_24CXX_H__ */

