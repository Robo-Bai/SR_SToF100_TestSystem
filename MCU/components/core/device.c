/**
 * Copyright (C) 2019 Standard-robots, Inc
 *
 * @file 	device.c
 * @author	Neethan <lijun@standard-robots.com> / <neethan@foxmail.com>
 * @version V1.0.0
 * @date 	2020年 2月 12日 星期三 10:50:47 CST
 *
 * @brief    device manager
 */

#include <stdlib.h>
#include "srtos\err_code.h"
#include "drivers\include\device.h"
#include "srtos\srtos.h"
#include "drivers\include\sr_system_regs.h"
#include "sr\app\motion_crl\include\sr_mc_regs.h"
#include "sr\app\include\sr_def_common.h"
#include "bsp\include\drv_can.h"


#define SR_DEV_MANAGER_TASK_PRIO			5
#define SR_DEV_MANAGER_STK_SIZE 			(512 *1) /* 以字为单位 */

/* 时钟节拍10us,电机心跳300ms */
#define	HEART_BEAT_KINCO_TICK_MS		(30000)

static TaskHandle_t sr_device_manager_handler = NULL;
static TimerHandle_t device_manage_timers = NULL;
static TimerHandle_t device_heart_timers = NULL;

static volatile int32_t last_error_code[DEVICE_NUM_MAX] = {-1};

static QueueHandle_t sr_dev_status = NULL;
static struct dev_status dev_state = {NULL, 0};
static int32_t base_type = 0;
static volatile uint8_t dev_num = 0;


/* 
 * 设备动态地址分配及读取设备数据：
 * 1、保持设备寄存器数据结构一致、寄存器大小对齐；
 * 2、通过sros下发的配置参数到srtos，获取到车类型从而获取srtos要挂载的设备；
 * 3、注册设备，通过设备链表将name,major,addr字段绑定，注册的设备总数需写入系统
 * 寄存器,便于上位机sros读取设备数据；
 * 4、建立设备缓存空间，独立保存设备数据；
 * 5、设备按照先后注册顺序，依次以设备寄存器偏移量分配地址；
 * 6、srtos保存设备数据，通过设备号/设备名/设备地址查找设备寄存器将数据写入；
 * 7、sros读取设备数据，通过设备起始地址查找设备信息，再进一步通过设备名即可知道
 * 设备地址与设备相对应，然后将寄存器将数据读出。
 */



struct _device_class {
	const char *name;
	sr_device_id_t type;
	uint16_t minor;
	const char *serial_no;
	const char *drv_serial_no; 
};


/*
 * 电机类设备号：0~19
 * EU类设备号：20~29
 * 编码器设备号：30~39
 * PGV类设备号：40~49
 * 保留设备号：50~255
 */
static struct _device_class dev_class[] = {

	/* 运动控制底盘电机 */
	{"motor1", DEV_MOTOR_MC_1, 1, "24", "F123300XX171170073"},
	{"motor2", DEV_MOTOR_MC_2, 2, "24", "F123300XX171170073"},
	{"motor3", DEV_MOTOR_MC_3, 3, "24", "F123300XX171170073"},
	{"motor4", DEV_MOTOR_MC_4, 4, "24", "F123300XX171170073"},

	/* 动作控制电机次设备号从5开始 */
	{"motor5", DEV_MOTOR_AC_1, 5, "24", "F123300XX171170073"},
	{"motor6", DEV_MOTOR_AC_2, 6, "24", "F123300XX171170073"},

	{"eu100_1", DEV_EU100_1, 20, "eu100", "eu100_v2"},
	{"eu100_2", DEV_EU100_2, 21, "eu100", "eu100_v2"},
	{"eu100_3", DEV_EU100_3, 22, "eu100", "eu100_v2"},
	{"eu100_4", DEV_EU100_4, 23, "eu100", "eu100_v2"},
	{"eu100_5", DEV_EU100_5, 24, "eu100", "eu100_v2"},
	{"eu100_6", DEV_EU100_6, 25, "eu100", "eu100_v2"},
	
	{"pgv_1", DEV_PGV_UP_1, 40, NULL, NULL},
	
	{NULL, DEV_NULL, 0, NULL, NULL} /* NULL device */
};

static SemaphoreHandle_t _semaphore_device = NULL;
static struct _device_manager *device_manager_head = NULL;

