#ifndef __ADC_H__
#define __ADC_H__

#include "bsp\include\drv_common.h"

enum adc_items {
	ITEM_ADC_TEMP_CH,
	ITEM_ADC_PWR_3V3,
	ITEM_ADC_PWR,
	ITEM_ADC_PWR_5V,
	ITEM_ADC_PWR_AIN,
};

void adc_init(void);
int32_t pwr_adc_read(enum adc_items item, uint16_t* volt_mv);

#endif /* __ADC_H__ */
