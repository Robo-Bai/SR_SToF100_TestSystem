/**
 * Copyright (C) 2019 Standard-robots, Inc
 *
 * @file 	sr_motor.c
 * @author	Neethan <lijun@standard-robots.com> / <neethan@foxmail.com>
 * @version V1.0.0
 * @date 	2019年 07月 17日 星期三 10:50:47 CST
 *
 * @breif	srtos motor drivers manage. 
 */


#include <string.h>
#include "bsp\include\drv_can.h"
#include "drivers\include\drivers\sr_motor.h"
#include "drivers\include\drivers\sr_motor_kinco.h"
#include "drivers\include\sr_system_regs.h"





int32_t motor_init(sr_device_id_t type, sr_motor_model model, device_rx_cb cb)
{
	int32_t ret = -1;
	struct _device_manager *device = NULL;

	if (!sr_device_find(NULL, type)){
		ret = sr_device_add(dev_class_maps(type, OP_NAME), NULL, type, 
			 dev_class_map2minor(type), kinco_node_protect);
			
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
		case KINCO_FD122:
			ret = kinco_fd122_init(device, type, cb);
			strcpy((char *)device->model, MOTOR_MODEL_KINCO_FD122);
			break;
		
		default:
			break;
	}	

	return ret;
}

int32_t motor_deinit(sr_device_id_t dev)
{
	sr_kprintf("enter %s\r\n", __FUNCTION__);

	return sr_device_del(NULL, dev);
}						


int32_t motor_speed_control(sr_device_id_t dev, int16_t speed) 
{
	struct _device_manager *device = sr_device_find(NULL, dev);
	
	if (device){
		if(strcmp((char *)device->model, MOTOR_MODEL_KINCO_FD122) == 0){
			kinco_fd122_speed_contorl(dev, speed);
		}
	}
	return 0;
}

int32_t motor_err_clr(sr_device_id_t dev)
{
	struct _device_manager *device = sr_device_find(NULL, dev);
	
	if (device){
		if(strcmp((char *)device->model, MOTOR_MODEL_KINCO_FD122) == 0){
			kinco_fd122_err_clr(dev);
		}
	}
	return 0;
}

int32_t  motor_enable(sr_device_id_t dev, bool state)
{
	struct _device_manager *device = sr_device_find(NULL, dev);
	
	if (device){
		if(strcmp((char *)device->model, MOTOR_MODEL_KINCO_FD122) == 0){
			kinco_fd122_enable(dev, state);
		}
	}
	return 0;
}

int32_t motor_pdo_enable(sr_device_id_t dev)
{
	struct _device_manager *device = sr_device_find(NULL, dev);
	
	if(strcmp((char *)device->model, MOTOR_MODEL_KINCO_FD122) == 0){
		kinco_fd122_pdo_enable(device);
	}	
	
	return 0;
}

int32_t motor_mode_select(sr_device_id_t dev)
{
	struct _device_manager *device = sr_device_find(NULL, dev);
	
	if(strcmp((char *)device->model, MOTOR_MODEL_KINCO_FD122) == 0){
		kinco_fd122_mode_select(dev);
	}
	
	return 0;
}


int32_t motor_emergency_stop(sr_device_id_t dev) 
{
	int32_t ret = -1;
	struct _device_manager *device = sr_device_find(NULL, dev);
	
	if (!device) return ret;
	
		if(strcmp((char *)device->model, MOTOR_MODEL_KINCO_FD122) == 0){
			ret = kinco_emergency_stop(dev);
			sr_kprintf("emergency_stop, dev id:%d\r\n", dev);
		}
	
	return ret;
}

int32_t motor_emergency_cancel(sr_device_id_t dev) 
{
	int32_t ret = -1;
	struct _device_manager *device = sr_device_find(NULL, dev);
	
	if (!device) return ret;
	
		if(strcmp((char *)device->model, MOTOR_MODEL_KINCO_FD122) == 0){
			ret = kinco_emergency_cancel(dev);
			sr_kprintf("emergency_cancel, dev id:%d\r\n", dev);
		}
	
	return ret;
}


int16_t motor_write(sr_device_id_t dev, uint8_t* data, uint8_t len, uint16_t timeout)
{
	struct _device_manager *device = sr_device_find(NULL, dev);
	
	if (device){
		if(strcmp((char *)device->model, MOTOR_MODEL_KINCO_FD122) == 0){
			return kinco_fd122_write(dev, data, len, timeout);
		}
	}
	return -1;
}
