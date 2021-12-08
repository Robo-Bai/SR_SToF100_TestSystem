#ifndef __DEVICE_H__
#define __DEVICE_H__

#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>
#include <timers.h>


#ifdef __cplusplus
extern "C"{
#endif


#define DEVICE_NUM_MAX			64
#define DEVICE_NAME_LEN_MAX		8

// 高两位表示设备类，最低位表示设备号
typedef enum _device_no {
	DEV_NULL = 0,

	/* 运动-底盘 */
	DEV_MOTOR_MC_1 = 211,
	DEV_MOTOR_MC_2 = 212,
	DEV_MOTOR_MC_3 = 213,
	DEV_MOTOR_MC_4 = 214,

	/* 动作-非标 */
	DEV_MOTOR_AC_1 = 911,
	DEV_MOTOR_AC_2 = 912,

	DEV_EU100_1    =  921,
	DEV_EU100_2    =  922,
	DEV_EU100_3    =  923,
	DEV_EU100_4    =  924,
	DEV_EU100_5    =  925,
	DEV_EU100_6    =  926,
	
	DEV_PGV_UP_1   =  221,

} sr_device_id_t;
	

enum device_type {
	DEV_MOTOR_MC 	= 21,
	DEV_MOTOR_AC 	= 91,
	DEV_PGV_UP 		= 22,
	DEV_EU100 		= 92
};

enum dev_options_t
{
	OP_NAME,
	OP_SERIAL_NO,
	OP_DRV_SERIAL_NO,
};

typedef enum {
	MOTOR_MSG_NULL = 0x00,		/* 空 */
	MOTOR_MSG_PSC = 0x01,		/* 位置/状态/电流 */
	MOTOR_MSG_VEE = 0x02,		/* 速度/错误码1/错误码2 */
	MOTOR_MSG_CITT  = 0x03,		/* 控制字/电机IO输入/温度/扭矩 */
	MOTOR_MSG_WM  = 0x04,		/* 工作模式/ */
	MOTOR_MSG_ACK  = 0x05,		/* 应答 */
	MOTOR_MSG_HEART  = 0x06,	/* 心跳 */
	EU_MSG  		= 0x07,		/* EUx00 消息 */
} sr_motor_msg_type_t;


typedef enum {
    DEVICE_NONE = 0x00,  // 状态不可用

    // 0x01 ~ 0x0F

    DEVICE_OK = 0x01,  // 工作正常

    // 0x10 ~ 0x1F

    DEVICE_INITIALIZING = 0x10,  // 初始化中

    // 0x20 ~ 0x2F

    DEVICE_OFF = 0x40,  // 设备正常关闭

    // 0x30 ~ 0x7F
    // RESERVED

    // 0x80 ~ 0xFF

    DEVICE_ERROR = 0x80,              // 其他ERROR
    DEVICE_ERROR_OPEN_FAILED = 0x81,  // 设备打开失败
    DEVICE_ERROR_TIMEOUT = 0x82,      // 通信超时
	DEVICE_ERROR_INITIAL = 0x93, 	// 初始化出错

} sr_device_state_t;

struct dev_status {
	struct _device_manager *dev;
	uint8_t data[8];
};

struct _device_manager;

typedef int32_t (*exception_handle)(struct _device_manager *device, sr_motor_msg_type_t type, uint8_t* data);
typedef  void (*device_rx_cb)(sr_device_id_t dev, sr_motor_msg_type_t type, uint8_t* data);


typedef int32_t (*p_func)(sr_device_id_t dev, uint8_t *data, uint16_t len);

struct _device_manager {
	char *name;
	sr_device_id_t type;
	uint16_t minor;
	uint32_t model[4];
	uint32_t serial_no[8];
	uint32_t drv_serial_no[8];
	uint32_t status;
	uint32_t error_code;
	uint32_t io_status;
	uint32_t status_op_word;
	struct _device_manager *pre;
	struct _device_manager *next;
	p_func hb_handler;
	device_rx_cb rx_cb;
	exception_handle e_handler;
	int32_t (*msg_map)(uint16_t can_id, sr_motor_msg_type_t *type);
};



/**
 * 设备管理器，注册新增设备到系统
 *
 * @param[in]	dev_name:
 * @param[in]	cookie:
 * @param[out]	None.
 *
 * @return		0表示成功,其它返回失败
 */
int32_t sr_device_add(const char *dev_name, char *model, sr_device_id_t type,
							uint16_t minor, p_func function);

int32_t sr_device_del(const char *dev_name, sr_device_id_t dev);

int32_t sr_device_list(void);

/**
 * 设备管理器，更新设备信息
 *
 * @param[in]	dev_name:
 * @param[in]	cookie:
 * @param[out]	None.
 *
 * @return		0表示成功,其它返回失败
 */
int32_t sr_device_update(struct _device_manager *dev, uint32_t status, uint32_t sta_code);

struct _device_manager *sr_device_find(const char *dev_name, sr_device_id_t dev);

const char *dev_class_maps(sr_device_id_t type, enum dev_options_t op);
uint16_t dev_class_map2minor(sr_device_id_t type);

struct _device_manager *sr_device_get_node(void);
int32_t sr_device_total_check(void);
int32_t sr_device_manager_init(void);
QueueHandle_t get_dev_status_handle(void);
uint32_t device_state_maps(sr_device_id_t type, uint32_t state);

#ifdef __cplusplus
}
#endif

#endif /* __DEVICE_H__ */
