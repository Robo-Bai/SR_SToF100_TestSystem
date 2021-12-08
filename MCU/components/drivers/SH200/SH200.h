#ifndef __SH200_H
#define __SH200_H
#include "sys.h"

int32_t SH200_INconfig(uint32_t SH200_ID);//进入配置模式
int32_t SH200_transmit(uint32_t SH200_ID); //使能主动上报，设置上报周期为25ms--HEX：0x19
int32_t SH200_OUTconfig(uint32_t SH200_ID);//退出配置模式
int32_t SH200_activate(uint32_t SH200_ID);//激活设备


//void SH200_active(void);//SH200动作


#endif

