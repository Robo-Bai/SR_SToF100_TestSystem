#ifndef __DRV_HWTIMER_H
#define __DRV_HWTIMER_H

#include <stdint.h>

int32_t sr_hw_timer_init(void);
uint32_t system_time_tick(void);

#endif /* __DRV_HWTIMER_H */


