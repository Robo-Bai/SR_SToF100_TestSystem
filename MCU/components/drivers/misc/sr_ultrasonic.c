#include "ultrasonic.h"
#include "srtos\srconfig.h"

#ifdef ULTRASONIC_ENABLE

//uint8_t middle1 = 0;
//uint8_t middle2 = 0;
//uint8_t middle3 = 0;
//uint8_t middle4 = 0;
//uint8_t middle5 = 0;
//uint8_t middle6 = 0;

float middle1 = 0;
float middle2 = 0;
float middle3 = 0;
float middle4 = 0;
float middle5 = 0;
float middle6 = 0;
float middle7 = 0;
float middle8 = 0;

uint8_t ucBlockfront = 0;//??????????
uint8_t ucBlockback = 0;//???????????
uint8_t ucBlockturn = 0;//??????????
uint8_t block_lr = 0;//????????????

uint16_t deltaTime_UltraSonic[9][5]={
										{65535,65535,65535,65535,65535},
										{65535,65535,65535,65535,65535},//ultrasonic1
										{65535,65535,65535,65535,65535},//ultrasonic2
										{65535,65535,65535,65535,65535},//ultrasonic3
										{65535,65535,65535,65535,65535},//ultrasonic4
										{65535,65535,65535,65535,65535},//ultrasonic5
										{65535,65535,65535,65535,65535},//ultrasonic6
										{65535,65535,65535,65535,65535},//ultrasonic7
										{65535,65535,65535,65535,65535},//ultrasonic8
									};
uint8_t ultraSonicLoop_6s = 1;
uint8_t ultraSonicLoop_8s = 1;									
UltraSonic_TypeDef ultraSonic1;
UltraSonic_TypeDef ultraSonic2;
UltraSonic_TypeDef ultraSonic3;
UltraSonic_TypeDef ultraSonic4;
UltraSonic_TypeDef ultraSonic5;
UltraSonic_TypeDef ultraSonic6;
UltraSonic_TypeDef ultraSonic7;
UltraSonic_TypeDef ultraSonic8;	


