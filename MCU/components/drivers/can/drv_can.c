/**
 * Copyright (C) 2019 Standard-robots, Inc
 *
 * @file 	drv_can.c
 * @author	Neethan <lijun@standard-robots.com> / <neethan@foxmail.com>
 * @version V1.0.0
 * @date 	2019年 06月 18日 星期二 10:50:47 CST
 *
 * @brief    driver can 
 */


#include <string.h>
#include <FreeRTOS.h>
#include <timers.h>
#include "bsp\include\drv_can.h"
#include "bsp\include\drv_uart.h"
#include "bsp\include\drv_hwtimer.h"
#include "srtos\err_code.h"
#include "drivers\include\sr_system_regs.h"

#define SR_CAN_TX_TASK_PRIO			5
#define SR_CAN_TX_STK_SIZE 			(512 *1) /* 以字为单位 */

static TaskHandle_t sr_can_tx_handler = NULL;
static QueueHandle_t sr_can_rx_que 	= NULL;
static QueueHandle_t sr_can_tx_que 	= NULL;
static BaseType_t xHigherPriorityTaskWoken = pdFALSE;
static SemaphoreHandle_t can_tx_semaphore 	= NULL;	


#define CAN_BAUDRATE_1M  			1000 /* 1MBps   */
#define CAN_BAUDRATE_500K  			500  /* 500kBps */
#define CAN_BAUDRATE_250K  			250 
#define CAN_BAUDRATE_125K  			125  
#define CAN_BAUDRATE_100K  			100

#define  SR_CAN1                   CAN1
#define  SR_CAN1_CLK               RCC_APB1Periph_CAN1
#define  SR_CAN1_GPIO_PORT		   GPIOA

#define  SR_CAN1_GPIO_CLK          RCC_AHB1Periph_GPIOA
#define  SR_CAN1_TX_GPIO_PORT      GPIOA   
#define  SR_CAN1_TX_GPIO_PIN       GPIO_Pin_12
#define  SR_CAN1_RX_GPIO_PORT      GPIOA
#define  SR_CAN1_RX_GPIO_PIN       GPIO_Pin_11

#define CAN_MODE_TX					0x01
#define CAN_MODE_RX					0

extern void signal_report(uint32_t sig, uint16_t reg, uint16_t regs_num);

static int32_t sr_can_init(void);
static struct can_msg  sr_can_msg;
static int32_t sr_can_deinit(void);

static struct sr_can_ops can_ops = {
	.name 						= "sr_can1",
	.init 						= sr_can_init,
	.deinit 					= sr_can_deinit,
};

static int32_t can_baud_set(CAN_TypeDef* canx, uint16_t baud)
{
	CAN_InitTypeDef        	CAN_InitStructure;
	
	CAN_DeInit(canx);
	CAN_StructInit(&CAN_InitStructure);
	
	CAN_InitStructure.CAN_TTCM=DISABLE;	 
	CAN_InitStructure.CAN_ABOM=ENABLE;	// 软件自动离线管理,上电后自动退出离线状态，继续收发数据。
	CAN_InitStructure.CAN_AWUM=DISABLE;
	CAN_InitStructure.CAN_NART=DISABLE;	 
	CAN_InitStructure.CAN_RFLM=DISABLE;	
	CAN_InitStructure.CAN_TXFP=ENABLE;
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;	

	/* CAN Baudrate = 1MBps
	 *
	 * ??? = Fpclk1/((tbs1+tbs2+1)*brp)
	 *		  = 42M /((1+7+6)*3)
	 */
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1 = CAN_BS1_7tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_6tq;

	switch (baud){
		case CAN_BAUDRATE_1M:
			CAN_InitStructure.CAN_Prescaler = 3;
			break;
		case CAN_BAUDRATE_500K:
			CAN_InitStructure.CAN_Prescaler = 6;
			break;
		case CAN_BAUDRATE_250K:
			CAN_InitStructure.CAN_Prescaler = 12;
			break;
			
		case CAN_BAUDRATE_125K:
			CAN_InitStructure.CAN_Prescaler = 24;
			break;
		case CAN_BAUDRATE_100K:
			CAN_InitStructure.CAN_Prescaler = 30;
			break;

		default:
			CAN_InitStructure.CAN_Prescaler = 6;
			break;
	}
	
	CAN_Init(SR_CAN1, &CAN_InitStructure);
	return 0;
}


