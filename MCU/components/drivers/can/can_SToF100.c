#include "can_SToF100.h"
#include "string.h"
#include "blink.h"
#include "delay.h"
#include "SH200.h"
#include "exti.h"
#include "SH200.h"
#include "motor_kinco.h"
#include "led.h"

void CAN_Config(void)
{
	GPIO_InitTypeDef       GPIO_InitStructure;
	CAN_InitTypeDef        CAN_InitStructure;
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
	//  CanTxMsg TxMessage;

	CAN_DeInit(CAN1);
	/* Configure CAN1 and CAN2 IOs **********************************************/
	/* GPIOB, GPIOD and AFIO clocks enable */
	//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  
	/* Configure CAN1 RX pin */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
   

	/* Configure CAN1 TX pin */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Remap CAN1 and CAN2 GPIOs */
	//  GPIO_PinRemapConfig(GPIO_Remap2_CAN1 , ENABLE);//启用会导致TCAN中初始化CAN1失败

	/* Configure CAN1 and CAN2 **************************************************/  
	/* CAN1 and CAN2 Periph clocks enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);  
  
	/* CAN1 and CAN2 register init */
	//  CAN_DeInit(CAN1);

	/* Struct init*/
	//CAN_StructInit(&CAN_InitStructure);

  //CAN1单元设置
	CAN_InitStructure.CAN_TTCM = DISABLE;
	CAN_InitStructure.CAN_ABOM = DISABLE;
	CAN_InitStructure.CAN_AWUM = DISABLE;
	CAN_InitStructure.CAN_NART = DISABLE;
	CAN_InitStructure.CAN_RFLM = DISABLE;
	CAN_InitStructure.CAN_TXFP = ENABLE;
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
	//设置波特率500
//	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;  
//  CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;
//  CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;

	CAN_InitStructure.CAN_SJW       = CAN_SJW_1tq;                
	CAN_InitStructure.CAN_BS1       = CAN_BS1_9tq;          
	CAN_InitStructure.CAN_BS2       = CAN_BS2_8tq;   
	
  /*--------设置分频系数----------*/
#if CAN_BAUDRATE == 1000 /* 1MBps */
  CAN_InitStructure.CAN_Prescaler =6;
#elif CAN_BAUDRATE == 500 /* 500KBps */
//  CAN_InitStructure.CAN_Prescaler =12;
	CAN_InitStructure.CAN_Prescaler = 4;
#elif CAN_BAUDRATE == 250 /* 250KBps */
  CAN_InitStructure.CAN_Prescaler =24;
#elif CAN_BAUDRATE == 125 /* 125KBps */
  CAN_InitStructure.CAN_Prescaler =48;
#elif  CAN_BAUDRATE == 100 /* 100KBps */
  CAN_InitStructure.CAN_Prescaler =60;
#elif  CAN_BAUDRATE == 50 /* 50KBps */
  CAN_InitStructure.CAN_Prescaler =120;
#elif  CAN_BAUDRATE == 20 /* 20KBps */
  CAN_InitStructure.CAN_Prescaler =300;
#elif  CAN_BAUDRATE == 10 /* 10KBps */
  CAN_InitStructure.CAN_Prescaler =600;
#else
   #error "Please select first the CAN Baudrate in Private defines in main.c "
#endif  /* CAN_BAUDRATE == 1000 */
				
	/*------------------------------------------------------*/
	//初始化CAN1
	CAN_Init(CAN1, &CAN_InitStructure);

	/*-------------------- CAN Fileter Configuuration ---------------------*/
	
	CAN_FilterInitStructure.CAN_FilterNumber = 0;                         //过滤器0
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;      //32位
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;                    //32位ID，不过滤任何
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;                //32位mask
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;  //过滤器0关联到FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;                //激活过滤器0
	CAN_FilterInit(&CAN_FilterInitStructure);                             //过滤器初始化
	
	/*--------------------- CAN NVIC Configuration ----------------------*/
	NVIC_InitTypeDef  NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel                   = CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel                   = CAN1_TX_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* IT Configuration for CAN1 开启CAN中断*/  
	CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);
	CAN_ITConfig(CAN1,CAN_IT_TME,ENABLE);
}