uint32_t device_state_maps(sr_device_id_t type, uint32_t state)
{
	static volatile uint32_t s = DEVICE_NONE;

	switch (type){
		case DEV_MOTOR_MC_1:
		case DEV_MOTOR_MC_2:
		case DEV_MOTOR_MC_3:
		case DEV_MOTOR_MC_4:
		case DEV_MOTOR_AC_1:
		case DEV_MOTOR_AC_2:
		case DEV_EU100_1:
		case DEV_EU100_2:
		case DEV_EU100_3:
		case DEV_EU100_4:
		case DEV_EU100_5:
		case DEV_EU100_6:
			if (state == DEVICE_NONE){
				s = DEVICE_OK;
			} else {
				s = DEVICE_ERROR;
			}
			break;

		default:
			break;
	}

	return s;
}


const char *dev_class_maps(sr_device_id_t type, enum dev_options_t op)
{
	if (OP_NAME == op){
		for (uint8_t i=0; i<sizeof(dev_class)/sizeof(dev_class[0]); ++i){
			if (dev_class[i].type == type){
				return dev_class[i].name;
			}
		}
	} else 	if(OP_SERIAL_NO == op){
		for (uint8_t i=0; i<sizeof(dev_class)/sizeof(dev_class[0]); ++i){
			if (dev_class[i].type == type){
				return dev_class[i].serial_no;
			}
		}
	} else 	if(OP_DRV_SERIAL_NO == op){
		for (uint8_t i=0; i<sizeof(dev_class)/sizeof(dev_class[0]); ++i){
			if (dev_class[i].type == type){
				return dev_class[i].drv_serial_no;
			}
		}
	}
	return NULL;
}

uint16_t dev_class_map2minor(sr_device_id_t type)
{
	for (uint8_t i=0; i<sizeof(dev_class)/sizeof(dev_class[0]); ++i){
		if (dev_class[i].type == type){
			return dev_class[i].minor;
		}
	}
	return 0;
}


int32_t sr_device_add(const char *dev_name, char *model, sr_device_id_t type,
							uint16_t minor, p_func function)
{
	char *str = NULL;
	struct _device_manager *_dev_new = NULL;
	struct _device_manager *_dev_cur = NULL;	
	
	if (!dev_name || (strlen(dev_name) > DEVICE_NAME_LEN_MAX)){
		return -1;
	}
	
	if (xSemaphoreTake(_semaphore_device, portMAX_DELAY) == pdTRUE){
		str = pvPortMalloc(strlen(dev_name)+1);
		if (str) {
			strcpy(str,dev_name);
			_dev_new = pvPortMalloc(sizeof(struct _device_manager));
			if (_dev_new) {
				_dev_new->name = str;
				_dev_new->type = type;
				_dev_new->minor = minor;
				strcpy((char *)_dev_new->model, model);
				strcpy((char *)_dev_new->serial_no, dev_class_maps(type, OP_SERIAL_NO));
				strcpy((char *)_dev_new->drv_serial_no, dev_class_maps(type, OP_DRV_SERIAL_NO));
				_dev_new->status = 0;
				_dev_new->error_code = 0;
				_dev_new->io_status = 0;
				_dev_new->status_op_word = 0;
				_dev_new->pre = NULL;
				_dev_new->next = NULL;
				_dev_new->hb_handler = function;
				
				if (!device_manager_head){
					device_manager_head = _dev_new;
				} else {
					_dev_cur = device_manager_head;
					while (_dev_cur->next) {
						_dev_cur = _dev_cur->next;
					}
					_dev_cur->next = _dev_new;
					_dev_new->pre = _dev_cur;
				}
				xSemaphoreGive(_semaphore_device);
				return 0;	
			}
		}
		xSemaphoreGive(_semaphore_device);
	}
	return -1;
}

int32_t sr_device_list(void)
{
	uint8_t i = 0;
	struct _device_manager *_dev_cur;
	
	_dev_cur = device_manager_head;

	while (_dev_cur){
		i++;
		sr_kprintf("%s, minor:%d, dev:%d, serial_no:%s\r\n", _dev_cur->name, 
		_dev_cur->minor, _dev_cur->type, _dev_cur->serial_no);
		_dev_cur = _dev_cur->next;
	}
	return 0;
}

