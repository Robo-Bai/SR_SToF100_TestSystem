#ifndef __ASD101x_H__
#define __ASD101x_H__

#include "bsp\include\drv_common.h"

#define ADS1015_CHANNELS 8

struct ads1015_channel_data {
	uint8_t enabled;
	unsigned int pga;
	unsigned int data_rate;
};

struct ads1015_platform_data {
	struct ads1015_channel_data channel_data[ADS1015_CHANNELS];
};

void ads101x_init(void);

int32_t adc_cur_read(uint8_t ain, uint16_t* current);



#endif /* __ASD101x_H__ */
