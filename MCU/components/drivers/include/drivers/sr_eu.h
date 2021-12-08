#ifndef __SR_EU_H__
#define __SR_EU_H__

#include <stdint.h>
#include "bsp\include\drv_can.h"
#include "drivers\include\device.h"


typedef enum _eu_model {
	EU100 = 0x01,

} sr_eu_model;

#define	EU_MODEL_EU100		"EU100"


int32_t eu_init(sr_device_id_t type, sr_eu_model model, device_rx_cb cb);


/**
 * eu复位
 *
 * @param[in]	dev: 设备号
 * @param[out]	None
 *
 * @return		>=0成功，其他失败
 */
int32_t sr_eu_rst(sr_device_id_t dev);

int32_t sr_eu_ctrl(sr_device_id_t dev,  uint8_t *data, uint8_t len);


#endif  /* __SR_EU_H__ */
