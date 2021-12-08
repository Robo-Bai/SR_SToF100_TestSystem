/**
 * Copyright (C) 2016-2021 Standard-robots, Inc
 *
 * @file 	main.c
 * @author	Neethan <lijun@standard-robots.com> / <neethan@foxmail.com>
 * @version V1.0.0
 * @date 	2021年 03月 29日 星期一 10:50:47 CST
 *
 * @brief	main 
 */
 
#include "core\include\sr_system.h"
#include "drivers\include\board.h"
#include "drivers\include\drivers\sr_serial.h"

#include "..\app\include\ev_process.h"
#include "..\app\include\app.h"


bool tx_flag = true;

int32_t utest_handle(void)
{
	int32_t ret = 0;
	
	while (1){
		
		//HAL_Delay(100);
		if (tx_flag){
			//request_cmd();
			//system_info_get();
			tx_flag = false;
			ret = utest_item_dispatch();
		}
		ev_handle();
		serial_data_handle();
	}
}

int main(void)
{
	sr_startup();
	sr_app_init();
	sr_start_scheduler();

	HAL_Delay(1000);
	
    while(1) {

		utest_handle();
    }
}

