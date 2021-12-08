#ifndef __SR_SERIASL_H__
#define __SR_SERIASL_H__



#include "core\include\srtos.h"
#include "stm32f4xx.h"
#include "drivers\include\drivers\drv_uart.h"
#include "core\include\sr_kfifo.h"


#define FRAME_HEAD					0xAA	
#define FRAME_REAR					0x55
#define FRAME_LEN_MAX				256

#define ENCODE_FIELD(value) \
             do { memcpy(&send_buff[send_offset], &value, sizeof(value));\
							send_offset += sizeof(value);} while(0)

#define ENCODE_FIELD_LEN(value, len) \
             do { memcpy(&send_buff[send_offset], &value, len);\
							send_offset += len; } while(0)

enum sr_com {
	COM1 = 0,	/* USART1  PA9, PA10 */
	COM2 = 1,	/* USART2, PD5,PD6 æˆ– PA2, PA3 */
	COM3 = 2,	/* USART3, PB10, PB11 */
	COM4 = 3,	/* UART4, PC10, PC11 */
	COM5 = 4,	/* UART5, PC12, PD2 */
} ;

enum serial_mode {
	SERIAL_TX = 0,
	SERIAL_RX = 1,
};


#define PAYLOAD_BUF_LEN 	 10


struct serial_operations {
#define RX_BUF_MAX 			4096
	const char *name;
	USART_TypeDef *usartx;
	uint32_t bound;
	struct kfifo *rx_pkfifo;
	uint8_t* rx_buf;
	
	uint8_t tx_state; 
	uint8_t tx_len[PAYLOAD_BUF_LEN];
	uint8_t tx_cnt; // 当前发送的字节个数
	uint8_t	*tx_buf;
	uint8_t tx_buf_r; // 需要读取的block
	uint8_t	tx_buf_w; // 正在写入的block

	int32_t (*init)(USART_TypeDef * uart_x, const char *name);
	int32_t (*read)(uint8_t *data, uint8_t len, uint32_t timeout);
	int32_t (*write)(const uint8_t *data, uint8_t len);
	int32_t (*remove)(struct serial_operations *ops);
	uint32_t (*fifo_len)(struct kfifo *pkfifo);

};


int32_t sr_serial_register(void);


/**
 * 通过串口发送数据,放到发送缓冲区后立即返回，由中断服务程序在后台完成发送
 *
 * @param[in]	None
 * @param[out]	None
 *
 * @return		0成功，其他失败
 */
int32_t sr_serial_write(enum sr_com com, uint8_t *data, uint8_t len, uint32_t timeout);

/**
 * 从接收缓冲区读取数据，非阻塞
 *
 * @param[in]	None
 * @param[out]	None
 *
 * @return		>0成功返回接收到的数据长度，其他失败
 */
int32_t sr_serial_read(enum sr_com com, uint8_t *data, uint32_t len, uint32_t timeout);

uint32_t sr_serial_fifo_len(void);

int32_t sr_msg_package(uint8_t *msg, uint16_t len);

void system_info_get(void);
void print_check(bool rst);

#endif /* __SR_SERIASL_H__ */
