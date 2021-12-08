#ifndef __SR_MOTOR_KINCO_H__
#define __SR_MOTOR_KINCO_H__


#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>
#include "bsp\include\drv_can.h"
#include "drivers\include\device.h"
#include "drivers\include\drivers\sr_motor.h"


	

typedef int32_t (*kinco_rx_handler)(uint8_t type, CanRxMsg *rx_msg);


int32_t kinco_fd122_init(struct _device_manager *device, sr_device_id_t type,
								device_rx_cb cb);
int32_t kinco_fd122_info_get(uint16_t pos, uint8_t* buf, uint16_t* len);


/*
 * kinco上电或断电
 *
 * @param[in]	pos: 电机位置
 * @param[in]	state: 使能或关闭
 * @param[out]	None
 *
 * @return		>=0成功，其他失败
 */
int32_t kinco_fd122_enable(sr_device_id_t dev, bool state);

/*
 * kinco fd122 清除指定电机错误状态
 *
 * @param[in]	pos: 电机位置
 * @param[out]	None
 *
 * @return		>=0成功，其他失败
 */
int32_t kinco_fd122_err_clr(sr_device_id_t dev);

int32_t kinco_fd122_pdo_enable(struct _device_manager *device);

int32_t kinco_fd122_speed_contorl(sr_device_id_t dev, int16_t speed);

int32_t kinco_fd122_write(sr_device_id_t dev, uint8_t* data, 
							uint8_t len, uint16_t timeout);

/*
 * kinco电机模式选择
 *
 * @param[in]	pos: 电机位置
 * @param[in]	mode: 模式
 * @param[out]	None
 *
 * @return		>=0成功，其他失败
 */
int32_t kinco_fd122_mode_select(sr_device_id_t dev);

/*
 * kinco节点保护，需要按照驱动器设置的时间内下发
 *
 * @param[in]	pos: 电机所在位置
 * @param[out]	None
 *
 * @return		>=0成功，其他失败
 */
int32_t kinco_node_protect(sr_device_id_t dev, uint8_t *data1, uint16_t len);

/*
 * 查询驱动器编号操作A，CAN_ID：60x其中x表示驱动器地址编号，
 * 反馈58x，x驱动器地址
 *
 * @param[in]	pos: 电机所在位置
 * @param[out]	None
 *
 * @return		>=0成功，其他失败
 */
int32_t kinco_num_a_check(sr_device_id_t dev);

/*
 * 查询驱动器编号操作B，CAN_ID：60x其中x表示驱动器地址编号，
 * 反馈58x，x驱动器地址
 *
 * @param[in]	pos: 电机所在位置
 * @param[out]	None
 *
 * @return		>=0成功，其他失败
 */
int32_t kinco_num_b_check(sr_device_id_t dev);

int32_t sr_kinco_init(void);
uint16_t* kinco_motor_id_get(void);

int32_t kinco_register(sr_motor_vendor_t vendor, kinco_rx_handler handle);
int32_t kinco_emergency_cancel(sr_device_id_t dev);
int32_t kinco_emergency_stop(sr_device_id_t dev);

#ifdef __cplusplus
}
#endif
#endif /* __SR_MOTOR_KINCO_H__ */

