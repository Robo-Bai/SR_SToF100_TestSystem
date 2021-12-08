#ifndef __DRV_IMU_H__
#define __DRV_IMU_H__


#include "sr\app\include\system.h"


#define IMU_485 0

void ReadIMU(void);
void IMU_StartRun(void);
void IMU_Init(void);
void SendIMUQuery(void);
void IMU_SendClear(void);
float HexArr2Num(uint8_t *hexArr);
extern uint8_t IMU_Status;
extern uint8_t IMU_RxOK;

#endif /* __DRV_IMU_H__ */

