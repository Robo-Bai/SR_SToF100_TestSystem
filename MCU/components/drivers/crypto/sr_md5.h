 /**
 *  Implementation of MD5 hash function. Originally written by Alexander Peslyak. Modified by WaterJuice retaining
 *  Public Domain license,
 *  This is free and unencumbered software released into the public domain - June 2013 waterjuice.org.
 */
 #ifndef __SR_MD5_H__
 #define __SR_MD5_H__

#include <stdint.h>


  
/**
 *  Md5Context an MD5 Context. Use this to initialise/reset a context.
 *  This must be initialised using Md5Initialised,
 *  Do not modify the contents of this structure directly.
 */
typedef struct {
    uint32_t     lo;
    uint32_t     hi;
    uint32_t     a;
    uint32_t     b;
    uint32_t     c;
    uint32_t     d;
    uint8_t      buffer[64];
    uint32_t     block[16];
} Md5Context;

#define MD5_HASH_SIZE           ( 128 / 8 )

typedef struct {
    uint8_t      bytes [MD5_HASH_SIZE];
} MD5_HASH;


/**
 *  Initialises an MD5 Context. Use this to initialise/reset a context.
 *
 * @param[in]	None
 * @param[out]	Context
 *
 * @return		0成功，其他失败
 */
void Md5Initialise(Md5Context*  Context );

/**
 *  Adds data to the MD5 context. This will process the data and update the internal state of the context. Keep on
 *  calling this function until all the data has been added. Then call Md5Finalise to calculate the hash.
 *
 * @param[in]	Context
 * @param[in]	Buffer
 * @param[in]	BufferSize
 * @param[out]	None
 *
 * @return		0成功，其他失败
 */
void Md5Update(Md5Context* Context,  void const*  Buffer,  uint32_t BufferSize);

/**
 *  Performs the final calculation of the hash and returns the digest (16 byte buffer containing 128bit hash). After
 *  calling this, Md5Initialised must be used to reuse the context.
 *
 * @param[in]	Context
 * @param[in]	Digest
 * @param[out]	None
 *
 * @return		0成功，其他失败
 */
void Md5Finalise(Md5Context* Context, MD5_HASH*  Digest);

/**
 * Combines Md5Initialise, Md5Update, and Md5Finalise into one function. Calculates the MD5 hash of the buffer.
 *
 * @param[in]	Buffer
 * @param[in]	BufferSize
 * @param[in]	Digest
 * @param[out]	None
 *
 * @return		0成功，其他失败
 */
void Md5Calculate(void  const* 	Buffer, uint32_t BufferSize, MD5_HASH*	Digest);

#endif /* __SR_MD5_H__ */
