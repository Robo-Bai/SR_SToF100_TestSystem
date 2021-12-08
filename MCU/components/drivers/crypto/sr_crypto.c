/**
 * Copyright (C) 2019 Standard-robots, Inc
 *
 * @file 	sr_crypto.c
 * @author	Neethan <lijun@standard-robots.com> / <neethan@foxmail.com>
 * @version V1.0.0
 * @date 	2019年 06月 19日 星期三 10:50:47 CST
 *
 * @brief    crypto.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "drivers\crypto\sr_crypto.h"
#include "srtos\err_code.h"




/* CRC16 implementation according to CCITT standards.
 *
 * Note by @antirez: this is actually the XMODEM CRC 16 algorithm, using the
 * following parameters:
 *
 * Name                       : "XMODEM", also known as "ZMODEM", "CRC-16/ACORN"
 * Width                      : 16 bit
 * Poly                       : 1021 (That is actually x^16 + x^12 + x^5 + 1)
 * Initialization             : 0000
 * Reflect Input byte         : False
 * Reflect Output CRC         : False
 * Xor constant to output CRC : 0000
 * Output for "123456789"     : 31C3
 */
 #ifdef SR_USING_CRC16_XMODEM
static const uint16_t crc16_tab[256] = 
{
    0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,
    0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef,
    0x1231,0x0210,0x3273,0x2252,0x52b5,0x4294,0x72f7,0x62d6,
    0x9339,0x8318,0xb37b,0xa35a,0xd3bd,0xc39c,0xf3ff,0xe3de,
    0x2462,0x3443,0x0420,0x1401,0x64e6,0x74c7,0x44a4,0x5485,
    0xa56a,0xb54b,0x8528,0x9509,0xe5ee,0xf5cf,0xc5ac,0xd58d,
    0x3653,0x2672,0x1611,0x0630,0x76d7,0x66f6,0x5695,0x46b4,
    0xb75b,0xa77a,0x9719,0x8738,0xf7df,0xe7fe,0xd79d,0xc7bc,
    0x48c4,0x58e5,0x6886,0x78a7,0x0840,0x1861,0x2802,0x3823,
    0xc9cc,0xd9ed,0xe98e,0xf9af,0x8948,0x9969,0xa90a,0xb92b,
    0x5af5,0x4ad4,0x7ab7,0x6a96,0x1a71,0x0a50,0x3a33,0x2a12,
    0xdbfd,0xcbdc,0xfbbf,0xeb9e,0x9b79,0x8b58,0xbb3b,0xab1a,
    0x6ca6,0x7c87,0x4ce4,0x5cc5,0x2c22,0x3c03,0x0c60,0x1c41,
    0xedae,0xfd8f,0xcdec,0xddcd,0xad2a,0xbd0b,0x8d68,0x9d49,
    0x7e97,0x6eb6,0x5ed5,0x4ef4,0x3e13,0x2e32,0x1e51,0x0e70,
    0xff9f,0xefbe,0xdfdd,0xcffc,0xbf1b,0xaf3a,0x9f59,0x8f78,
    0x9188,0x81a9,0xb1ca,0xa1eb,0xd10c,0xc12d,0xf14e,0xe16f,
    0x1080,0x00a1,0x30c2,0x20e3,0x5004,0x4025,0x7046,0x6067,
    0x83b9,0x9398,0xa3fb,0xb3da,0xc33d,0xd31c,0xe37f,0xf35e,
    0x02b1,0x1290,0x22f3,0x32d2,0x4235,0x5214,0x6277,0x7256,
    0xb5ea,0xa5cb,0x95a8,0x8589,0xf56e,0xe54f,0xd52c,0xc50d,
    0x34e2,0x24c3,0x14a0,0x0481,0x7466,0x6447,0x5424,0x4405,
    0xa7db,0xb7fa,0x8799,0x97b8,0xe75f,0xf77e,0xc71d,0xd73c,
    0x26d3,0x36f2,0x0691,0x16b0,0x6657,0x7676,0x4615,0x5634,
    0xd94c,0xc96d,0xf90e,0xe92f,0x99c8,0x89e9,0xb98a,0xa9ab,
    0x5844,0x4865,0x7806,0x6827,0x18c0,0x08e1,0x3882,0x28a3,
    0xcb7d,0xdb5c,0xeb3f,0xfb1e,0x8bf9,0x9bd8,0xabbb,0xbb9a,
    0x4a75,0x5a54,0x6a37,0x7a16,0x0af1,0x1ad0,0x2ab3,0x3a92,
    0xfd2e,0xed0f,0xdd6c,0xcd4d,0xbdaa,0xad8b,0x9de8,0x8dc9,
    0x7c26,0x6c07,0x5c64,0x4c45,0x3ca2,0x2c83,0x1ce0,0x0cc1,
    0xef1f,0xff3e,0xcf5d,0xdf7c,0xaf9b,0xbfba,0x8fd9,0x9ff8,
    0x6e17,0x7e36,0x4e55,0x5e74,0x2e93,0x3eb2,0x0ed1,0x1ef0
};

