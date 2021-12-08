#ifndef __RTC_H__
#define __RTC_H__


#include <stdint.h>
#include <time.h>


/**
 * 时间戳转化为标准格式的时间
 *
 * @param[in]	timestamp:时间戳,例"1581844348"
 * @param[out]	format_time：标准格式时间，例"2020-02-16 17:12:28"
 *
 * @return		0表示成功,其它返回失败
 */
int32_t sr_timestamp2time(time_t timestamp, char *format_time);


/**
 * 标准格式的时间转化为时间戳
 *
 * @param[in]	timestamp:时间戳,例"1581844348"
 * @param[out]	format_time：标准格式时间，例"2020-02-16 17:12:28"
 *
 * @return		0表示成功,其它返回失败
 */
int32_t sr_time2timestamp(time_t *timestamp, char *format_time);


#endif /* __RTC_H__ */