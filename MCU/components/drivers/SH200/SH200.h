#ifndef __SH200_H
#define __SH200_H
#include "sys.h"

int32_t SH200_INconfig(uint32_t SH200_ID);//��������ģʽ
int32_t SH200_transmit(uint32_t SH200_ID); //ʹ�������ϱ��������ϱ�����Ϊ25ms--HEX��0x19
int32_t SH200_OUTconfig(uint32_t SH200_ID);//�˳�����ģʽ
int32_t SH200_activate(uint32_t SH200_ID);//�����豸


//void SH200_active(void);//SH200����


#endif

