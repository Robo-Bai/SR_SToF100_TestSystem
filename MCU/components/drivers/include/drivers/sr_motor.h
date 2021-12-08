#ifndef __SR_MOTOR_H__
#define __SR_MOTOR_H__

#ifdef __cplusplus
 extern "C" {
#endif


#include <stdint.h>
#include "srtos\srtos.h"
#include "drivers\include\device.h"


//电机和动作控制周期。
#define TIME_MOTOR					20	/* 100Hz //2016年11月2日 远能现场修改，由于调节频率较高,改成20ms for L14 */
#define MOTOR_PERIOD 				TIME_MOTOR*0.001f

#define RAD_TO_DEGREE(x)   			(((x)/PI)*180.0f)
#define DEGREE_TO_RAD(x)   			(((x)/180.0f)*PI)


#define	MOTOR_MODEL_KINCO_FD122		"KINCO_FD122"


typedef enum _motor_model {
	KINCO_FD122 = 0x01,

} sr_motor_model;

typedef enum _sid_heart {
	SR_SID,
	SR_HEART,
} sr_sid_heart_t;




typedef enum _motor_vendor{
	Invalid = 0,
	Kinco = 0x01,
	Nanotec = 0x02,

} sr_motor_vendor_t;


int32_t motor_init(sr_device_id_t type, sr_motor_model model, device_rx_cb cb);
int32_t motor_deinit(sr_device_id_t dev);
int16_t motor_write(sr_device_id_t dev, uint8_t* data, uint8_t len, uint16_t timeout);
int32_t motor_emergency_cancel(sr_device_id_t dev);

int32_t motor_speed_control(sr_device_id_t dev, int16_t speed);
int32_t motor_err_clr(sr_device_id_t dev);
int32_t  motor_enable(sr_device_id_t dev, bool state);
int32_t motor_pdo_enable(sr_device_id_t dev);
int32_t motor_mode_select(sr_device_id_t dev);
int32_t motor_emergency_stop(sr_device_id_t dev);

#ifdef __cplusplus
}
#endif
#endif /* __SR_MOTOR_H__ */