int32_t sr_device_total_check(void)
{
	uint8_t i = 0;
	struct _device_manager *_dev_cur = NULL;

	if (xSemaphoreTake(_semaphore_device, portMAX_DELAY) == pdTRUE ){
		_dev_cur = device_manager_head;
		while (_dev_cur){
			i++;
			_dev_cur = _dev_cur->next;
		}
		xSemaphoreGive(_semaphore_device);
	}
	return i;
}

struct _device_manager *sr_device_find(const char *dev_name, sr_device_id_t dev)
{
	static struct _device_manager *_dev_cur = NULL;

	if (xSemaphoreTake(_semaphore_device, portMAX_DELAY) == pdTRUE ){

		_dev_cur = device_manager_head;
		while (_dev_cur) {
			if (_dev_cur->type == dev || strcmp(_dev_cur->name, dev_name) == 0){
                break;  
			} else {
				_dev_cur = _dev_cur->next;
			}
		}
		xSemaphoreGive(_semaphore_device);
        return _dev_cur;
	}
	return NULL;
}

static int32_t del_name(struct _device_manager *dev_del)
{
	struct _device_manager *_dev_cur;
	struct _device_manager *_dev_pre;
	struct _device_manager *_dev_next;

	if (xSemaphoreTake(_semaphore_device, portMAX_DELAY) == pdTRUE){
		if (device_manager_head == dev_del) {
			device_manager_head = dev_del->next;
		} else {
			_dev_cur = device_manager_head->next;
			while (_dev_cur) {
				if (_dev_cur == dev_del){
					/* 从链表中删除 */
					_dev_pre 		= _dev_cur->pre;
					_dev_next 		= _dev_cur->next;
					_dev_pre->next 	= _dev_next;
					if (_dev_next){
						_dev_next->pre = _dev_pre;
					}
				} else {
					_dev_cur = _dev_cur->next;
				}
			}
		}
		vPortFree(dev_del->name);
		vPortFree(dev_del);
		dev_del->name = NULL;
		dev_del = NULL;
        xSemaphoreGive(_semaphore_device);
        return 0;
	}
	return -1;
}

int32_t sr_device_del(const char *name, sr_device_id_t dev)
{
	struct _device_manager *_dev_find;

	_dev_find = sr_device_find(NULL, dev);
	if(!_dev_find) {
		sr_kprintf("warning:No the %s device\r\n", name);
		return -1;
	}
	return del_name(_dev_find);
}


int32_t sr_device_update(struct _device_manager *dev, uint32_t status, uint32_t sta_code)
{
	if (dev) {
		dev->status = device_state_maps(dev->type, status);
		dev->error_code = sta_code;
		//sr_kprintf("minor:%d, state:0x%x, e_code:0x%x\r\n", dev->minor, dev->status, dev->error_code);
	}

	return 0;
}

static int32_t device_manage(struct _device_manager *dev)
{
	static volatile uint32_t start_heart_beat_cnt = 0;
	//int32_t ret = -1;
	int32_t dev_info[4] = {0};
	struct _device_manager *dev_manage = dev;

		// 查询注册的电机，底层驱动会自动更新状态和错误码
		// 通知机制需要做成异步通知，某个位置的电机状态变化了，才更新错误码
		// 查找出异常设备，系统做标记
		while (dev_manage){

			/* 电机心跳回调, 20s = 1 * 50 ms后在发节点保护 */
			if (++start_heart_beat_cnt >= 400 ){
				start_heart_beat_cnt = 401;
				dev_manage->hb_handler(dev_manage->type, NULL, 0);
			}

			/* 出现异常的设备,需要判断错误码和通信是否异常 */
			if (dev_manage->status != DEVICE_OK){
				memset(dev_info, 0, sizeof(dev_info));
				dev_info[0]  = dev_manage->type;
				dev_info[1]  = dev_manage->error_code;
				//ret = sr_comm_dispatch_event(EV_DEVICE_ERROR, sizeof(int32_t)*index,
				//	dev_info, qev_ac);
				//ret = sr_comm_dispatch_event(EV_DEVICE_ERROR, sizeof(int32_t)*index,
				//	dev_info, qev_mc);				
				/* 异常处理 */
			}

			dev_manage = dev_manage->next;
		}

	return 0;
}