void ultraSonic_BlockDis(void)
{
	middle1=Middle(deltaTime_UltraSonic[1]);//???2??
	middle2=Middle(deltaTime_UltraSonic[2]);
	middle3=Middle(deltaTime_UltraSonic[3]);
	middle4=Middle(deltaTime_UltraSonic[4]);
	middle5=Middle(deltaTime_UltraSonic[5]);
	middle6=Middle(deltaTime_UltraSonic[6]);
	middle7=Middle(deltaTime_UltraSonic[7]);//????2??
	middle8=Middle(deltaTime_UltraSonic[8]);
	ucBlockfront = (uint8_t)(0.017f*MIN(middle1,middle2));
	ucBlockback = (uint8_t)(0.017f*MIN(middle7,middle8));
//	block_lr = (uint8_t)(0.017f*MIN(middle3,middle4));
	ucBlockturn = (uint8_t)(MIN(ucBlockfront,ucBlockback));//????????????????	
//	ucBlockturn = (uint8_t)(MIN(ucBlockturn,block_lr));//?????¦¶6????????????????
}

									
void ultraSonic1_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

	//TRIG1[PC2] init
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//Enable GPIOC clock
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//GPIO Output Mode
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//Push Pull
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//Pull Down
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOC,GPIO_Pin_2);//TIRG1 = 0
	
	//ECHO1[PC0] init
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//Enable GPIOC clock
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//GPIO Input Mode
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//Pull Down
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	//EXTI0 init
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//enable SYSCFG clock
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource0);//connect PC0 to EXTI0
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//trig mode
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	//NVIC init
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
void ultraSonic2_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

	//TRIG2[PG6] init
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);//Enable GPIOG clock
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//GPIO Output Mode
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//Push Pull
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//Pull Down
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOG,GPIO_Pin_6);//TIRG2 = 0
	
	//ECHO2[PE2] init
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//Enable GPIOE clock
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//GPIO Input Mode
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//Pull Down
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	//EXTI2 init
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//enable SYSCFG clock
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource2);//connect PE2 to EXTI2
	EXTI_InitStructure.EXTI_Line = EXTI_Line2;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//trig mode
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	//NVIC init
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void ultraSonic3_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

	//TRIG3[PB13] init
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//Enable GPIOB clock
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//GPIO Output Mode
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//Push Pull
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//Pull Down
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_13);//TIRG3 = 0
	
	//ECHO3[PE3] init
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//Enable GPIOE clock
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//GPIO Input Mode
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//Pull Down
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	//EXTI3 init
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//enable SYSCFG clock
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource3);//connect PE3 to EXTI3
	EXTI_InitStructure.EXTI_Line = EXTI_Line3;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//trig mode
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	//NVIC init
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void ultraSonic4_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

	//TRIG4[PA6] init
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//Enable GPIOA clock
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//GPIO Output Mode
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//Push Pull
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//Pull Down
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_6);//TIRG4 = 0
	
	//ECHO4[PE4] init
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//Enable GPIOE clock
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//GPIO Input Mode
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//Pull Down
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	//EXTI4 init
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//enable SYSCFG clock
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource4);//connect PE4 to EXTI4
	EXTI_InitStructure.EXTI_Line = EXTI_Line4;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//trig mode
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	//NVIC init
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void ultraSonic5_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

	//TRIG5[PA4] init
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//Enable GPIOA clock
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//GPIO Output Mode
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//Push Pull
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//Pull Down
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);//TIRG5 = 0
	
	//ECHO5[PE5] init
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//Enable GPIOE clock
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//GPIO Input Mode
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//Pull Down
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	//EXTI5 init
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//enable SYSCFG clock
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource5);//connect PE5 to EXTI5
	EXTI_InitStructure.EXTI_Line = EXTI_Line5;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//trig mode
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	//NVIC init
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
void ultraSonic6_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

	//TRIG6[PA3] init
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//Enable GPIOA clock
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//GPIO Output Mode
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//Push Pull
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//Pull Down
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_3);//TIRG6 = 0
	
	//ECHO6[PE6] init
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//Enable GPIOE clock
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//GPIO Input Mode
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//Pull Down
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	//EXTI6 init
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//enable SYSCFG clock
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource6);//connect PE6 to EXTI6
	EXTI_InitStructure.EXTI_Line = EXTI_Line6;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//trig mode
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	//NVIC init
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void ultraSonic7_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

	//TRIG7[PA0] init
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//Enable GPIOA clock
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//GPIO Output Mode
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//Push Pull
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//Pull Down
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);//TIRG7 = 0
	
	//ECHO7[PF9] init
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//Enable GPIOF clock
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//GPIO Input Mode
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//Pull Down
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	//EXTI9 init
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//enable SYSCFG clock
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource9);//connect PF9 to EXTI9
	EXTI_InitStructure.EXTI_Line = EXTI_Line9;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//trig mode
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	//NVIC init
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
void ultraSonic8_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

	//TRIG8[PC3] init
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//Enable GPIOC clock
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//GPIO Output Mode
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//Push Pull
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//Pull Down
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOC,GPIO_Pin_3);//TIRG8 = 0
	
	//ECHO8[PF10] init
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//Enable GPIOF clock
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//GPIO Input Mode
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//Pull Down
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	//EXTI10 init
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//enable SYSCFG clock
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource10);//connect PF10 to EXTI10
	EXTI_InitStructure.EXTI_Line = EXTI_Line10;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//trig mode
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	//NVIC init
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
void ultraSonic1_Trig(void)
{
	TRIG1 = 1;
	delay_us(20);
	TRIG1 = 0;
	EXTI->RTSR = EXTI_ULTRASONIC_1;//rising trig
	EXTI->FTSR = 0;
	//ultraSonic1.Echo_Status = ECHO_WAITING;
}
void ultraSonic2_Trig(void)
{
	TRIG2 = 1;
	delay_us(20);
	TRIG2 = 0;
	EXTI->RTSR = EXTI_ULTRASONIC_2;//rising trig
	EXTI->FTSR = 0;
	//ultraSonic1.Echo_Status = ECHO_WAITING;
}
void ultraSonic3_Trig(void)
{
	TRIG3 = 1;
	delay_us(20);
	TRIG3 = 0;
	EXTI->RTSR = EXTI_ULTRASONIC_3;//rising trig
	EXTI->FTSR = 0;
	//ultraSonic1.Echo_Status = ECHO_WAITING;
}
void ultraSonic4_Trig(void)
{
	TRIG4 = 1;
	delay_us(20);
	TRIG4 = 0;
	EXTI->RTSR = EXTI_ULTRASONIC_4;//rising trig
	EXTI->FTSR = 0;
	//ultraSonic1.Echo_Status = ECHO_WAITING;
}
void ultraSonic5_Trig(void)
{
	TRIG5 = 1;
	delay_us(20);
	TRIG5 = 0;
	EXTI->RTSR = EXTI_ULTRASONIC_5;//rising trig
	EXTI->FTSR = 0;
	//ultraSonic1.Echo_Status = ECHO_WAITING;
}
void ultraSonic6_Trig(void)
{
	TRIG6 = 1;
	delay_us(20);
	TRIG6 = 0;
	EXTI->RTSR = EXTI_ULTRASONIC_6;//rising trig
	EXTI->FTSR = 0;
	//ultraSonic1.Echo_Status = ECHO_WAITING;
}
void ultraSonic7_Trig(void)
{
	TRIG7 = 1;
	delay_us(20);
	TRIG7 = 0;
	EXTI->RTSR = EXTI_ULTRASONIC_7;//rising trig
	EXTI->FTSR = 0;
	//ultraSonic1.Echo_Status = ECHO_WAITING;
}
void ultraSonic8_Trig(void)
{
	TRIG8 = 1;
	delay_us(20);
	TRIG8 = 0;
	EXTI->RTSR = EXTI_ULTRASONIC_8;//rising trig
	EXTI->FTSR = 0;
	//ultraSonic1.Echo_Status = ECHO_WAITING;
}

