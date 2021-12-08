/**
 * Copyright (C) 2016-2021 Standard-robots, Inc
 *
 * @file 	drv_uart.c
 * @author	Neethan <lijun@standard-robots.com> / <neethan@foxmail.com>
 * @version V1.0.0
 * @date 	2021年 03月 29日 星期一 10:50:47 CST
 *
 * @brief   driver uart 
 */

#include "core\include\sr_system.h"	
#include "drivers\include\drivers\drv_uart.h"



//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
//#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)	
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
	USART1->DR = (uint8_t) ch;      
	return ch;
}
#endif 



static uint8_t commRxBuffer;
static UART_HandleTypeDef huart3; //UART句柄
static DMA_HandleTypeDef hdma_usart3_tx;

//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
uint8_t USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
uint16_t USART_RX_STA = 0;       //接收状态标记	
uint8_t aRxBuffer[RXBUFFERSIZE];//HAL库使用的串口接收缓冲
UART_HandleTypeDef UART1_Handler; //UART句柄



//初始化IO 串口1 
//bound:波特率
void usart_debug_init(uint32_t bound)
{	
	UART1_Handler.Instance=USART1;					    //USART1
	UART1_Handler.Init.BaudRate=bound;				    //波特率
	UART1_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //字长为8位数据格式
	UART1_Handler.Init.StopBits=UART_STOPBITS_1;	    //一个停止位
	UART1_Handler.Init.Parity=UART_PARITY_NONE;		    //无奇偶校验位
	UART1_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //无硬件流控
	UART1_Handler.Init.Mode=UART_MODE_TX_RX;		    //收发模式
	HAL_UART_Init(&UART1_Handler);					    //HAL_UART_Init()会使能UART1
	
	//该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
	HAL_UART_Receive_IT(&UART1_Handler, (uint8_t *)aRxBuffer, RXBUFFERSIZE);
}

void usart_comm_init(uint32_t bound)
{	
	huart3.Instance = USART3;					    //USART3
	huart3.Init.BaudRate = bound;				    //波特率
	huart3.Init.WordLength = UART_WORDLENGTH_8B;   //字长为8位数据格式
	huart3.Init.StopBits = UART_STOPBITS_1;	    //一个停止位
	huart3.Init.Parity = UART_PARITY_NONE;		    //无奇偶校验位
	huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;   //无硬件流控
	huart3.Init.Mode = UART_MODE_TX_RX;		    //收发模式
	HAL_UART_Init(&huart3);					    //HAL_UART_Init()会使能UART3
	
	//该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
	HAL_UART_Receive_IT(&huart3, &commRxBuffer, 1);
}

//UART底层初始化，时钟使能，引脚配置，中断配置
//此函数会被HAL_UART_Init()调用
//huart:串口句柄
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    //GPIO端口设置
	GPIO_InitTypeDef GPIO_Initure;
	
	if (huart->Instance == USART1) {
		
		__HAL_RCC_GPIOA_CLK_ENABLE();			//使能GPIOA时钟
		__HAL_RCC_USART1_CLK_ENABLE();			//使能USART1时钟
	
		GPIO_Initure.Pin=GPIO_PIN_9 | GPIO_PIN_10;		
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//复用推挽输出
		GPIO_Initure.Pull=GPIO_PULLUP;			//上拉
		GPIO_Initure.Speed=GPIO_SPEED_FAST;		//高速
		GPIO_Initure.Alternate=GPIO_AF7_USART1;	//复用为USART1
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//初始化PA9
		
		HAL_NVIC_EnableIRQ(USART1_IRQn);				//使能USART1中断通道
		HAL_NVIC_SetPriority(USART1_IRQn,6,0);			
	}
	else if (huart->Instance == USART3){
		__HAL_RCC_GPIOB_CLK_ENABLE();			//使能GPIOB时钟
		__HAL_RCC_USART3_CLK_ENABLE();			//使能USART3时钟
	
		GPIO_Initure.Pin=GPIO_PIN_10 | GPIO_PIN_11;	
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//复用推挽输出
		GPIO_Initure.Pull=GPIO_PULLUP;			//上拉
		GPIO_Initure.Speed=GPIO_SPEED_FAST;		//高速
		GPIO_Initure.Alternate=GPIO_AF7_USART3;	//复用为USART3
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);	   	//初始化PB10
		
		/* USART3 DMA Init */
		__HAL_RCC_DMA1_CLK_ENABLE();
		
		/* USART3_TX Init */
		hdma_usart3_tx.Instance = DMA1_Stream3;
		hdma_usart3_tx.Init.Channel = DMA_CHANNEL_4;
		hdma_usart3_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
		hdma_usart3_tx.Init.PeriphInc = DMA_PINC_DISABLE;
		hdma_usart3_tx.Init.MemInc = DMA_MINC_ENABLE;
		hdma_usart3_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		hdma_usart3_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
		hdma_usart3_tx.Init.Mode = DMA_NORMAL;
		hdma_usart3_tx.Init.Priority = DMA_PRIORITY_HIGH;
		hdma_usart3_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
		hdma_usart3_tx.Init.MemBurst=DMA_MBURST_SINGLE;                 // 存储器突发单次传输
		hdma_usart3_tx.Init.PeriphBurst=DMA_PBURST_SINGLE;              
		if (HAL_DMA_Init(&hdma_usart3_tx) != HAL_OK){
			Error_Handler();
		}
		__HAL_LINKDMA(huart,hdmatx,hdma_usart3_tx);
	
		/* DMA interrupt init */
		/* DMA1_Stream3_IRQn interrupt configuration */
		HAL_NVIC_SetPriority(DMA1_Stream3_IRQn, 1, 0);
		HAL_NVIC_EnableIRQ(DMA1_Stream3_IRQn);
		
		HAL_NVIC_EnableIRQ(USART3_IRQn);				//使能USART3中断通道
		HAL_NVIC_SetPriority(USART3_IRQn, 1, 1);			//抢占优先级1，子优先级0	
	}
	else {
	
	}
}

