#include <string.h>
#include "drivers\imu\sr_imu.h"
#include "srtos\srconfig.h"
#include "sr\app\include\Global_Declare.h"
#include "bsp\include\usart.h"
#include "sr\app\include\src_server.h"
#include "sr\app\location\include\sr_location.h"
#include "sr\app\dbug\include\ParameterDebug.h"

typedef struct
{
	float 	Roll;
	float 	Pitch;
	float	Yaw;
	float  X_Accel;
	float  Z_AngularVel;  //机器人的角速度
	uint32_t		Timestamp;//间隔时间单位ms
}AHRS_TypeDef;

AHRS_TypeDef ahrs_INS550;
uint8_t ahrs_INS550_Array[20];

float currentYaw;
float deltaYaw ;
float IMU_Angle=0;
float Current_Angle=0;
float IMU_Accl;
uint8_t IMU_RxOK = 0;
//uint32_t IMU_Send_count = 0;
void SendIMUQuery()
{
	uint8_t cmd[1] = {0x30};
	ser3WriteData(cmd,1);
	ser3.RxOK = 0;
	IMU_RxOK = 1;   
	//IMU_Send_count++;
}
void IMU_Init(void)
{
	uint8_t cmd[1] = {0x09};
	ser3WriteData(cmd,1);
	ser3.RxOK = 0;
}
//-----------------IMU开始积分-------------------//
void IMU_StartRun()
{
	uint8_t cmd[1] = {0x10};
	ser3WriteData(cmd,1);
	ser3.RxOK = 0;
}
void IMU_SendClear()
{
	uint8_t cmd[1] = {0x20};
	ser3WriteData(cmd,1);
	ser3.RxOK = 0;
}
uint8_t IMU_Status = 0;
float IMU_Wz;
//uint32_t IMU_Rec_count = 0;
void ReadIMU()
{	
	static uint8_t first_read=1;
	static float lastYaw ;
	if(stCtlPara.IMUorNot==TRUE)
	{
		if (ser3.RxOK == 1) {
			#if (IMU_485 == 1)//导远单轴陀螺仪。
				//IMU_Rec_count++;
				ahrs_INS550.Yaw = HexArr2Num(ser3.RxData);
				IMU_Yaw_IMU= -PI*ahrs_INS550.Yaw/180;
				if(Fabs(ahrs_INS550.Yaw)>950.0f)
				{
					IMU_SendClear();
					first_read = 1;
				}
			#elif (IMU_485 == 0)//广州导远六轴陀螺仪
				ahrs_INS550.Roll = (float)(((int16_t)((ser3.RxData[1]<<8)|(ser3.RxData[0])))*360.0f/32768.0f);
				ahrs_INS550.Pitch = (float)(((int16_t)((ser3.RxData[3]<<8)|(ser3.RxData[2])))*360.0f/32768.0f);
				ahrs_INS550.Yaw = (float)(((int16_t)((ser3.RxData[5]<<8)|(ser3.RxData[4])))*360.0f/32768.0f);
				ahrs_INS550.X_Accel = (float)(((int16_t)((ser3.RxData[17]<<8)|(ser3.RxData[16])))*12.0f/32768.0f);
				ahrs_INS550.Z_AngularVel = (float)(((int16_t)((ser3.RxData[15]<<8)|(ser3.RxData[14])))*573.0f/32768.0f);
				ODO_W = -ahrs_INS550.Z_AngularVel/180.0f*3.141592f;
				ahrs_INS550.Timestamp = (uint32_t)((ser3.RxData[27]<<24) | (ser3.RxData[26]<<16) | (ser3.RxData[25]<<8) | (ser3.RxData[24]));

				ahrs_INS550_Array[0] = (uint8_t)(*((char*)(&ahrs_INS550.Roll)));
				ahrs_INS550_Array[1] = (uint8_t)(*((char*)(&ahrs_INS550.Roll)+1));
				ahrs_INS550_Array[2] = (uint8_t)(*((char*)(&ahrs_INS550.Roll)+2));
				ahrs_INS550_Array[3] = (uint8_t)(*((char*)(&ahrs_INS550.Roll)+3));
				ahrs_INS550_Array[4] = (uint8_t)(*((char*)(&ahrs_INS550.Pitch)));
				ahrs_INS550_Array[5] = (uint8_t)(*((char*)(&ahrs_INS550.Pitch)+1));
				ahrs_INS550_Array[6] = (uint8_t)(*((char*)(&ahrs_INS550.Pitch)+2));
				ahrs_INS550_Array[7] = (uint8_t)(*((char*)(&ahrs_INS550.Pitch)+3));
				ahrs_INS550_Array[8] = (uint8_t)(*((char*)(&ahrs_INS550.Yaw)));
				ahrs_INS550_Array[9] = (uint8_t)(*((char*)(&ahrs_INS550.Yaw)+1));
				ahrs_INS550_Array[10] = (uint8_t)(*((char*)(&ahrs_INS550.Yaw)+2));
				ahrs_INS550_Array[11] = (uint8_t)(*((char*)(&ahrs_INS550.Yaw)+3));
				ahrs_INS550_Array[12] = (uint8_t)((ahrs_INS550.Timestamp & 0XFF000000) >> 24);
				ahrs_INS550_Array[13] = (uint8_t)((ahrs_INS550.Timestamp & 0X00FF0000) >> 16);
				ahrs_INS550_Array[14] = (uint8_t)((ahrs_INS550.Timestamp & 0X0000FF00) >> 8);
				ahrs_INS550_Array[15] = (uint8_t)(ahrs_INS550.Timestamp & 0X000000FF);
				//send_usart_data(ahrs_INS550_Array,16);
				
				IMU_Yaw_IMU= -PI*ahrs_INS550.Yaw/180;
				IMU_Pitch = -PI*ahrs_INS550.Pitch/180;  ///角度转化为弧度;
				IMU_Roll =  PI*ahrs_INS550.Roll/180;   ///角度转化为弧度；
				IMU_Accl = ahrs_INS550.X_Accel;
			#elif (IMU_485 == 2)//深圳维特智能6轴imu
				IMU_Wz=(float)(((int16_t)((ser3.RxData[5]<<8)|(ser3.RxData[4])))*2000.0f/32768.0f)/180.0f*PI;
				
				
				
				
				
			#endif

			if (first_read) 
			{
				first_read =0;
				lastYaw = IMU_Yaw_IMU;
			}
			
			currentYaw=IMU_Yaw_IMU;
			deltaYaw = currentYaw-lastYaw;
			lastYaw = IMU_Yaw_IMU;
			
			deltaYaw = sr_convert_angle(deltaYaw);
			
			IMU_Angle=deltaYaw+IMU_Angle;
			Current_Angle = deltaYaw+Current_Angle;
			//由于上电时IMU角度值不为0.所以必须转换。最后得到IMU_Angle。
			IMU_Angle=sr_convert_angle(IMU_Angle);
			
			//机器人角度值是以地图的坐标有关。与机器人上电时角度无关。所以
			//通过SLAM定位之后机器人的角度值是会发生变化的。
			Current_Angle=sr_convert_angle(Current_Angle);
			
			ser3.RxOK = 0;
		}
	}			
}

// 根据本传感器的协议来设置的转换函数: -000.01234
float HexArr2Num(uint8_t *hexArr)
{
	int inx = 2;
	float byte = 100.0f;
	float fpIMUDeg = 0;
	for(inx = 2; inx < 11;inx++)
	{
		if(hexArr[inx] !=0x2E)
		{
			fpIMUDeg += Hex2Ascii(hexArr[inx]) * byte;
			byte = byte * 0.1;
			
		}
	}
	if(hexArr[1] == 0x2D)
	{
		fpIMUDeg = -fpIMUDeg;
	}
	return fpIMUDeg;
}

