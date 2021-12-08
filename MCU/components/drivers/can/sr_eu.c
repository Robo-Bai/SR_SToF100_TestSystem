/**
 * Copyright (C) 2019 Standard-robots, Inc
 *
 * @file 	sr_eu.c
 * @author	Neethan <lijun@standard-robots.com> / <neethan@foxmail.com>
 * @version V1.0.0
 * @date 	2019年 06月 19日 星期三 10:50:47 CST
 *
 * @brief    eu100 devices driver.
 */

#include <string.h>
#include "srtos\srtos.h"
#include "drivers\include\drivers\sr_eu.h"
#include "drivers\include\drivers\sr_eu100.h"
#include "drivers\include\sr_system_regs.h"


int32_t eu_init(sr_device_id_t type, sr_eu_model model, device_rx_cb cb)
{
	int32_t ret = -1;
	struct _device_manager *device = NULL;

	if (!sr_device_find(NULL, type)){
		ret = sr_device_add(dev_class_maps(type, OP_NAME), NULL, type, 
			 dev_class_map2minor(type), sr_eu100_heart_beat);
			
		if (ret < 0){
			sr_kprintf("%s, sr_device_add %s failed\n", __FUNCTION__, 
			dev_class_maps(type, OP_NAME));
			return -1;
		}
	} else {
		sr_kprintf("%s, the device %s is registered\n", __FUNCTION__, 
		dev_class_maps(type,OP_NAME));
		return -2;
	}

	device = sr_device_find(NULL, type);
	switch (model){
		case EU100:
			ret = sr_eu100_init(device, type, cb);
			strcpy((char *)device->model, EU_MODEL_EU100);
			break;
		
		default:
			break;
	}	

	return ret;
}


/**
 * eu复位
 *
 * @param[in]	dev: 设备号
 * @param[out]	None
 *
 * @return		>=0成功，其他失败
 */
int32_t sr_eu_rst(sr_device_id_t dev)
{
	struct _device_manager *device = sr_device_find(NULL, dev);
	
	if (device){
		if(strcmp((char *)device->model, EU_MODEL_EU100) == 0){
			sr_eu100_rst(dev);
		}
	}
	return 0;
}

int32_t sr_eu_ctrl(sr_device_id_t dev,  uint8_t *data, uint8_t len) 
{
	struct _device_manager *device = sr_device_find(NULL, dev);
	
	if (device){
		if(strcmp((char *)device->model, EU_MODEL_EU100) == 0){
			eu100_ctrl(dev, data, len);
		}
	}
	return 0;
}


int32_t eu_deinit(sr_device_id_t dev)
{
	sr_kprintf("enter %s\r\n", __FUNCTION__);

	return sr_device_del(NULL, dev);
}