static int32_t device_state_update(struct _device_manager *dev)
{
	int32_t ret = -1;
	int32_t buf[32+1] = {0};
	uint16_t index = 0;
	uint8_t device_total = 0;
	
	/* 更新设备信息 */
	while (dev) {
		memset(buf, 0 , sizeof(buf));
		buf[index++] = dev->type; 
		buf[index++] = dev->status; 
		buf[index++] = dev->error_code; 
		strcpy((char *)&buf[index], (char *)dev->model); 
		index += 4;
		//sr_kprintf("dev->type %d, dev->minor %d\r\n", dev->type, dev->minor);
		strcpy((char *)&buf[index], (char *)dev->serial_no); 
		//sr_kprintf("%s, dev->serial_no=%s\r\n", __FUNCTION__, &buf[index]);
		index += 8;
		strcpy((char *)&buf[index], (char *)dev->drv_serial_no); 
		//sr_kprintf("%s, dev->drv_serial_no=%s\r\n", __FUNCTION__, &buf[index]);
		index += 8;
	
		switch (dev->type){
		
			case DEV_MOTOR_MC_1:
			case DEV_MOTOR_MC_2:
			case DEV_MOTOR_MC_3:
			case DEV_MOTOR_MC_4:
			case DEV_MOTOR_AC_1:
			case DEV_MOTOR_AC_2:
			{
				buf[index++] = dev->io_status; 
				buf[index++] = dev->status_op_word; 
				memset(&buf[index], 0, sizeof(uint32_t)*7); 
				index += 7;
				ret =  0;
				break;
			} 
	
			case DEV_EU100_1:
			case DEV_EU100_2:
			case DEV_EU100_3:
			case DEV_EU100_4:
			case DEV_EU100_5:
			case DEV_EU100_6:				
			{
				buf[index++] = dev->io_status; 
				// 输出电流及运行模式监控，低16位低8位-运行模式  低16位高8位-电流输出
				buf[index++] = dev->status_op_word; 
				memset(&buf[index], 0, sizeof(uint32_t)*7); 
				index += 7;
				ret =  0;
				break;
			}
			
			default:
				ret = -1;
				break;
		}
		
		if (ret == 0){
			ret = sr_reg_write((REG_DEVICE_BASE+(device_total << 5)), REG_DEVICE_BASE_OFFET, buf);
			//sr_kprintf("devices reg write dev addr:0x%x, dev_type:%d, minor:%d, index:%d, status:0x%x, error_code:0x%x\r\n", 
			//	(REG_DEVICE_BASE+(device_total << 5)), dev->type, dev->minor, index, dev->status, dev->error_code);
			if (ret < 0){
				sr_kprintf("error:%s, devices %d\r\n", __FUNCTION__, dev->type);
				sr_log2sros("error:%s, devices %d\n", __FUNCTION__, dev->type);
			}
		}

		index = 0;
		device_total++;
		dev = dev->next;
	}
	return ret;
}


static void sr_device_manage_cb(xTimerHandle px_timer)
{
	uint32_t timer_id = 0; 
	static volatile bool dev_state_flag = false;
	
	struct _device_manager *dev = sr_device_get_node();
	
	configASSERT(px_timer);

	timer_id = (uint32_t)pvTimerGetTimerID(px_timer);
	if (timer_id == SR_TIMER_DEV_MANAGE){ 

		dev_num = sr_device_total_check();
		if (dev_num){
			if (!dev_state_flag){
				sr_reg_read(REG_PARA_MC_BASE_TYPE, 1, &base_type);
				sr_kprintf("%s, base_type:%d\r\n", __FUNCTION__, base_type);
				if (MEC == base_type){
					#define BASE_TYPE_MEC
				}
				drv_can_state_set(1);
				dev_state_flag = true;
			}
			/* 设备状态更新 */
			device_state_update(dev);
		}
	}

	if (timer_id == SR_TIMER_DEV_NODE_PROTECT){ 
		if (dev_num){
			/* 设备异常/心跳管理 */ 	
			device_manage(dev); 
		}
	}
}