/*************************************************************************
                          CAN1_TX_IRQHandler
*************************************************************************/
//unsigned char can1_tx_success_flag=0;//CAN1发送成功标志位

void CAN1_TX_IRQHandler(void){
	
	if (CAN_GetITStatus(CAN1,CAN_IT_TME)!= RESET){
		CAN_ClearITPendingBit(CAN1,CAN_IT_TME);
		//can1_tx_success_flag=1;
	}
}

/*************************************************************************
                          CAN1_RX0_IRQHandler
*************************************************************************/
extern unsigned char SH200_INconfig_flag; //SH200进入配置成功标志位
extern unsigned char SH200_transmit_flag;  //SH200使能主动上报成功标志位
extern unsigned char SH200_OUTconfig_flag;  //SH200退出配置成功标志位
extern unsigned char SH200_activate_flag;  //SH200激活成功标志位

extern unsigned char kinco_enable_flag; //使能成功标志位
extern unsigned char kinco_disable_flag;  //关闭使能成功标志位
extern unsigned char kinco_motor_speed_flag;  //电机速度赋值成功标志位
extern unsigned char kinco_motor_mode_flag;  //电机速度赋值成功标志位

extern unsigned char LED_X_Origin_flag;  //X轴回原点成功标志位
extern unsigned char LED_Z_Origin_flag;  //Z轴回原点成功标志位
extern unsigned char LED_R_Origin_flag;  //R轴回原点成功标志位


CanRxMsg RxMessage;
void CAN1_RX0_IRQHandler(void)
{
	uint32_t SH200_index = 0;
//	uint32_t Kinco_index = 0;
	uint32_t State_word_index = 0;
//	static uint8_t active_flag = 0;
	unsigned char SH200_init_flag = 0;  //SH200配置完成标志位
	
	if (CAN_GetITStatus(CAN1,CAN_IT_FMP0)!= RESET)
	{
		CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
		SH200_index = (uint32_t)((RxMessage.Data[1]<<16)|(RxMessage.Data[2]<<24)|(RxMessage.Data[3]<<8)|(RxMessage.Data[4]));
//		Kinco_index = (uint32_t)((RxMessage.Data[1]<<16)|(RxMessage.Data[2]<<24)|(RxMessage.Data[3]<<8)|(RxMessage.Data[4]));
		State_word_index = (uint32_t)((RxMessage.Data[2]<<24)|(RxMessage.Data[1]<<16)|(RxMessage.Data[4])|(RxMessage.Data[5]<<8));

		//X轴电机回原点反馈动作
		if((RxMessage.StdId == 0x581)&&(RxMessage.IDE == CAN_ID_STD)&&(RxMessage.DLC == 8)&&(RxMessage.Data[0]==0x4B))
		{
			switch(State_word_index){
				case 0x6041D637:{
					LED_X_Origin_flag = 1;
					LED_active();
				};break;

				case 0x60414237:{
					LED_R_Origin_flag = 0;
					LED_active();
				};break;
				
				default:break;	
			}
		}
		
		//Z轴电机回原点反馈动作
		if((RxMessage.StdId == 0x582)&&(RxMessage.IDE == CAN_ID_STD)&&(RxMessage.DLC == 8)&&(RxMessage.Data[0]==0x4B))
		{
			switch(State_word_index){
				case 0x6041D637:{
					LED_Z_Origin_flag = 1;
					LED_active();
				};break;
				
				case 0x60414237:{
					LED_R_Origin_flag = 0;
					LED_active();
				};break;
								
				default:break;	
			}
		}
		
		//R轴电机回原点反馈动作
		if((RxMessage.StdId == 0x583)&&(RxMessage.IDE == CAN_ID_STD)&&(RxMessage.DLC == 8)&&(RxMessage.Data[0]==0x4B))
		{
			switch(State_word_index){
				case 0x6041D637:{
					LED_R_Origin_flag = 1;
					LED_active();
				};break;
				
				case 0x60414237:{
					LED_R_Origin_flag = 0;
					LED_active();
				};break;
				
				default:break;	
			}
		}
		
		
		//SH200接收动作
		if((RxMessage.StdId == 0x511)&&(RxMessage.IDE == CAN_ID_STD)&&(RxMessage.DLC == 8) && (SH200_init_flag == 0))
		{
			switch(SH200_index){
				case 0x20010101:{
					SH200_transmit_flag = 1;
					SH200_active();
				};break;
				
				case 0x20010801:{
					SH200_OUTconfig_flag = 1; 
					SH200_active();
				};break;
				
				case 0x20010202:{
					SH200_activate_flag = 1;
					SH200_active();
				};break;
				
				case 0x20110101:{
					LED_RED = !LED_RED;
					SH200_init_flag = 1;
				};
				default:break;
			}
		}
		else if((RxMessage.StdId == 0x511)&&(RxMessage.Data[1] == 0x04)){
				
		}
			
	}
}

