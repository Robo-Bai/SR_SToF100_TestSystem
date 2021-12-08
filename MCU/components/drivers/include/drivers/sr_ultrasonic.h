#ifndef __ULTRASONIC_H__
#define __ULTRASONIC_H__

#include "stm32f4xx.h" 
#include "bsp\include\drv_common.h"//�Զ����λ������ͷ�ļ�



#define	TRIG1	PCout(2) 
#define	TRIG2	PGout(6)
#define	TRIG3	PBout(13)
#define	TRIG4	PAout(6)
#define	TRIG5	PAout(4)
#define	TRIG6	PAout(3)
#define	TRIG7	PAout(0)
#define	TRIG8	PCout(3)

#define EXTI_ULTRASONIC_1 EXTI_Line0
#define EXTI_ULTRASONIC_2 EXTI_Line2
#define EXTI_ULTRASONIC_3 EXTI_Line3
#define EXTI_ULTRASONIC_4 EXTI_Line4
#define EXTI_ULTRASONIC_5 EXTI_Line5
#define EXTI_ULTRASONIC_6 EXTI_Line6
#define EXTI_ULTRASONIC_7 EXTI_Line9
#define EXTI_ULTRASONIC_8 EXTI_Line10


#define US_OUT_ERROR	0//code error
#define US_OUT_RF		1//rising and falling both checked
#define US_OUT_R		2//only rising checked[the distance is too long]
#define US_OUT_NOTHING	3//only rising checked[the distance is too long,or the sensor did not worked]

/* Echo_Status_define */
#define ECHO_WAITING			0
#define ECHO_RISING_CHECKED		1
#define ECHO_FALLING_CHECKED	2
/* UltraSonic structure definition */ 
typedef struct
{
	uint8_t 	Echo_Status; /*!<	Specifies the Echo pin status.
								This parameter can be any value of Echo_Status_define */
	uint16_t 	Time_EchoUp;
	uint16_t 	Time_EchoDown;
	uint8_t		ArrayCount;
}UltraSonic_TypeDef;

//extern uint8_t middle1;
//extern uint8_t middle2;
//extern uint8_t middle3;
//extern uint8_t middle4;
//extern uint8_t middle5;
//extern uint8_t middle6;
extern uint8_t ucBlockfront;
extern uint8_t ucBlockback;
extern uint8_t ucBlockturn;

extern uint16_t deltaTime_UltraSonic[9][5];
extern UltraSonic_TypeDef ultraSonic1;
extern UltraSonic_TypeDef ultraSonic2;
extern UltraSonic_TypeDef ultraSonic3;
extern UltraSonic_TypeDef ultraSonic4;
extern UltraSonic_TypeDef ultraSonic5;
extern UltraSonic_TypeDef ultraSonic6;
extern UltraSonic_TypeDef ultraSonic7;
extern UltraSonic_TypeDef ultraSonic8;		

void ultraSonic_Init(uint8_t config);
//void ultraSonic1_Trig(void);
//void ultraSonic2_Trig(void);
//void ultraSonic3_Trig(void);
//void ultraSonic4_Trig(void);
//void ultraSonic5_Trig(void);
//void ultraSonic6_Trig(void);
//void ultraSonic7_Trig(void);
//void ultraSonic8_Trig(void);
//uint8_t ultraSonicUpdate(UltraSonic_TypeDef* ultraSonic,uint16_t ultraSonnic_Array[9][5],uint8_t index);
void ultraSonicUpdate_6s(void);
void ultraSonicUpdate_8s(void);

void ultraSonic_BlockDis(void);


#endif