static int32_t sr_can_init(void)
{
	GPIO_InitTypeDef 		GPIO_InitStructure; 
	CAN_FilterInitTypeDef  	CAN_FilterInitStructure;
	NVIC_InitTypeDef  		NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(SR_CAN1_GPIO_CLK, ENABLE);                   											 
	RCC_APB1PeriphClockCmd(SR_CAN1_CLK, ENABLE);

	GPIO_InitStructure.GPIO_Pin	= SR_CAN1_RX_GPIO_PIN | SR_CAN1_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(SR_CAN1_GPIO_PORT, &GPIO_InitStructure);

	GPIO_PinAFConfig(SR_CAN1_GPIO_PORT, GPIO_PinSource11, GPIO_AF_CAN1); 
	GPIO_PinAFConfig(SR_CAN1_GPIO_PORT, GPIO_PinSource12, GPIO_AF_CAN1); 

	can_baud_set(SR_CAN1, 500);

	CAN_FilterInitStructure.CAN_FilterNumber=0;	  
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; 
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; 
	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;
	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; 
	CAN_FilterInit(&CAN_FilterInitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;   ///< 优先级必须低于 configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	CAN_ITConfig(SR_CAN1, CAN_IT_FMP0, DISABLE);

	return 0;
}   

void drv_can_state_set(uint8_t status)
{
	if (status){
		CAN_ITConfig(SR_CAN1, CAN_IT_FMP0, ENABLE);	
	} else {
		CAN_ITConfig(SR_CAN1, CAN_IT_FMP0, DISABLE);	
	}
}

QueueHandle_t* can_que_handler_get(void)
{
	return sr_can_rx_que;
}

static int32_t sr_can_deinit(void)
{
	CAN_DeInit(SR_CAN1);
	return 0;
}

void can_tx_precfg(CanTxMsg *tx_msg, uint32_t can_id, uint8_t* data)
{
#ifdef DEVICE_STAND_ID
	tx_msg->StdId = can_id;
	tx_msg->RTR	= CAN_RTR_DATA; 
	tx_msg->IDE 	= CAN_ID_STD;  ///< 设定消息标识符的类型：标准帧
#elif defined(DEVICE_EXTEND_ID)
	tx_msg->ExtId = DEVICE_EXTEND_ID;
	tx_msg->RTR 	= CAN_RTR_DATA; 
	tx_msg->IDE 	= CAN_ID_EXT; ///< 扩展帧
#endif
	tx_msg->DLC 	= 8;
	memcpy(tx_msg->Data, data, 8);
}

void CAN1_RX0_IRQHandler(void)
{
	int32_t ret = -1;
	
	if (CAN_GetITStatus(SR_CAN1, CAN_IT_FMP0) != RESET) {

		memset(&sr_can_msg.rx_msg, 0, sizeof(sr_can_msg.rx_msg));
	    CAN_Receive(SR_CAN1, CAN_FIFO0, &sr_can_msg.rx_msg);
		
		if (sr_can_rx_que){
			ret = xQueueSendFromISR(sr_can_rx_que, (void *)&sr_can_msg.rx_msg, 
				&xHigherPriorityTaskWoken);
			if (ret != pdPASS){
				sr_kprintf("failed can to xQueueSendFromISR\r\n");
			}
			portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
		}
		CAN_ClearITPendingBit(SR_CAN1, CAN_IT_FF0);	
	}
}

static int32_t can_write(can_typedef_t *can_x, struct can_msg *msg, uint32_t timeout)
{
	int32_t ret = -1;

	ret = xQueueSend(sr_can_tx_que, &msg->tx_msg, timeout*SYSTEM_TICK_MS);
	if (ret != pdPASS) {                                      
		sr_kprintf("failed to xQueueSend, %s\r\n", __FUNCTION__);
		return -1;
	}
	return 0;
}


//#define	GET_BIT(x, bit)	((x & (1 << bit)) >> bit)	/* 获取第bit位 */
//#define	CLEAR_BIT(x, bit)	(x &= ~(1 << bit))	/* 清零第bit位 */
//#define	SET_BIT(x, bit)	(x |= (1 << bit))			/* 置位第bit位 */

static int32_t sr_can_tx(void *pvParameters)
{
	CanTxMsg tx_msg;
	uint8_t tx_state = 0;
	volatile uint16_t cnt = 0;
	
	can_tx_semaphore = xSemaphoreCreateMutex();
	if (!can_tx_semaphore){
		sr_kprintf("error, %s, can_tx_semaphore\r\n", __FUNCTION__);
	}
	
	while (1) {
		
		cnt = 3;
		memset(&tx_msg, 0, sizeof(tx_msg));
		if (xQueueReceive(sr_can_tx_que, &tx_msg, 0) == pdTRUE) {		
			tx_state = CAN_Transmit(SR_CAN1, &tx_msg);
			while((CAN_TransmitStatus(SR_CAN1, tx_state) != CAN_TxStatus_Ok) && cnt--){
				if (0 == cnt){ 
					// TODO
					// 需要建立轻量级消息通知机制，底层只上报异常状态，不做逻辑处理!!
					// CAN发送超时，写入故障码到寄存器
					// 无错误码时，也要同步刷新设备状态 
					// 特殊情况：当电机正常工作时，CAN总线突然断开，要人为强制构造错误码
					// 并写入设备错误码寄存器，便于SROS及时更新状态显示

					//signal_report(SYS_FAULT_CODE_CAN_TX_TIMEOUT_BIT7, REG_SYS_FAULT_CODE, 1);

					//sr_kprintf("error, CAN bus transmit status timeout\r\n");
					sr_log2sros("error, CAN bus transmit status timeout\n");
					break;
				}
				vTaskDelay(1); // CAN发送数据必须有一定时间间隔，否则总线将会拥堵
			}
		}
		taskYIELD(); // request a context switch to another task
	}
}


/*
 * 通过can接口发送数据
 *
 * @param[in]	None
 * @param[out]	None
 *
 * @return		>=0成功，其他失败
 */
int32_t sr_can_write(can_typedef_t *can_x, struct can_msg *msg, uint16_t timeout)
{
	int32_t ret = -1;
	
	if (NULL == msg ||  can_x != SR_CAN1) return -1;
	
	ret = can_write(can_x, msg, timeout);
	if (ret == E_OK){
		return ret;
	}
	return ERROR;
}

static int32_t register_can(struct sr_can_ops *ops)
{
	int32_t ret = -1;

	ret = ops->init();
	if (ret < 0 ){
		sr_kprintf("failed to can init, ret = %d\n", ret);
		return ERROR;
	}

    sr_can_rx_que = xQueueCreate(80, sizeof(CanRxMsg));
    if (!sr_can_rx_que) {
		sr_kprintf("failed can_rx to xQueueCreate, %s\r\n", __FUNCTION__);
		return ERROR;
    }
	
	sr_can_tx_que = xQueueCreate(50, sizeof(CanTxMsg));
    if (!sr_can_tx_que) {
		sr_kprintf("failed can_tx to xQueueCreate, %s\r\n", __FUNCTION__);
		return ERROR;
    }
	ret = xTaskCreate((TaskFunction_t)sr_can_tx, "sr_can_tx", SR_CAN_TX_STK_SIZE, NULL, 
						SR_CAN_TX_TASK_PRIO, &sr_can_tx_handler);
	if (pdPASS != ret) {
		sr_kprintf("failed sr_can_tx xTaskCreate, %s\r\n", __FUNCTION__);                                   //创建发送任务失败
	}	
	return 0;
}

int32_t register_bsp_can(void)
{
	return register_can(&can_ops);
}


int32_t test_can_tx(void)
{
	CanTxMsg TxMessage;
	uint8_t mbox = 0;
	uint32_t i = 0;
	uint8_t data[8] = {0x55,0xaa, 0xab,0x55,0x55,0xaa, 0xab,0x55};

#ifdef DEVICE_STAND_ID
	TxMessage.StdId = 0x301;
	TxMessage.RTR = CAN_RTR_DATA; 
	TxMessage.IDE = CAN_ID_STD;  
#elif defined(DEVICE_EXTEND_ID)
	TxMessage.ExtId = DEVICE_EXTEND_ID;
	TxMessage.RTR = CAN_RTR_DATA; 
	TxMessage.IDE = CAN_ID_EXT; 
#endif

	TxMessage.DLC = 8;	
	memcpy(TxMessage.Data, data, 8);
	
	mbox = CAN_Transmit(SR_CAN1, &TxMessage);
	while((CAN_TransmitStatus(SR_CAN1, mbox) != CAN_TxStatus_Ok) && (++i<0x0FFF))
	if (i >= 0xFFF)
		return -1;

	return 0;
}