////电机动作
//void motor_active()
//{
//	if( kinco_enable_flag == 1)
//	{
//		kinco_enable(0x601);
//		kinco_enable_flag = 0;
//	}else if(kinco_disable_flag == 1)
//	{
//		kinco_disable(0x601);
//		kinco_disable_flag = 0;
//	}else if(kinco_motor_speed_flag == 1)
//	{
//		kinco_motor_speed(0x601);
//		kinco_motor_speed_flag = 0;		
//	}else if(kinco_motor_mode_flag == 1)
//	{
//		kinco_motor_mode(0x601);
//		kinco_motor_mode_flag = 0;		
//	}
//}


////启动SH200
//void SH200_active()
//{
//	if(SH200_INconfig_flag == 1)
//	{
//		SH200_INconfig(0x411);
//		SH200_INconfig_flag = 0;
//	}else if(SH200_transmit_flag == 1)
//	{
//		SH200_transmit(0x411);
//		SH200_transmit_flag = 0;
//	}else if(SH200_OUTconfig_flag == 1)
//	{
//		SH200_OUTconfig(0x411);
//		SH200_OUTconfig_flag = 0;
//	}else if(SH200_activate_flag == 1)
//	{
//		SH200_activate(0x411);
//		SH200_activate_flag = 0;
//	}
//}

		

//	}
	
	//SH200接收中断操作
	//判断收到进入配置
//	if ((RxMessage.StdId == 0x511)&&(RxMessage.IDE == CAN_ID_STD)&&(RxMessage.Data[1] == 0x01)&&(RxMessage.Data[3] == 0x01)&&(RxMessage.Data[4] == 0x01))
//	{
////		LED_YELLOW = !LED_YELLOW;
//		SH200_transmit(0x411);
//		//判断使能主动上报
//		if ((RxMessage.StdId == 0x511)&&(RxMessage.IDE == CAN_ID_STD)&&(RxMessage.Data[3] == 0x08))
//		{
//			SH200_OUTconfig(0x411);
//			//判断退出配置
//			if ((RxMessage.StdId == 0x511)&&(RxMessage.IDE == CAN_ID_STD)&&(RxMessage.Data[3] == 0x02)&&(RxMessage.Data[4] == 0x02))
//			{
//				SH200_activate(0x411);
//				//判断激活设备
//				if ((RxMessage.StdId == 0x511)&&(RxMessage.IDE == CAN_ID_STD)&&(RxMessage.Data[1] == 0x11))
//				{
//					LED_RED = !LED_RED;
//				}
//			}
//		}
//	}




/*应答标志位动作blink*/



//int32_t test_can_tx(void)
//{
//  CanTxMsg TxMessage;
//  uint8_t mbox = 0;
//  uint32_t i = 0;
//  uint8_t data[8] = {0x55,0x01, 0x02,0x03,0x04,0x05, 0x06,0x55};

//  TxMessage.StdId = 0x100;
//  TxMessage.RTR = CAN_RTR_DATA; 
//  TxMessage.IDE = CAN_ID_STD;  
//	TxMessage.DLC = 8;  

//  memcpy(TxMessage.Data, data, 8);
//  mbox = CAN_Transmit(CAN1, &TxMessage);

//  while((CAN_TransmitStatus(CAN1, mbox) != CAN_TxStatus_Ok) && (i<0x0FFF))i++;
//  if (i >= 0xFFF)
//		return -1;
//  return 0;
//}




