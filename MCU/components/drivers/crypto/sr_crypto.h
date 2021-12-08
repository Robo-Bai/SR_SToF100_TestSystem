#ifndef __SR_CRYPTO_H__
#define __SR_CRYPTO_H__


#include <stdint.h>


typedef struct {
    uint32_t  total[2];     /*!< number of bytes processed  */
    uint32_t  state[8];     /*!< intermediate digest state  */
    uint8_t   buffer[64];   /*!< data block being processed */

    uint8_t  ipad[64];     /*!< HMAC: inner padding        */
    uint8_t  opad[64];     /*!< HMAC: outer padding        */
    uint8_t  is224;         /*!< 0 => SHA-256, else SHA-224 */
} sha256_context;



uint16_t sr_crc16(const uint8_t *buf, uint32_t len);
uint32_t sr_crc32(const uint8_t* s, uint32_t len);

int32_t sr_sha256_init(sha256_context *hd );
int32_t sr_sha256_update(sha256_context *hd, uint8_t *inbuf, uint32_t inlen);
int32_t sr_sha256_final(sha256_context *hd, uint8_t *strHash);

void uint8_to_int32(int32_t* addr,uint8_t value_h,uint8_t value_mh,uint8_t value_ml,uint8_t value_l);
void uint8_to_int16(int16_t* addr, uint8_t value_h, uint8_t value_l);
void uint8_to_float(float* addr, uint8_t value_h,uint8_t value_mh,uint8_t value_ml,uint8_t value_l);

#endif /* __SR_CRYPTO_API_H__ */

