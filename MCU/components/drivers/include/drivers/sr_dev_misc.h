#ifndef __SR_DRV_MISC_H__
#define __SR_DRV_MISC_H__

#include "bsp\include\drv_common.h"


#define DEV_ID_L14						0x0000
#define DEV_ID_EU100_NO1				0x0301
#define DEV_ID_EU100_NO2				0x0302
#define DEV_ID_EU100_NO3				0x0303

#define DEV_ID_RMDS_RESET                              0x00
#define DEV_ID_RMDS_MODE_CHOICE                        0x01
#define DEV_ID_RMDS_PWM_MODE                           0x02
#define DRV_ALL_PWM_CURRENT_MODE_ID                   0x03
#define DRV_ALL_PWM_VELOCITY_MODE_ID                  0x04
#define DRV_ALL_PWM_POSITION_MODE_ID                  0x05
#define DRV_ALL_PWM_VELOCITY_POSITION_MODE_ID         0x06
#define DRV_ALL_CURRENT_VELOCITY_MODE_ID              0x07
#define DRV_ALL_CURRENT_POSITION_MODE_ID              0x08
#define DRV_ALL_CURRENT_VELOCITY_POSITION_MODE_ID     0x09
#define DEV_ID_RMDS_CONFIG                             0x0a
#define DRV_ALL_RETURN_ID                             0x0b

#define DEV_ID_MIKELIMEI_WHEEL_L	0x602
#define DEV_ID_MIKELIMEI_WHEEL_R	0x601

#define DEV_ID_MKLM_MEC_WHEEL_A	0x601
#define DEV_ID_MKLM_MEC_WHEEL_B	0x602
#define DEV_ID_MKLM_MEC_WHEEL_C	0x603
#define DEV_ID_MKLM_MEC_WHEEL_D	0x604









#define OK_LEFT_WHEEL	0x602
#define OK_RIGHT_WHEEL	0x601


#define HTKJ_0				0x0101
#define HTKJ_1				0x0102
#define HTKJ_2				0x0103
#define HTKJ_3				0x0104

#define ENTER_PWM_MODE                                0x01
#define ENTER_PWM_CURRENT_MODE                        0x02
#define ENTER_PWM_VELOCITY_MODE                       0x03
#define ENTER_PWM_POSITION_MODE                       0x04
#define ENTER_PWM_VELOCITY_POSITION_MODE              0x05
#define ENTER_CURRENT_VELOCITY_MODE                   0x06
#define ENTER_CURRENT_POSITION_MODE                   0x07
#define ENTER_CURRENT_VELOCITY_POSITION_MODE          0x08


int32_t misc_dev_init(void);
int32_t sr_l14_init(void);

int32_t mklm_speed_crl(uint16_t can_id, int16_t speed);
int32_t tiean_motor_init(void);

int32_t jql_fork_encoder_init(void);
int32_t sr_motor_driver_init(void);
int32_t aeropod_climb_motor_init(void);
int32_t oasis_tray_motor_init(void);
int32_t jql_fork_lift_init(void);
int32_t curtis_heart_beat(void);
int32_t curtis_manual_enable(void);




#endif /* __SR_DRV_MISC_H__ */

