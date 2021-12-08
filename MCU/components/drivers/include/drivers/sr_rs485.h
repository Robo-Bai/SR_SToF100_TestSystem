#ifndef __DRV_RS485_H__
#define __DRV_RS485_H__

#include "stm32f4xx.h" 
#include "sr\app\include\sr_def_common.h"
#include "bsp\include\drv_common.h"


//#define FULL_DUPLEX
#define RS485_MAX_LENGTH 100
typedef struct
{
	uint8_t 	RxCount; 
	uint8_t 	RxLength;
	uint8_t 	RxXorCount;
	uint8_t		RxXor;
	uint8_t		RxOK;
	uint8_t		RxData[RS485_MAX_LENGTH];
	uint8_t		TxCount;
	uint8_t		TxLength;
	uint8_t		TxOK;
	uint8_t		TxData[RS485_MAX_LENGTH];
}RS485_TypeDef;

extern int32_t PGV_XPosition;//单位0.1mm
extern int16_t PGV_YPosition;//单位0.1mm
extern uint16_t PGV_AngleValue;//单位0.1度
extern int32_t PGV_TagNumber;
extern uint8_t PGV_ReceiveOK;
extern uint8_t Tag_Enable;//表示已经检测到二维码。
extern uint8_t Tag_Enable1;//表示已经检测到二维码
extern int32_t PGV_XPosition1;//单位0.1mm
extern int16_t PGV_YPosition1;//单位0.1mm
extern uint16_t PGV_ErrCode;
extern uint16_t PGV_ErrCode1;
extern uint16_t PGV_AngleValue1;//单位0.1度
extern int32_t PGV_TagNumber1;
extern uint8_t NL;


extern ST_PGV  RS485UpPGV;
extern ST_PGV  RS485DownPGV;

#define RS485_1_RE PAout(8)
#define RS485_1_TE PAout(15)

#define RS485_T 1
#define RS485_R 0

int32_t sr_rs485_init(uint32_t baudRate);
uint8_t rs485_1_ReadData(uint8_t* array);
uint8_t rs485_1_WriteData(uint8_t* array,uint8_t length);
//void rs485_1_SendData(uint8_t* array,uint8_t length);



void ReadPGV(void);
void getPowerData(void);
void getPower1Data(void);


uint16_t Modbus_CRC16(uint8_t *Buff_addr,uint16_t len);
void getTrackEncoder(int32_t* enc);
void requestTrackEncoder(void);
void StationPrecisionCheck(void);

extern float delta_x;
extern float delta_y;
extern float LineSlope;
extern int32_t PGV_ColorError;
extern  uint16_t PGV_AngleValue_Color;

extern uint8_t StationCheckError;


extern uint8_t PGV_Position;
extern uint8_t PGV_CAN_OK;


uint8_t GetPGVDownState485(void);
uint8_t GetPGVUpState485(void);

float  GetElectronicScale(void);

#endif /* __DRV_RS485_H__ */