#if 0
int32_t uart_sync(const uint8_t *data, uint16_t len, uint16_t timeout_ms)
{
	HAL_StatusTypeDef status;
	
	sr_data_print("uart_sync", data, len);
	
	status = HAL_UART_Transmit(&huart3, (uint8_t*)data, len, timeout_ms);	//发送接收到的数据
	while(__HAL_UART_GET_FLAG(&huart3, UART_FLAG_TC) != SET){		//等待发送结束
	}
	
	return len;
}
#else

int32_t uart_sync(const uint8_t *data, uint16_t len, uint16_t timeout_ms)
{
	HAL_StatusTypeDef status;
	uint32_t tick = 0;
	
	tick = system_time_tick();
	while (get_delta_time(tick, system_time_tick()) <= timeout_ms){
		if ((status = HAL_UART_Transmit_DMA(&huart3, (uint8_t *)data, len)) == HAL_OK) {
			return len;
		}
	}
	sr_kprintf("error, uart_sync timeout, status %d\r\n", status);
	return -1;
}
#endif

/**
  * @brief This function handles DMA1 stream3 global interrupt.
  */
void DMA1_Stream3_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_usart3_tx);
}


// DMA_FLAG_TCIF0_4就是指DMA的Stream0或Stream4， 
// DMA_FLAG_TCIF1_5就是指DMA的Stream1或Stream5， 
// DMA_FLAG_TCIF2_6就是指DMA的Stream2或Stream6， 
// DMA_FLAG_TCIF3_7就是指DMA的Stream3或Stream7
//if(__HAL_DMA_GET_FLAG(&hdma_usart3_tx,DMA_FLAG_TCIF3_7)){
//	__HAL_DMA_CLEAR_FLAG(&hdma_usart3_tx,DMA_FLAG_TCIF3_7);
//	HAL_UART_DMAStop(&huart3);     
//}

//extern bool tx_flag;
extern uint32_t sr_serial_irq_handler(uint8_t* ch, uint32_t len);


void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART3){
//		tx_flag = true;
		return;
	}	
}	

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART3){
		sr_serial_irq_handler(&commRxBuffer, 1);
		return;
	}
		
	if(huart->Instance == USART1){
		return;
	}
}

void USART3_IRQHandler(void)                	
{ 
	HAL_UART_IRQHandler(&huart3);
	HAL_UART_Receive_IT(&huart3, &commRxBuffer, 1);	
} 

void USART1_IRQHandler(void)                	
{ 
	uint32_t timeout=0;
	
	HAL_UART_IRQHandler(&UART1_Handler);	//调用HAL库中断处理公用函数
	
	timeout=0;
    while (HAL_UART_GetState(&UART1_Handler) != HAL_UART_STATE_READY)//等待就绪
	{
		timeout++;////超时处理
		if(timeout>HAL_MAX_DELAY) break;		
	
	}
     
	timeout=0;
	while(HAL_UART_Receive_IT(&UART1_Handler, (uint8_t *)aRxBuffer, RXBUFFERSIZE) != HAL_OK)//一次处理完成之后，重新开启中断并设置RxXferCount为1
	{
		timeout++; //超时处理
		if(timeout>HAL_MAX_DELAY) break;	
	}
} 


/*下面代码我们直接把中断控制逻辑写在中断服务函数内部。
 说明：采用HAL库处理逻辑，效率不高。*/
/*

//串口1中断服务程序
void USART1_IRQHandler(void)                	
{ 
	uint8_t Res;

	if((__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_RXNE)!=RESET))  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
        HAL_UART_Receive(&UART1_Handler,&Res,1,1000); 
		if((USART_RX_STA&0x8000)==0)//接收未完成
		{
			if(USART_RX_STA&0x4000)//接收到了0x0d
			{
				if(Res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
				else USART_RX_STA|=0x8000;	//接收完成了 
			}
			else //还没收到0X0D
			{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
				}		 
			}
		}   		 
	}
	HAL_UART_IRQHandler(&UART1_Handler);	
} 
#endif	
*/


