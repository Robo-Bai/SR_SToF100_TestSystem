/**
 * Copyright (C) 2021 Standard-robots, Inc
 *
 * @file 	serial.c
 * @author	Neethan <lijun@standard-robots.com> / <neethan@foxmail.com>
 * @version V1.0.0
 * @date 	2021年 03月 31日 星期三 10:50:47 CST
 *
 * @brief    serial device driver.
 */

#include <string.h>
#include <stdlib.h>
#include "drivers\include\drivers\drv_hwtimer.h"
#include "core\include\err_code.h"
#include "drivers\include\drivers\sr_serial.h"


int32_t comm_read(uint8_t *data, uint8_t len, uint32_t timeout);
int32_t comm_write(const uint8_t *data, uint8_t len);
int32_t comm_remove(struct serial_operations *ops);
uint32_t comm_fifo_len(struct kfifo *pkfifo);


enum __serial_frame_stage {
	FRAME_READY = 0x01,
	FRAME_LEN,
	FRAME_PAYLOAD,
	FRAME_CHECKSUM,
	FRAME_EOF,
};


static struct serial_operations serial_ops[] = {
	{
		.name 				= "com1",
		.usartx				= USART1,
		.bound				= BAUD_RATE_460800,
		.rx_buf				= NULL,
		.rx_pkfifo 			= NULL,
	},
	{
		.name 				= "com2",
		.usartx				= USART2,
		.bound				= BAUD_RATE_115200,
		.rx_buf				= NULL,
		.rx_pkfifo 			= NULL,
	},
	{
		.name 				= "com3",
		.usartx				= USART3,
		.bound				= BAUD_RATE_460800,
		.rx_buf				= NULL,
		.rx_pkfifo 			= NULL,
		.read				= comm_read,
		.write				= comm_write,
		.remove				= comm_remove,
		.fifo_len			= comm_fifo_len,
	},
	
};

int32_t comm_read(uint8_t *data, uint8_t len, uint32_t timeout)
{
	if (!data) return 0;
	
	return kfifo_out(serial_ops[COM3].rx_pkfifo , data, len);
}

int32_t comm_write(const uint8_t *data, uint8_t len)
{
	return 0;
}

int32_t comm_remove(struct serial_operations *ops)
{
	if (ops->rx_buf){
		free(ops->rx_buf);
		ops->rx_buf = NULL;
	}
	if (ops->rx_pkfifo){
		free(ops->rx_pkfifo);
		ops->rx_pkfifo = NULL;
	}	
	return 0;
}

uint32_t comm_fifo_len(struct kfifo *pkfifo)
{
	return kfifo_len(pkfifo);
}

uint32_t sr_serial_fifo_len(void)
{
	return kfifo_len(serial_ops[COM3].rx_pkfifo);
}


/**
 * 通过串口发送数据,放到发送缓冲区后立即返回，由中断服务程序在后台完成发送
 *
 * @param[in]	None
 * @param[out]	None
 *
 * @return		0成功，其他失败
 */
int32_t sr_serial_write(enum sr_com com, uint8_t *data, uint8_t len, uint32_t timeout)
{
	if (NULL == data) return E_ERROR;
		
	return 0;
}


int32_t sr_serial_read(enum sr_com com, uint8_t *data, uint32_t len, uint32_t timeout)
{
	return comm_read(data, len, timeout);
}

/**
 * 清除串口发送/接收缓冲区
 *
 * @param[in]	None
 * @param[out]	None
 *
 * @return		0成功，其他失败
 */
//int32_t sr_serial_fifo_clear(struct sr_serial_comm *serial, enum serial_mode mode)
//{
//	if (!serial) return E_ERROR;
//	
//	return E_ERROR;
//}


/*
 * This function initializes serial device.
 */
static int32_t serial_init(struct serial_operations *ops)
{
	
	if (strcmp(ops->name, "com1") == 0){
		usart_debug_init(ops->bound); 
	}

	if (strcmp(ops->name, "com3") == 0){	
		usart_comm_init(ops->bound);
		if (!ops->rx_buf){
			if ((ops->rx_buf = (uint8_t*)malloc(RX_BUF_MAX)) == NULL){
				sr_debug("error\r\n");
				return ERROR;
			}
		}
		if (!ops->rx_pkfifo){
			if ((ops->rx_pkfifo = (struct kfifo*)malloc(sizeof(struct kfifo))) == NULL){
				sr_debug("error\r\n");
				return ERROR;
			}
		}
		kfifo_init(ops->rx_pkfifo, ops->rx_buf, RX_BUF_MAX);
	}

    return 0;
}

#if 0
uint16_t ser1_read_data(uint8_t* payload)
{
	uint16_t len = 0;
	
	if (sros_serial_comm.Rx_payload_buf_r != sros_serial_comm.Rx_payload_buf_w) {

		memcpy(payload, sros_serial_comm.Rx_payload_buf[sros_serial_comm.Rx_payload_buf_r], 
				sros_serial_comm.Rx_payload_len[sros_serial_comm.Rx_payload_buf_r]);

		len = sros_serial_comm.Rx_payload_len[sros_serial_comm.Rx_payload_buf_r];
		sros_serial_comm.Rx_payload_buf_r = (sros_serial_comm.Rx_payload_buf_r + 1) % SER1_PAYLOAD_BUF_LEN;

		//printf("Rx_r:%d, Rx_w:%d\r\n", sros_serial_comm.Rx_payload_buf_r, sros_serial_comm.Rx_payload_buf_w);
		return len;
	} else {
		return 0;
	}
}

