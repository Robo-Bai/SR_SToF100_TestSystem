/**
 * Copyright (C) 2019 Standard-robots, Inc
 *
 * @file    sr_crc.c
 * @author  Neethan <lijun@standard-robots.com> / <neethan@foxmail.com>
 * @version V1.0.0
 * @date    2020年 11月 23日 星期一 10:50:47 CST
 *
 * @brief  crc.                        
 */

#include "..\bootloader\include\common.h"
#include "..\components\drivers\crypto\sr_crc.h"


uint32_t dwPolynomial = 0x04c11db7;
uint32_t crc_register = 0xFFFFFFFF;    // init

uint32_t sr_reset_crc32(void)
{
	crc_register = 0xFFFFFFFF;
	return 0;
}

uint32_t sr_cal_crc32(uint32_t *ptr, uint32_t len)
{
    uint32_t    xbit;
    uint32_t    data;
   
    while (len--) {
        xbit = (uint32_t)(1 << 31);

        data = *ptr++;
        for (int bits = 0; bits < 32; bits++) {
            if (crc_register & 0x80000000) {
                crc_register <<= 1;
                crc_register ^= dwPolynomial;
            }
            else
                crc_register <<= 1;
            if (data & xbit)
                crc_register ^= dwPolynomial;

            xbit >>= 1;
        }
    }
    return crc_register;
}

/*
 * ST自带硬件32-bit CRC计算
 *
 * @param[in]	ptr: length of the buffer to be computed
 * @param[in]	len: pointer to the buffer containing the data to be computed
 * @param[out]	None
 *
 * @return		>=0成功，其他失败
 */
uint32_t sr_hw_crc32(uint32_t *ptr, uint32_t len)
{
	CRC_ResetDR();
	return CRC_CalcBlockCRC(ptr, len);
}

#if 0
uint32_t sr_cal_crc32(uint32_t *ptr, uint32_t len)
{
    uint32_t    xbit;
    uint32_t    data;
    uint32_t    crc_register = 0xFFFFFFFF;    // init
	
    while (len--) {
        xbit = (uint32_t)(1 << 31);

        data = *ptr++;
        for (int bits = 0; bits < 32; bits++) {
            if (crc_register & 0x80000000) {
                crc_register <<= 1;
                crc_register ^= dwPolynomial;
            }
            else
                crc_register <<= 1;
            if (data & xbit)
                crc_register ^= dwPolynomial;

            xbit >>= 1;
        }
    }
    return crc_register;
}
#endif