static int32_t new_timer_manage_device(void)
{
	device_manage_timers = xTimerCreate("timer_report",			/* 定时器名字 */
								 100*SYSTEM_TICK_MS, 			/* 定时器周期,100ms */
								 pdTRUE,						/* 周期性 */
								 (void *)SR_TIMER_DEV_MANAGE,	/* 定时器ID */
								 sr_device_manage_cb);			/* 定时器回调函数 */
	if (!device_manage_timers){
		sr_kprintf("failed to xTimerCreate device_manage_timers\r\n");
		return E_ERROR;
	} else {
		/* 启动定时器，系统启动后才开始工作 */
		if (xTimerStart(device_manage_timers, 10000) != pdPASS){
			 /* 定时器还没有进入激活状态 */
		   sr_kprintf("xTimerStart device_manage_timers false\r\n");
		}
	}

	device_heart_timers = xTimerCreate("timer_heart",			/* 定时器名字 */
								 50*SYSTEM_TICK_MS, 			/* 定时器周期,50ms */
								 pdTRUE,						/* 周期性 */
								 (void *)SR_TIMER_DEV_NODE_PROTECT,	/* 定时器ID */
								 sr_device_manage_cb);			/* 定时器回调函数 */
	if (!device_heart_timers){
		sr_kprintf("failed to xTimerCreate device_heart_timers\r\n");
		return E_ERROR;
	} else {
		/* 启动定时器，系统启动后才开始工作 */
		if (xTimerStart(device_heart_timers, 10000) != pdPASS){
			 /* 定时器还没有进入激活状态 */
		   sr_kprintf("xTimerStart device_heart_timers false\r\n");
		}
	}

	return 0;
}


void sr_device(void *pvParameters)
{
	volatile uint32_t cur_error_code[DEVICE_NUM_MAX]  = {0};

	memset((uint32_t *)last_error_code, -1, sizeof(last_error_code));
    memset((uint8_t *)cur_error_code, 0, sizeof(cur_error_code));
    
	if (!_semaphore_device){
		_semaphore_device = xSemaphoreCreateMutex();
	}
	
    sr_dev_status = xQueueCreate(50, sizeof(struct dev_status));
    if (!sr_dev_status) {
		sr_kprintf("failed to xQueueCreate sr_dev_status\r\n");
		return;
    }

	new_timer_manage_device();
	
	while (1) {

		if (xQueueReceive(sr_dev_status, &dev_state, 0) == pdPASS){
			switch (dev_state.dev->type/10){
				case DEV_MOTOR_MC:
				case DEV_MOTOR_AC:
					// MOTOR_MSG_VEE: /* 速度/错误码1/错误码2 */		
					cur_error_code[dev_state.dev->minor] = (uint32_t)(dev_state.data[4] | 
					(dev_state.data[5] << 8) |  (dev_state.data[6] << 16) | 
					(dev_state.data[7] << 24)); 				
					break;
				
				case DEV_EU100:
					 /* 错误码 */		
					cur_error_code[dev_state.dev->minor] = dev_state.data[4]; 
					break;

				default:
					break;
			}
		
			if (last_error_code[dev_state.dev->minor] != cur_error_code[dev_state.dev->minor]) {

				// 只有错误时，才更新一次
				last_error_code[dev_state.dev->minor] = cur_error_code[dev_state.dev->minor];

				sr_device_update(dev_state.dev, cur_error_code[dev_state.dev->minor], 
				cur_error_code[dev_state.dev->minor]);
			
				// TODO
				// 第二个参数为状态码，暂时将错误码当做状态处理
				sr_kprintf("dev_type:%d, minor:%d, error_code:0x%x\r\n", dev_state.dev->type, dev_state.dev->minor, 
					cur_error_code[dev_state.dev->minor]);
				sr_log2sros("dev_type:%d, minor:%d, error_code:0x%x\n", dev_state.dev->type, dev_state.dev->minor, 
					cur_error_code[dev_state.dev->minor]);
				vTaskDelay(pdMS_TO_TICKS(10));	// 当有错误时，10ms刷新一次
			}
		}
		
		taskYIELD(); // request a context switch to another task
		//vTaskDelay(pdMS_TO_TICKS(2));	
	}
}

QueueHandle_t get_dev_status_handle(void)
{
	return sr_dev_status;
}

int32_t sr_device_manager_init(void)
{
	int32_t ret = -1;
	
	ret = xTaskCreate((TaskFunction_t)sr_device, 
					(const char *)"sr_device",  		
					SR_DEV_MANAGER_STK_SIZE, 
					NULL, 
					SR_DEV_MANAGER_TASK_PRIO, 
					&sr_device_manager_handler);
	if (ret){
		return E_OK;
	} else {
		sr_kprintf("failed to xTaskCreate sr_device,ret=%d\r\n", ret);
		return E_ERROR;
	}
}

struct _device_manager *sr_device_get_node(void)
{
	return device_manager_head; 
}