//ultrasonic1 interrupt
void EXTI0_IRQHandler(void)
{
	uint16_t time_Echo_Temp = 0;
	time_Echo_Temp = TIM7->CNT;
	
	if((EXTI->PR & EXTI_ULTRASONIC_1) != 0)
	{
		if((EXTI->RTSR&EXTI_ULTRASONIC_1) != 0)//rising trig
		{
			ultraSonic1.Time_EchoUp = time_Echo_Temp;
			EXTI->RTSR = 0;
			EXTI->FTSR = EXTI_ULTRASONIC_1;//config to falling trig
			ultraSonic1.Echo_Status = ECHO_RISING_CHECKED;
		}
		else//falling trig
		{
			ultraSonic1.Time_EchoDown = time_Echo_Temp;
			EXTI->RTSR = 0;//no trig
			EXTI->FTSR = 0;//no trig
			ultraSonic1.Echo_Status = ECHO_FALLING_CHECKED;
		}
		//clear interrupt flag
		EXTI->PR = EXTI_ULTRASONIC_1;
	}
}
//ultrasonic2 interrupt
void EXTI2_IRQHandler(void)
{
	uint16_t time_Echo_Temp = 0;
	time_Echo_Temp = TIM7->CNT;
	
	if((EXTI->PR & EXTI_ULTRASONIC_2) != 0)
	{
		if((EXTI->RTSR&EXTI_ULTRASONIC_2) != 0)//rising trig
		{
			ultraSonic2.Time_EchoUp = time_Echo_Temp;
			EXTI->RTSR = 0;
			EXTI->FTSR = EXTI_ULTRASONIC_2;//config to falling trig
			ultraSonic2.Echo_Status = ECHO_RISING_CHECKED;
		}
		else//falling trig
		{
			ultraSonic2.Time_EchoDown = time_Echo_Temp;
			EXTI->RTSR = 0;//no trig
			EXTI->FTSR = 0;//no trig
			ultraSonic2.Echo_Status = ECHO_FALLING_CHECKED;
		}
		//clear interrupt flag
		EXTI->PR = EXTI_ULTRASONIC_2;
	}
}
//ultrasonic3 interrupt
void EXTI3_IRQHandler(void)
{
	uint16_t time_Echo_Temp = 0;
	time_Echo_Temp = TIM7->CNT;
	
	if((EXTI->PR & EXTI_ULTRASONIC_3) != 0)
	{
		if((EXTI->RTSR&EXTI_ULTRASONIC_3) != 0)//rising trig
		{
			ultraSonic3.Time_EchoUp = time_Echo_Temp;
			EXTI->RTSR = 0;
			EXTI->FTSR = EXTI_ULTRASONIC_3;//config to falling trig
			ultraSonic3.Echo_Status = ECHO_RISING_CHECKED;
		}
		else//falling trig
		{
			ultraSonic3.Time_EchoDown = time_Echo_Temp;
			EXTI->RTSR = 0;//no trig
			EXTI->FTSR = 0;//no trig
			ultraSonic3.Echo_Status = ECHO_FALLING_CHECKED;
		}
		//clear interrupt flag
		EXTI->PR = EXTI_ULTRASONIC_3;
	}
}
//ultrasonic4 interrupt
void EXTI4_IRQHandler(void)
{
	uint16_t time_Echo_Temp = 0;
	time_Echo_Temp = TIM7->CNT;
	
	if((EXTI->PR & EXTI_ULTRASONIC_4) != 0)
	{
		if((EXTI->RTSR&EXTI_ULTRASONIC_4) != 0)//rising trig
		{
			ultraSonic4.Time_EchoUp = time_Echo_Temp;
			EXTI->RTSR = 0;
			EXTI->FTSR = EXTI_ULTRASONIC_4;//config to falling trig
			ultraSonic4.Echo_Status = ECHO_RISING_CHECKED;
		}
		else//falling trig
		{
			ultraSonic4.Time_EchoDown = time_Echo_Temp;
			EXTI->RTSR = 0;//no trig
			EXTI->FTSR = 0;//no trig
			ultraSonic4.Echo_Status = ECHO_FALLING_CHECKED;
		}
		//clear interrupt flag
		EXTI->PR = EXTI_ULTRASONIC_4;
	}
}
//ultrasonic5/6/7 interrupt
void EXTI9_5_IRQHandler(void)
{
	uint16_t time_Echo_Temp = 0;
	time_Echo_Temp = TIM7->CNT;
	
	//ultrasonic5
	if((EXTI->PR & EXTI_ULTRASONIC_5) != 0)
	{
		if((EXTI->RTSR&EXTI_ULTRASONIC_5) != 0)//rising trig
		{
			ultraSonic5.Time_EchoUp = time_Echo_Temp;
			EXTI->RTSR = 0;
			EXTI->FTSR = EXTI_ULTRASONIC_5;//config to falling trig
			ultraSonic5.Echo_Status = ECHO_RISING_CHECKED;
		}
		else//falling trig
		{
			ultraSonic5.Time_EchoDown = time_Echo_Temp;
			EXTI->RTSR = 0;//no trig
			EXTI->FTSR = 0;//no trig
			ultraSonic5.Echo_Status = ECHO_FALLING_CHECKED;
		}
		//clear interrupt flag
		EXTI->PR = EXTI_ULTRASONIC_5;
	}
	//ultrasonic6
	if((EXTI->PR & EXTI_ULTRASONIC_6) != 0)
	{
		if((EXTI->RTSR&EXTI_ULTRASONIC_6) != 0)//rising trig
		{
			ultraSonic6.Time_EchoUp = time_Echo_Temp;
			EXTI->RTSR = 0;
			EXTI->FTSR = EXTI_ULTRASONIC_6;//config to falling trig
			ultraSonic6.Echo_Status = ECHO_RISING_CHECKED;
		}
		else//falling trig
		{
			ultraSonic6.Time_EchoDown = time_Echo_Temp;
			EXTI->RTSR = 0;//no trig
			EXTI->FTSR = 0;//no trig
			ultraSonic6.Echo_Status = ECHO_FALLING_CHECKED;
		}
		//clear interrupt flag
		EXTI->PR = EXTI_ULTRASONIC_6;
	}
	//ultrasonic7
	if((EXTI->PR & EXTI_ULTRASONIC_7) != 0)
	{
		if((EXTI->RTSR&EXTI_ULTRASONIC_7) != 0)//rising trig
		{
			ultraSonic7.Time_EchoUp = time_Echo_Temp;
			EXTI->RTSR = 0;
			EXTI->FTSR = EXTI_ULTRASONIC_7;//config to falling trig
			ultraSonic7.Echo_Status = ECHO_RISING_CHECKED;
		}
		else//falling trig
		{
			ultraSonic7.Time_EchoDown = time_Echo_Temp;
			EXTI->RTSR = 0;//no trig
			EXTI->FTSR = 0;//no trig
			ultraSonic7.Echo_Status = ECHO_FALLING_CHECKED;
		}
		//clear interrupt flag
		EXTI->PR = EXTI_ULTRASONIC_7;
	}
}	
//ultrasonic8 interrupt
void EXTI15_10_IRQHandler(void)
{
	uint16_t time_Echo_Temp = 0;
	time_Echo_Temp = TIM7->CNT;
	
	//ultrasonic8
	if((EXTI->PR & EXTI_ULTRASONIC_8) != 0)
	{
		if((EXTI->RTSR&EXTI_ULTRASONIC_8) != 0)//rising trig
		{
			ultraSonic8.Time_EchoUp = time_Echo_Temp;
			EXTI->RTSR = 0;
			EXTI->FTSR = EXTI_ULTRASONIC_8;//config to falling trig
			ultraSonic8.Echo_Status = ECHO_RISING_CHECKED;
		}
		else//falling trig
		{
			ultraSonic8.Time_EchoDown = time_Echo_Temp;
			EXTI->RTSR = 0;//no trig
			EXTI->FTSR = 0;//no trig
			ultraSonic8.Echo_Status = ECHO_FALLING_CHECKED;
		}
		//clear interrupt flag
		EXTI->PR = EXTI_ULTRASONIC_8;
	}
}
uint8_t ultraSonicUpdate(UltraSonic_TypeDef* ultraSonic,uint16_t ultraSonnic_Array[9][5],uint8_t index)
{
	uint8_t status = 0;
	int32_t deltaTime_Temp = 0;
	deltaTime_Temp = (int32_t)(ultraSonic->Time_EchoDown - ultraSonic->Time_EchoUp);
	//deltaTime_Temp = (int32_t)(time_Echo1_Down - time_Echo1_Up);
	
	switch(ultraSonic->Echo_Status)
	{
		case ECHO_FALLING_CHECKED:
		{
			if(deltaTime_Temp < (-30000))
			{
				deltaTime_Temp += 65536;//rising and falling both checked
			}
			status = US_OUT_RF;
		}break;
		case ECHO_RISING_CHECKED:
		{
			deltaTime_Temp = 65535;//only rising checked[the distance is too long]
			status = US_OUT_R;
		}break;
		case ECHO_WAITING:
		{
			status = US_OUT_NOTHING;//only rising checked[the distance is too long,or the sensor did not worked]
		}break;
		default:
		{
			status = US_OUT_ERROR;
		}break;
	}
	
	if((status == ECHO_FALLING_CHECKED)||(status == ECHO_RISING_CHECKED))
	{
		ultraSonnic_Array[index][ultraSonic->ArrayCount++] = (uint16_t)deltaTime_Temp;
		if(ultraSonic->ArrayCount == 5)
		{
			ultraSonic->ArrayCount = 0;
		}
	}
	
	return status;
}