uint16_t sr_crc16(const uint8_t *buf, uint32_t len)
{
    uint32_t counter 	= 0;
    uint16_t crc 		= 0;
	
	if (NULL == buf) return 0;
	
    for (counter = 0; counter < len; counter++)
            crc = (crc<<8) ^ crc16_tab[((crc>>8) ^ *buf++)&0x00FF];
    return crc;
}

#endif /* SR_USING_CRC16_XMODEM */


#ifdef SR_USING_CRC32
static const uint32_t crc32_tab[] =
{
    0x00000000L, 0x77073096L, 0xee0e612cL, 0x990951baL, 0x076dc419L,
    0x706af48fL, 0xe963a535L, 0x9e6495a3L, 0x0edb8832L, 0x79dcb8a4L,
    0xe0d5e91eL, 0x97d2d988L, 0x09b64c2bL, 0x7eb17cbdL, 0xe7b82d07L,
    0x90bf1d91L, 0x1db71064L, 0x6ab020f2L, 0xf3b97148L, 0x84be41deL,
    0x1adad47dL, 0x6ddde4ebL, 0xf4d4b551L, 0x83d385c7L, 0x136c9856L,
    0x646ba8c0L, 0xfd62f97aL, 0x8a65c9ecL, 0x14015c4fL, 0x63066cd9L,
    0xfa0f3d63L, 0x8d080df5L, 0x3b6e20c8L, 0x4c69105eL, 0xd56041e4L,
    0xa2677172L, 0x3c03e4d1L, 0x4b04d447L, 0xd20d85fdL, 0xa50ab56bL,
    0x35b5a8faL, 0x42b2986cL, 0xdbbbc9d6L, 0xacbcf940L, 0x32d86ce3L,
    0x45df5c75L, 0xdcd60dcfL, 0xabd13d59L, 0x26d930acL, 0x51de003aL,
    0xc8d75180L, 0xbfd06116L, 0x21b4f4b5L, 0x56b3c423L, 0xcfba9599L,
    0xb8bda50fL, 0x2802b89eL, 0x5f058808L, 0xc60cd9b2L, 0xb10be924L,
    0x2f6f7c87L, 0x58684c11L, 0xc1611dabL, 0xb6662d3dL, 0x76dc4190L,
    0x01db7106L, 0x98d220bcL, 0xefd5102aL, 0x71b18589L, 0x06b6b51fL,
    0x9fbfe4a5L, 0xe8b8d433L, 0x7807c9a2L, 0x0f00f934L, 0x9609a88eL,
    0xe10e9818L, 0x7f6a0dbbL, 0x086d3d2dL, 0x91646c97L, 0xe6635c01L,
    0x6b6b51f4L, 0x1c6c6162L, 0x856530d8L, 0xf262004eL, 0x6c0695edL,
    0x1b01a57bL, 0x8208f4c1L, 0xf50fc457L, 0x65b0d9c6L, 0x12b7e950L,
    0x8bbeb8eaL, 0xfcb9887cL, 0x62dd1ddfL, 0x15da2d49L, 0x8cd37cf3L,
    0xfbd44c65L, 0x4db26158L, 0x3ab551ceL, 0xa3bc0074L, 0xd4bb30e2L,
    0x4adfa541L, 0x3dd895d7L, 0xa4d1c46dL, 0xd3d6f4fbL, 0x4369e96aL,
    0x346ed9fcL, 0xad678846L, 0xda60b8d0L, 0x44042d73L, 0x33031de5L,
    0xaa0a4c5fL, 0xdd0d7cc9L, 0x5005713cL, 0x270241aaL, 0xbe0b1010L,
    0xc90c2086L, 0x5768b525L, 0x206f85b3L, 0xb966d409L, 0xce61e49fL,
    0x5edef90eL, 0x29d9c998L, 0xb0d09822L, 0xc7d7a8b4L, 0x59b33d17L,
    0x2eb40d81L, 0xb7bd5c3bL, 0xc0ba6cadL, 0xedb88320L, 0x9abfb3b6L,
    0x03b6e20cL, 0x74b1d29aL, 0xead54739L, 0x9dd277afL, 0x04db2615L,
    0x73dc1683L, 0xe3630b12L, 0x94643b84L, 0x0d6d6a3eL, 0x7a6a5aa8L,
    0xe40ecf0bL, 0x9309ff9dL, 0x0a00ae27L, 0x7d079eb1L, 0xf00f9344L,
    0x8708a3d2L, 0x1e01f268L, 0x6906c2feL, 0xf762575dL, 0x806567cbL,
    0x196c3671L, 0x6e6b06e7L, 0xfed41b76L, 0x89d32be0L, 0x10da7a5aL,
    0x67dd4accL, 0xf9b9df6fL, 0x8ebeeff9L, 0x17b7be43L, 0x60b08ed5L,
    0xd6d6a3e8L, 0xa1d1937eL, 0x38d8c2c4L, 0x4fdff252L, 0xd1bb67f1L,
    0xa6bc5767L, 0x3fb506ddL, 0x48b2364bL, 0xd80d2bdaL, 0xaf0a1b4cL,
    0x36034af6L, 0x41047a60L, 0xdf60efc3L, 0xa867df55L, 0x316e8eefL,
    0x4669be79L, 0xcb61b38cL, 0xbc66831aL, 0x256fd2a0L, 0x5268e236L,
    0xcc0c7795L, 0xbb0b4703L, 0x220216b9L, 0x5505262fL, 0xc5ba3bbeL,
    0xb2bd0b28L, 0x2bb45a92L, 0x5cb36a04L, 0xc2d7ffa7L, 0xb5d0cf31L,
    0x2cd99e8bL, 0x5bdeae1dL, 0x9b64c2b0L, 0xec63f226L, 0x756aa39cL,
    0x026d930aL, 0x9c0906a9L, 0xeb0e363fL, 0x72076785L, 0x05005713L,
    0x95bf4a82L, 0xe2b87a14L, 0x7bb12baeL, 0x0cb61b38L, 0x92d28e9bL,
    0xe5d5be0dL, 0x7cdcefb7L, 0x0bdbdf21L, 0x86d3d2d4L, 0xf1d4e242L,
    0x68ddb3f8L, 0x1fda836eL, 0x81be16cdL, 0xf6b9265bL, 0x6fb077e1L,
    0x18b74777L, 0x88085ae6L, 0xff0f6a70L, 0x66063bcaL, 0x11010b5cL,
    0x8f659effL, 0xf862ae69L, 0x616bffd3L, 0x166ccf45L, 0xa00ae278L,
    0xd70dd2eeL, 0x4e048354L, 0x3903b3c2L, 0xa7672661L, 0xd06016f7L,
    0x4969474dL, 0x3e6e77dbL, 0xaed16a4aL, 0xd9d65adcL, 0x40df0b66L,
    0x37d83bf0L, 0xa9bcae53L, 0xdebb9ec5L, 0x47b2cf7fL, 0x30b5ffe9L,
    0xbdbdf21cL, 0xcabac28aL, 0x53b39330L, 0x24b4a3a6L, 0xbad03605L,
    0xcdd70693L, 0x54de5729L, 0x23d967bfL, 0xb3667a2eL, 0xc4614ab8L,
    0x5d681b02L, 0x2a6f2b94L, 0xb40bbe37L, 0xc30c8ea1L, 0x5a05df1bL,
    0x2d02ef8dL
};


