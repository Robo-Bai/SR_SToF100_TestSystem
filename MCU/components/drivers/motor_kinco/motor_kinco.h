#ifndef __MOTOR_KINCO_H
#define __MOTOR_KINCO_H

#include "sys.h"

//#define motor_kinco_ID1     0x601  //X轴电机驱动器ID
//int32_t kinco_enable(uint32_t motor_kinco_ID, int state);

int32_t kinco_enable(uint32_t motor_kinco_ID);//电机使能
int32_t kinco_disable(uint32_t motor_kinco_ID);//电机使能
int32_t kinco_motor_speed(uint32_t motor_kinco_ID);//电机速度
int32_t kinco_motor_mode(uint32_t motor_kinco_ID);//电机模式选择


void motor_active(uint32_t Motor_ID);//电机动作


#endif