//ultrasonic sensors init
void ultraSonic_Init(uint8_t config)
{
	if((config&0X01) != 0)
	{
		ultraSonic1_Init();
	}
	if((config&0X02) != 0)
	{
		ultraSonic2_Init();
	}
	if((config&0X04) != 0)
	{
		ultraSonic3_Init();
	}
	if((config&0X08) != 0)
	{
		ultraSonic4_Init();
	}
	if((config&0X10) != 0)
	{
		ultraSonic5_Init();
	}
	if((config&0X20) != 0)
	{
		ultraSonic6_Init();
	}
	if((config&0X40) != 0)
	{
		ultraSonic7_Init();
	}
	if((config&0X80) != 0)
	{
		ultraSonic8_Init();
	}
}

//ultrasonic sensors data update loop with 8 sensors
void ultraSonicUpdate_8s(void)
{
	switch(ultraSonicLoop_8s)
	{
		case 1:
		{
			ultraSonic1_Trig();
			ultraSonicUpdate(&ultraSonic1,deltaTime_UltraSonic,1);
		}break;
		case 2:
		{
			ultraSonic2_Trig();
			ultraSonicUpdate(&ultraSonic2,deltaTime_UltraSonic,2);
		}break;
		case 3:
		{
			ultraSonic3_Trig();
			ultraSonicUpdate(&ultraSonic3,deltaTime_UltraSonic,3);
		}break;
		case 4:
		{
			ultraSonic4_Trig();
			ultraSonicUpdate(&ultraSonic4,deltaTime_UltraSonic,4);
		}break;
		case 5:
		{
			ultraSonic5_Trig();
			ultraSonicUpdate(&ultraSonic5,deltaTime_UltraSonic,5);
		}break;
		case 6:
		{
			ultraSonic6_Trig();
			ultraSonicUpdate(&ultraSonic6,deltaTime_UltraSonic,6);
		}break;
		case 7:
		{
			ultraSonic7_Trig();
			ultraSonicUpdate(&ultraSonic7,deltaTime_UltraSonic,7);
		}break;
		case 8:
		{
			ultraSonic8_Trig();
			ultraSonicUpdate(&ultraSonic8,deltaTime_UltraSonic,8);
		}break;
		default:
		{
			
		}break;
	}
	
	ultraSonicLoop_8s++;
	if(ultraSonicLoop_8s == 9)
	{
		ultraSonicLoop_8s = 1;
	}
}