uint32_t sr_crc32(const uint8_t* s, uint32_t len)
{
    uint32_t i 		= 0;
    uint32_t crc32val = 0;
	
    crc32val ^= 0xFFFFFFFF;

    for (i = 0;  i < len;  i++) {
        crc32val = crc32_tab[(crc32val ^ s[i]) & 0xFF] ^ ((crc32val >> 8) & 0x00FFFFFF);
    }

    return labs(crc32val ^ 0xFFFFFFFF);
}
#endif /* SR_USING_CRC32 */


/**
 * 将4字节HEX转换为32位无符号整型
 *
 * @param[in]	HexBuf: 输入缓冲区
 * @param[out]	None
 *
 * @return		返回转换后的数值,出错返回0
 */
static uint32_t sr_hex2int(const uint8_t *HexBuf)
{
	return ((HexBuf[0] << 24) + (HexBuf[1] << 16) + (HexBuf[2] << 8) + HexBuf[3]);
}


/**
 * 将32位无符号整型转换为4字节Hex数据
 *
 * @param[in]	int_num: 输入无符号整型数
 * @param[out]	HexBuf:输出Hex缓冲区(缓冲大小必须为4字节以上)
 *
 * @return		0成功，其他失败
 */
static int32_t sr_int2hex(uint32_t int_num, uint8_t *HexBuf)
{
	HexBuf[0] = int_num >> 24;
	HexBuf[1] = int_num >> 16;
	HexBuf[2] = int_num >> 8;
	HexBuf[3] = int_num;

	return E_OK;
}

