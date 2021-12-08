/*
 * Copyright (C) 2016-2021 Standard-robots, Inc
 *
 * file 	app.c
 * author	Neethan <lijun@standard-robots.com> / <neethan@foxmail.com>
 * date 	2021年 04月 01日 星期五 10:50:47 CST
 *
 * app 
 */
 

#include "drivers\include\board.h"
#include "drivers\include\drivers\drv_hwtimer.h"
#include "drivers\include\drivers\sr_serial.h"
#include "core\include\sr_system.h"
#include "core\include\events.h"
#include "..\app\include\app.h"
#include "..\app\include\ev_process.h"
#include "..\app\include\unittest_manager.h"

#include "Basic.h"

static uint8_t g_buf[FRAME_LEN_MAX] = {0};
static uint32_t g_tick_last = 0;
static struct unittest_operations *g_utest_ops;

	

// TODO, 必须增加异常帧处理

//aa 22 07 11 00 30 07 00 21 00 00 00 a1 00 00 00 f0 00 00 00 0a 00 00 00 0b 00 00 00 0c 00 00 00 0d 00 00 00 51 55
//帧头：aa 
//数据长度：22 
//数据：07 11 00 30 07 00 21 00 00 00 a1 00 00 00 f0 00 00 00 0a 00 00 00 0b 00 00 00 0c 00 00 00 0d 00 00 00
//数据异或值：51 
//帧尾：55

// 异常帧：
//aa 16 01 21 00 00 00 00 00 00 00 00 00 00 00 00 00 aa 22 07 11 00 30 07 00 21 00 00 00 a1 00 00 00 f0 00 00 00 0a 00 00 00 0b 00 00 00 0c 00 00 00 0d 00 00 00 51 55

// 处理机制：
// 1.从接收此帧数据头开始丢弃，并且一次仅丢弃一个字节；
// 2.丢弃一个字节后，再次按照正常帧的逻辑来处理数据

int32_t serial_data_handle(void)
{
	int32_t ret = -1;
	uint8_t len = 0;
	uint8_t checksum = 0;
	uint8_t i = 0;
	uint8_t frame_head =  0;

	// 获取当前测试单元句柄
	g_utest_ops = get_utest_handler();

	while (sr_serial_fifo_len() > 4){
		sr_serial_read(COM3,  &frame_head, 1, 1); // 1.取出帧头
		if (FRAME_HEAD == frame_head){
			sr_serial_read(COM3,  &len, 1, 1); // 2.取出帧（数据）长度

			if (len > 0){
				memset(g_buf, 0, FRAME_LEN_MAX);
				while (1){
					if (sr_serial_fifo_len() >= len+2){
						sr_serial_read(COM3, g_buf, len+2, 0); // 4.读取数据包含xor、tail
						checksum = 0;
						for (i=0; i<len; i++){
							checksum ^= g_buf[i];
						}
						if ((checksum == g_buf[len]) && (FRAME_REAR == g_buf[len+1])){
							// TODO
							// 处理测试结果，是否通过
							// 解析接收到的数据事件（或寄存器）与当前测试单元寄存器比对
							if (g_utest_ops && g_utest_ops->ut_map->handler_callback){
								g_utest_ops->ut_map->handler_callback(g_utest_ops, g_buf, len);
							}
							//sr_data_print("rcv msg", g_buf, len); // 太耗时，减少打印!!				
							if (get_delta_time(g_tick_last, system_time_tick()) >= 100){
								sr_bsp_dev_toggle(SR_LED_CMD);
								g_tick_last = system_time_tick();
							}
							return 0;
						} else {
							// TODO
							// 接受到不完整帧特殊处理
							sr_kprintf("error, rcv:%X %X ", frame_head, len);
							sr_data_print("msg", g_buf, len+2);
							sr_kprintf("calculate checksum: 0x%x\r\n", checksum);
							return -1;
						}
					} else {
						delay_ms(2);
					}
				}
			}
		}
	}
	return ret;
}


int32_t sr_app_init(void)
{
	int32_t ret = 0;
	
	ret = sr_event_init();
	ret = sr_utest_init();

	sr_kprintf("\nTest cases start...\n");
	
	return ret;
}