//ultrasonic sensors data update loop with 6 sensors
void ultraSonicUpdate_6s(void)
{
	switch(ultraSonicLoop_6s)
	{
		case 1:
		{
			ultraSonic1_Trig();
			ultraSonicUpdate(&ultraSonic1,deltaTime_UltraSonic,1);
		}break;
		case 2:
		{
			ultraSonic2_Trig();
			ultraSonicUpdate(&ultraSonic2,deltaTime_UltraSonic,2);
		}break;
		case 3:
		{
			ultraSonic3_Trig();
			ultraSonicUpdate(&ultraSonic3,deltaTime_UltraSonic,3);
		}break;
		case 4:
		{
			ultraSonic6_Trig();
			ultraSonicUpdate(&ultraSonic6,deltaTime_UltraSonic,6);
		}break;
		case 5:
		{
			ultraSonic7_Trig();
			ultraSonicUpdate(&ultraSonic7,deltaTime_UltraSonic,7);
		}break;
		case 6:
		{
			ultraSonic8_Trig();
			ultraSonicUpdate(&ultraSonic8,deltaTime_UltraSonic,8);
		}break;
		default:
		{
			
		}break;
	}
	
	ultraSonicLoop_6s++;
	if(ultraSonicLoop_6s == 7)
	{
		ultraSonicLoop_6s = 1;
	}
}

#endif