void uint8_to_int32(int32_t* addr, uint8_t value_h,uint8_t value_mh,uint8_t value_ml,uint8_t value_l)
{
	uint8_t* pointer;
	pointer = (uint8_t*)(addr);
	
	*pointer = value_h;
	*(pointer+1) = value_mh;
	*(pointer+2) = value_ml;
	*(pointer+3) = value_l;
}

void uint8_to_int16(int16_t* addr, uint8_t value_h, uint8_t value_l)
{
	uint8_t* pointer;
	pointer = (uint8_t*)(addr);

	*pointer = value_h;
	*(pointer+1) = value_l;
}

void uint8_to_float(float* addr, uint8_t value_h,uint8_t value_mh,uint8_t value_ml,uint8_t value_l)
{
	uint8_t* pointer;
	pointer = (uint8_t*)(addr);
	
	*pointer = value_h;
	*(pointer+1) = value_mh;
	*(pointer+2) = value_ml;
	*(pointer+3) = value_l;
}

/**
 * SHA256初始化
 *
 * @param[in]	ctx: SHA256算法结构体
 * @param[in]	is224: 1 - SHA224, or is SHA256
 * @param[out]	None
 *
 * @return		None
 */
static void sr_sha256_type_set(sha256_context *ctx, uint8_t  is224 )
{
    ctx->total[0] = 0;
    ctx->total[1] = 0;

    if ( is224 == 0 ){
        /* SHA-256 */
        ctx->state[0] = 0x6A09E667;
        ctx->state[1] = 0xBB67AE85;
        ctx->state[2] = 0x3C6EF372;
        ctx->state[3] = 0xA54FF53A;
        ctx->state[4] = 0x510E527F;
        ctx->state[5] = 0x9B05688C;
        ctx->state[6] = 0x1F83D9AB;
        ctx->state[7] = 0x5BE0CD19;
    }
    else {
        /* SHA-224 */
        ctx->state[0] = 0xC1059ED8;
        ctx->state[1] = 0x367CD507;
        ctx->state[2] = 0x3070DD17;
        ctx->state[3] = 0xF70E5939;
        ctx->state[4] = 0xFFC00B31;
        ctx->state[5] = 0x68581511;
        ctx->state[6] = 0x64F98FA7;
        ctx->state[7] = 0xBEFA4FA4;
    }

    ctx->is224 = is224;
}

#define  SHR(x,n) ((x & 0xFFFFFFFF) >> n)
#define ROTR(x,n) (SHR(x,n) | (x << (32 - n)))