void ser1_frame_feed(uint8_t temp) 
{
	//UART4->DR = (temp & (uint16_t)0x01FF);
	
	switch (sros_serial_comm.Rx_state){
		case FRAME_READY:
		{
			if(temp == 0xAA){
				//tick_test[0] = xTaskGetTickCount();
				if ((sros_serial_comm.Rx_payload_buf_w + 1) % SER1_PAYLOAD_BUF_LEN 
					== sros_serial_comm.Rx_payload_buf_r) {
					// 接收缓冲区满，不接受新帧
					sr_kprintf("error, Rx_payload_buf full\r\n");
				} else {
					sros_serial_comm.Rx_state = FRAME_LEN;
				}
			}
		} break;
			
		case FRAME_LEN:
		{
			if (temp < RS232_MAX_LENGTH-1){

				sros_serial_comm.Rx_payload_len[sros_serial_comm.Rx_payload_buf_w] = temp;
				sros_serial_comm.Rx_payload_recv = 0;
				sros_serial_comm.Rx_checksum = 0;

				sros_serial_comm.Rx_state = FRAME_PAYLOAD;
			} else {
				sros_serial_comm.Rx_state = FRAME_READY;
			}
		} break;
		
		case FRAME_PAYLOAD:
		{
			sros_serial_comm.Rx_payload_buf[sros_serial_comm.Rx_payload_buf_w][sros_serial_comm.Rx_payload_recv] = temp;

			sros_serial_comm.Rx_checksum ^= temp; // XOR-checksum
			sros_serial_comm.Rx_payload_recv++;
			
			if(sros_serial_comm.Rx_payload_recv == 
				sros_serial_comm.Rx_payload_len[sros_serial_comm.Rx_payload_buf_w]){
				sros_serial_comm.Rx_state = FRAME_CHECKSUM;
			}
		} break;
		
		case FRAME_CHECKSUM:
		{
			if (sros_serial_comm.Rx_checksum == temp) {
				sros_serial_comm.Rx_state = FRAME_EOF;
			} else {
				// frame checksum failed! ignore this frame
				sros_serial_comm.Rx_state = FRAME_READY;
				sr_kprintf("error, checksum failed\n");
			}
		} break;
		
		case FRAME_EOF:
		{
			if (temp == 0x55) {
				//tick_test[1] = xTaskGetTickCount();
				// 移动write指针到下一个block
				sros_serial_comm.Rx_payload_buf_w = 
				(sros_serial_comm.Rx_payload_buf_w + 1) % SER1_PAYLOAD_BUF_LEN;
				sros_serial_comm.Rx_state = FRAME_READY;
			}
		} break;
		
		default:
		{
			sros_serial_comm.Rx_state = FRAME_READY;
		} break;
	}	
}
#endif

uint32_t sr_serial_irq_handler(uint8_t* ch, uint32_t len)
{
	if (kfifo_is_full(serial_ops[COM3].rx_pkfifo)){ // 接收缓冲区满，不接受新帧
		sr_kprintf("kfifo full\n");
	} else {
		return kfifo_in(serial_ops[COM3].rx_pkfifo, ch, len);
	}
	return 0; 
}

void test_connet(void)
{
	uint8_t data[] = {0x07,0x11, 0x00, 0x30, 0x07, 0x00, 0x21, 0x00, 0x00, 0x00, 0xa1, 0x00, 0x00,
		0x00, 0xf0, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x0c, 0x00,
		0x00, 0x00, 0x0d, 0x00, 0x00, 0x00};
		
	sr_msg_package(data, sizeof(data));
}

void system_info_get(void)
{
	uint8_t data[] = {0x07, 0x10, 0x00, 0x20, 0x11, 0x00};
		
	sr_msg_package(data, sizeof(data));
}


int32_t sr_msg_package(uint8_t *msg, uint16_t len)
{
	static uint8_t buf[256] = {0};
	uint16_t index = 0;
	uint16_t i = 0;
	uint8_t xor_checksum = 0;
	
	if (len <= 256-4){
		memset(buf, 0, sizeof(buf));
		buf[index++] = FRAME_HEAD;
		buf[index++] = len;
		memcpy(&buf[index], msg, len);
		for(i=0; i<len; i++){
			xor_checksum ^= msg[i];
		}
		index += len;
		buf[index++] = xor_checksum;
		buf[index++] = FRAME_REAR;
		//sr_data_print("snd_ack", (const uint8_t *)buf, index);
		return uart_sync(buf, index, 6);
	} 
	return -1;
}

int32_t sr_serial_register(void)
{
	int32_t ret = -1;
	uint8_t i = 0;

	for (i=0; i<sizeof(serial_ops)/sizeof(struct serial_operations); i++){
		if ((ret = serial_init(&serial_ops[i])) < 0){
			sr_kprintf("error, serial_init %s\r\n", serial_ops[i].name);
		}
	}
	
	return ret;
}

void print_check(bool rst)
{
	static uint32_t i = 0;
	static uint32_t last_timestamp = 0;
	
	(rst == true ? (i = 0) : 0);
	(last_timestamp <= 0 ? (last_timestamp = system_time_tick()) : 0);
	if (get_delta_time(last_timestamp, system_time_tick()) > 1000){ // 1s
		((++i%(32+1) != 0) ? (sr_kprintf("#")) : (sr_kprintf("\r\n")));
		last_timestamp = system_time_tick();
	}
}
