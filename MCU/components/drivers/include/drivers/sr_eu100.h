#ifndef __SR_EU100_H__
#define __SR_EU100_H__

#include <stdint.h>
#include "bsp\include\drv_can.h"
#include "drivers\include\device.h"



/**
 * eu100初始化
 *
 * @param[in]	dev: 设备号
 * @param[out]	None
 *
 * @return		>=0成功，其他失败
 */
int32_t sr_eu100_init(struct _device_manager *device, sr_device_id_t type,
								device_rx_cb cb);

/**
 * eu100复位
 *
 * @param[in]	dev: 设备号
 * @param[out]	None
 *
 * @return		>=0成功，其他失败
 */
int32_t sr_eu100_rst(sr_device_id_t dev);

int32_t eu100_ctrl(sr_device_id_t dev, uint8_t *data, uint8_t len);

int32_t sr_eu100_heart_beat(sr_device_id_t dev, uint8_t *data1, uint16_t len);



#endif  /* __SR_EU100_H__ */