static uint32_t S(uint32_t x, int8_t n)
{
    switch(n){
    case 0:
        return (ROTR(x, 7) ^ ROTR(x,18) ^  SHR(x, 3));
    case 1:
        return (ROTR(x,17) ^ ROTR(x,19) ^  SHR(x,10));
    case 2:
        return (ROTR(x, 2) ^ ROTR(x,13) ^ ROTR(x,22));
    default:
        return (ROTR(x, 6) ^ ROTR(x,11) ^ ROTR(x,25));
    }
}

#define F0(x,y,z) ((x & y) | (z & (x | y)))
#define F1(x,y,z) (z ^ (x & (y ^ z)))

static void P(unsigned int a,unsigned int b,unsigned int c,unsigned int* d,unsigned int e,
				unsigned int f,unsigned int g,unsigned int* h,unsigned int x,unsigned int K)
{
    uint32_t  temp1;
    temp1 = *h + S(e,3) + F1(e,f,g) + K + x;
    *d += temp1;
    *h = temp1 + S(a,2) + F0(a,b,c);
}

static const uint32_t KnumA[16]=
{
    0x428A2F98,0x71374491,0xB5C0FBCF,0xE9B5DBA5,0x3956C25B,0x59F111F1,0x923F82A4,0xAB1C5ED5,
    0xD807AA98,0x12835B01,0x243185BE,0x550C7DC3,0x72BE5D74,0x80DEB1FE,0x9BDC06A7,0xC19BF174
};
	
static const uint32_t KnumB[48]=
{
    0xE49B69C1,0xEFBE4786,0x0FC19DC6,0x240CA1CC,0x2DE92C6F,0x4A7484AA,0x5CB0A9DC,0x76F988DA,
    0x983E5152,0xA831C66D,0xB00327C8,0xBF597FC7,0xC6E00BF3,0xD5A79147,0x06CA6351,0x14292967,
    0x27B70A85,0x2E1B2138,0x4D2C6DFC,0x53380D13,0x650A7354,0x766A0ABB,0x81C2C92E,0x92722C85,
    0xA2BFE8A1,0xA81A664B,0xC24B8B70,0xC76C51A3,0xD192E819,0xD6990624,0xF40E3585,0x106AA070,
    0x19A4C116,0x1E376C08,0x2748774C,0x34B0BCB5,0x391C0CB3,0x4ED8AA4A,0x5B9CCA4F,0x682E6FF3,
    0x748F82EE,0x78A5636F,0x84C87814,0x8CC70208,0x90BEFFFA,0xA4506CEB,0xBEF9A3F7,0xC67178F2
};


/**
 * SHA256初始化
 *
 * @param[in]	ctx: SHA256算法结构体
 * @param[in]	data: 
 * @param[out]	None
 *
 * @return		None
 */
static void sr_sha256_process(sha256_context *ctx, uint8_t  data[64] )
{
    uint32_t temp1, W[64];
    uint32_t stateN[8];
    uint32_t indexA,indexB;

    for(temp1=0; temp1<16; temp1++)
    {
        W[temp1] = sr_hex2int(data+temp1*4);
    }

#define R(t) (W[t] = S(W[t -  2],1) + W[t -  7] + S(W[t - 15],0) + W[t - 16])

    memcpy(stateN,ctx->state,8*sizeof(unsigned int));
    
    for(indexA=0;indexA<2;indexA++)
    {
        for(indexB=0;indexB<8;indexB++)
        {
            P( stateN[(8-indexB)%8], stateN[(9-indexB)%8], stateN[(10-indexB)%8], &stateN[(11-indexB)%8], 
				stateN[(12-indexB)%8], stateN[(13-indexB)%8], stateN[(14-indexB)%8], &stateN[(15-indexB)%8], 
				W[indexA*8+indexB], KnumA[indexA*8+indexB]);
        }
    }
    
    for(indexA=0;indexA<6;indexA++)
    {
        for(indexB=0;indexB<8;indexB++)
        {
            P( stateN[(8-indexB)%8], stateN[(9-indexB)%8], stateN[(10-indexB)%8], &stateN[(11-indexB)%8], 
				stateN[(12-indexB)%8], stateN[(13-indexB)%8], stateN[(14-indexB)%8], &stateN[(15-indexB)%8], 
				R(indexA*8+indexB+16), KnumB[indexA*8+indexB]);
        }
    }
    
    for(indexA=0;indexA<8;indexA++)
    {
        ctx->state[indexA] += stateN[indexA];
    }
}


