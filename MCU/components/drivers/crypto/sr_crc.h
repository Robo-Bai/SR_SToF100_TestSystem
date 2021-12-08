#ifndef __SR_CRC_H
#define __SR_CRC_H


#include <stdint.h>

uint32_t sr_reset_crc32(void);
uint32_t sr_cal_crc32(uint32_t *ptr, uint32_t len);

/*
 * ST自带硬件32-bit CRC计算
 *
 * @param[in]	ptr: length of the buffer to be computed
 * @param[in]	len: pointer to the buffer containing the data to be computed
 * @param[out]	None
 *
 * @return		>=0成功，其他失败
 */
uint32_t sr_hw_crc32(uint32_t *ptr, uint32_t len);




#endif /* __SR_CRC_H */
