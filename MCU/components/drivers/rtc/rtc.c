/**
 * Copyright (C) 2019 Standard-robots, Inc
 *
 * @file 	rtc.c
 * @author	Neethan <lijun@standard-robots.com> / <neethan@foxmail.com>
 * @version V1.0.0
 * @date 	2020年 2月 16日 星期日 14:50:47 CST
 *
 * @brief    rtc
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "srtos\srtos.h"
#include "drivers\include\drivers\rtc.h"


#define FORMAT_TIME_MAX_LEN			19
#define TO_FORMAT_TIME_MAX_LEN		24

/**
 * 本地时间戳转化为格林威治标准时间,
 * 格林威治标准时间比北京时间晚8个小时
 *
 * @param[in]	timestamp:时间戳,例"1581844348"
 * @param[out]	format_time：标准格式时间，例"2020-02-16 17:12:28"
 *
 * @return		0表示成功,其它返回失败
 */
int32_t sr_timestamp2time(time_t timestamp, char *format_time)
{
	struct tm *gm_date;
	
	timestamp += 28800; // 8h时差 = 8*60*60 = 28800
	gm_date = localtime(&timestamp);
	strftime((char *)format_time, 24, "%Y-%m-%d %H:%M:%S", gm_date);
	sr_kprintf("nowtime = %s\n", format_time);
	
	return 0;
}


static long get_tick(char *str_time)
{
	struct tm stm;
	int32_t iY, iM, iD, iH, iMin, iS;

	memset(&stm,0,sizeof(stm));
	iY = atoi(str_time);
	iM = atoi(str_time+5);
	iD = atoi(str_time+8);
	iH = atoi(str_time+11);
	iMin = atoi(str_time+14);
	iS = atoi(str_time+17);

	stm.tm_year = iY-1900;
	stm.tm_mon = iM-1;
	stm.tm_mday = iD;
	stm.tm_hour = iH;
	stm.tm_min = iMin;
	stm.tm_sec = iS;

	//sr_kprintf("%d-%0d-%0d %0d:%0d:%0d\n", iY, iM, iD, iH, iMin, iS);
	return mktime(&stm);
}
 
/**
 * 本地时间时间转化为格林威治标准时间戳
 *
 * @param[in]	timestamp:时间戳,例"1581844348"
 * @param[out]	format_time：标准格式时间，例"2020-02-16 17:12:28"
 *
 * @return		0表示成功,其它返回失败
 */
int32_t sr_time2timestamp(time_t *timestamp, char *format_time)
{
    if (FORMAT_TIME_MAX_LEN == strlen(format_time)){
		*timestamp = get_tick(format_time) - 28800; // 8h时差 = 8*60*60 = 28800
		return 0;
	} 
    return -1;    
}