/** 
 * SHA-256 HASH算法更新结果值
 *
 * @param[in]	ctx: SHA256算法结构体
 * @param[in]	inbuf: 写入的数据
 * @param[in]	inlen: 写入数据的长度
 * @param[out]	None
 *
 * @return		None
 */
int32_t sr_sha256_update(sha256_context *hd, uint8_t *inbuf, uint32_t inlen)
{
    int32_t  fill;
    uint32_t  left;

    if (hd==NULL || inbuf==NULL || inlen <= 0){
        return E_EINVAL;
    }

    left = hd->total[0] & 0x3F;
    fill = 64 - left;

    hd->total[0] += inlen;
    hd->total[0] &= 0xFFFFFFFF;

    if(hd->total[0] < (unsigned long) inlen ){
        hd->total[1]++;
    }

    if(left && inlen >= fill ){
        memcpy((void *) (hd->buffer + left),(void *) inbuf, fill );
        sr_sha256_process(hd, hd->buffer );
        inbuf += fill;
        inlen  -= fill;
        left = 0;
    }

    while(inlen >= 64 )
    {
        sr_sha256_process(hd, inbuf );
        inbuf += 64;
        inlen  -= 64;
    }

    if(inlen > 0 ){
        memcpy((void *) (hd->buffer + left),(void *) inbuf, inlen );
    }
    return E_OK;
}


/**
 *  SHA-256 HASH算法结果
 *
 * @param[in]	hd:  sha256结构体
 * @param[out]	strHash: 最终的hash值
 *
 * @return		0成功，其他失败
 */
int sr_sha256_final(sha256_context *hd, uint8_t *strHash)
{
    uint32_t  last, padn;
    uint32_t  high, low;
    uint8_t  msglen[8];
    uint8_t *sha2_padding;

    if (hd == NULL || strHash == NULL){
        return E_EINVAL;
    }

    high = ( hd->total[0] >> 29 ) | ( hd->total[1] <<  3 );
    low  = ( hd->total[0] <<  3 );

    sr_int2hex( high, msglen);
    sr_int2hex( low,  msglen+4);

    last = hd->total[0] & 0x3F;
    padn = ( last < 56 ) ? ( 56 - last ) : ( 120 - last );

    sha2_padding =  malloc(padn);
    if (sha2_padding == NULL){
        return E_ERROR;
    }
    memset(sha2_padding,0,padn);
    sha2_padding[0]=0x80;
    sr_sha256_update(hd, (unsigned char *) sha2_padding, padn);
    free(sha2_padding);
    sr_sha256_update( hd, msglen, 8 );

    for (padn=0;padn<7;padn++){
        sr_int2hex( hd->state[padn], strHash+padn*4);
    }

    if ( hd->is224 == 0 ){
        sr_int2hex( hd->state[7], strHash+28 );
    }
    return E_OK;
}


/**
 *  SHA-256 HASH算法
 *
 * @param[in]	pData:  数据
 * @param[in]	pLen :  数据长度
 * @param[out]	strHash: 生成的HASH值(32字节)
 *
 * @return		0成功，其他失败
 */
int32_t crypt_sha256_test(const uint8_t *data, uint32_t len, uint8_t *hash)
{
    sha256_context *ctx;

    if (data == NULL || hash==NULL){
    	return E_EINVAL;
    }
	
    ctx = malloc(sizeof(sha256_context));
    if(ctx == NULL){
        return E_ERROR;
    }
    sr_sha256_type_set(ctx,0);
    sr_sha256_update(ctx, (unsigned char *)data, len);
    sr_sha256_final(ctx, hash);
    free(ctx);
	
    return E_OK;
}


/**
 * sha256算法初始化
 *
 * @param[in]	sha256_context:sha256算法结构体
 * @param[out]	None
 *
 * @return		0成功，其他失败
 */
int sr_sha256_init(sha256_context *hd )
{

    if (hd == NULL){
        return E_EINVAL;
    }
    sr_sha256_type_set(hd ,0);
    return E_OK;
}
