#ifndef __SR_IWDG_H__
#define __SR_IWDG_H__


#include "bsp\include\drv_common.h"



void iwdg_init(uint8_t prescaler, uint16_t reload);
void iwdg_feed(void);


#endif /* __SR_IWDG_H__ */