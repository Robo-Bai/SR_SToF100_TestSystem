#ifndef __DRV_UART_H__
#define __DRV_UART_H__

#include <stdio.h>	
#include "core\include\sr_system.h"	
#include "core\include\srtos.h"


#define BAUD_RATE_2400                  2400
#define BAUD_RATE_4800                  4800
#define BAUD_RATE_9600                  9600
#define BAUD_RATE_19200                 19200
#define BAUD_RATE_38400                 38400
#define BAUD_RATE_57600                 57600
#define BAUD_RATE_115200                115200
#define BAUD_RATE_230400                230400
#define BAUD_RATE_460800                460800
#define BAUD_RATE_921600                921600
#define BAUD_RATE_2000000               2000000
#define BAUD_RATE_3000000               3000000


	
#define USART_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART1_RX 			0		//使能（1）/禁止（0）串口1接收
	  	
extern uint8_t  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern uint16_t USART_RX_STA;         		//接收状态标记	
extern UART_HandleTypeDef UART1_Handler; //UART句柄

#define RXBUFFERSIZE   1 //缓存大小
extern uint8_t aRxBuffer[RXBUFFERSIZE];//HAL库USART接收Buffer
    	

void usart_debug_init(uint32_t bound);
void usart_comm_init(uint32_t bound);

int32_t uart_sync(const uint8_t *data, uint16_t len, uint16_t timeout_ms);


#endif /* __DRV_UART_H__ */

