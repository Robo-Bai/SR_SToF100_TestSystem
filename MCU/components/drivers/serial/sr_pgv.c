/*
 * Copyright (C) 2019 Standard-robots, Inc
 *
 * file 	pgv.c
 * author	Neethan <lijun@standard-robots.com> / <neethan@foxmail.com>
 * date 	2019年 06月 19日 星期三 10:50:47 CST
 *
 * brief    pgv device driver 
 */
 
#include "sr_pgv.h"
#include "bsp\include\drv_rs485.h"



int32_t sr_pgv_init(uint8_t no)
{
	uint8_t cmd[8] = {0};
	uint8_t index = 0;
	
	switch (no){
		case SR_PGV_NO1:
			cmd[index++] = 0xE8;
			cmd[index++] = 0x17;
			break;
		case SR_PGV_NO2:
			cmd[index++] = 0xE9;
			cmd[index++] = 0x16;
			break;

		default:
			break;
	}
	return rs485_1_WriteData(cmd, index);
}


