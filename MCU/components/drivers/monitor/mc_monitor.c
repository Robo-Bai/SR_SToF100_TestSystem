#include <string.h>
#include <stdlib.h>
#include "drivers\include\drivers\mc_monitor.h"
#include "drivers\include\drivers\monitor_manager.h"


// VSC监控SROS超时为2s
// SROS发送命令给VSC间隔为100ms

#define RPT_TIME_MS		60000UL /* 1min,ms */


static void mc_monitor_init(void);
static bool mc_timestamp_update(const uint32_t cur);
static bool mc_timeout_report_handler(struct monitor_mananger *m_manager);
static int32_t mc_sig_handler(void *sig, uint16_t reg, uint16_t len);


static struct monitor_mananger g_mc_monitor_opr = {
	.name     		= "monitor_pose_recv",
	.total_cnt      = 0,
	.last_timestamp = 0,
	.init     		= mc_monitor_init,
	.timestamp_update = mc_timestamp_update,
	.timeout_report_handler = mc_timeout_report_handler,
	.dump_monitor_infor = dump_monitor_infor,
	.sig_handler = mc_sig_handler,
};	

static void timestamp_compare(const uint32_t last, const uint32_t cur)
{
	volatile uint32_t diff = 0;
	
	diff = cur - last;
	if (diff <= TIMESTAMP_1MS){
		g_mc_monitor_opr.t_mark.t1_ms.diff = diff;
		g_mc_monitor_opr.t_mark.t1_ms.cnt += 1;
	} else if (diff <= TIMESTAMP_2MS){
		g_mc_monitor_opr.t_mark.t2_ms.diff = diff;
		g_mc_monitor_opr.t_mark.t2_ms.cnt += 1;
	} else if (diff <= TIMESTAMP_3MS){
		g_mc_monitor_opr.t_mark.t3_ms.diff = diff;
		g_mc_monitor_opr.t_mark.t3_ms.cnt += 1;
	} else if (diff <= TIMESTAMP_4MS){
		g_mc_monitor_opr.t_mark.t4_ms.diff = diff;
		g_mc_monitor_opr.t_mark.t4_ms.cnt += 1;
	} else if (diff <= TIMESTAMP_5MS){
		g_mc_monitor_opr.t_mark.t5_ms.diff = diff;
		g_mc_monitor_opr.t_mark.t5_ms.cnt += 1;
	}  else if (diff <= TIMESTAMP_6MS){
		g_mc_monitor_opr.t_mark.t6_ms.diff = diff;
		g_mc_monitor_opr.t_mark.t6_ms.cnt += 1;
	} else if (diff <= TIMESTAMP_7MS){
		g_mc_monitor_opr.t_mark.t7_ms.diff = diff;
		g_mc_monitor_opr.t_mark.t7_ms.cnt += 1;
	} else if (diff <= TIMESTAMP_8MS){
		g_mc_monitor_opr.t_mark.t8_ms.diff = diff;
		g_mc_monitor_opr.t_mark.t8_ms.cnt += 1;
	} else if (diff <= TIMESTAMP_9MS){
		g_mc_monitor_opr.t_mark.t9_ms.diff = diff;
		g_mc_monitor_opr.t_mark.t9_ms.cnt += 1;
	} else if (diff <= TIMESTAMP_10MS){
		g_mc_monitor_opr.t_mark.t10_ms.diff = diff;
		g_mc_monitor_opr.t_mark.t10_ms.cnt += 1;
	}else if (diff <= TIMESTAMP_20MS){
		g_mc_monitor_opr.t_mark.t20_ms.diff = diff;
		g_mc_monitor_opr.t_mark.t20_ms.cnt += 1;
	}else if (diff <= TIMESTAMP_30MS){
		g_mc_monitor_opr.t_mark.t30_ms.diff = diff;
		g_mc_monitor_opr.t_mark.t30_ms.cnt += 1;
	}else if (diff <= TIMESTAMP_40MS){
		g_mc_monitor_opr.t_mark.t40_ms.diff = diff;
		g_mc_monitor_opr.t_mark.t40_ms.cnt += 1;
	}else if (diff <= TIMESTAMP_50MS){
		g_mc_monitor_opr.t_mark.t50_ms.diff = diff;
		g_mc_monitor_opr.t_mark.t50_ms.cnt += 1;
	} else if (diff > TIMESTAMP_50MS){
		// POSE接收超时
		signal_report(SYS_FAULT_CODE_POSE_RX_TIMEOUT_BIT4, REG_MC_FAULT_CODE, 1);
		g_mc_monitor_opr.t_mark.range_t50_ms.diff = diff;
		g_mc_monitor_opr.t_mark.range_t50_ms.cnt += 1;
		//sr_kprintf("diff > TIMESTAMP_50MS diff:%d, cur:%d, last:%d\r\n", diff, cur, last);
		sr_log2sros("pose recv diff > TIMESTAMP_50MS diff:%d, range_t50_ms.cnt:%d\n", diff, g_mc_monitor_opr.t_mark.range_t50_ms.cnt);
	}
}

static void mc_monitor_init(void)
{
	g_mc_monitor_opr.last_timestamp = system_time_tick(); // getSysTime,  system_time_tick
	memset(&g_mc_monitor_opr.t_mark, 0, sizeof(g_mc_monitor_opr.t_mark));
}

static bool mc_timestamp_update(const uint32_t cur)
{
	// uint32_t 最大值按ms算，可以跑49天不溢出
	// while 循环周期有低于1ms的情况，而计数器时是ms级别的，所以if判断时不应该取等号!!
	if (cur < g_mc_monitor_opr.last_timestamp){
		sr_kprintf("%s, warnning:cur < last_timestamp\r\n", __FUNCTION__);
		return false;
	} else {
		g_mc_monitor_opr.total_cnt++;
		timestamp_compare(g_mc_monitor_opr.last_timestamp, cur); 
		g_mc_monitor_opr.last_timestamp = cur;
	}
	return 0;
}

static bool mc_timeout_report_handler(struct monitor_mananger *m_manager)
{	
	static uint32_t last_timestamp = 0;
	uint32_t cur_timestamp = 0;
	
	// 1s发送一次，把超时数据发送到sros
	cur_timestamp = system_time_tick(); // getSysTime,  system_time_tick
	if (abs(cur_timestamp - last_timestamp) >= RPT_TIME_MS){
		#if 0
		// 发送数据到SROS
		uint8_t buf[sizeof(g_mc_monitor_opr.t_mark)+1]; // buf size = 44+1
		
		memset(buf, 0, sizeof(buf));
		buf[0] = MSG_EM_STATE;
		memcpy(&buf[1], &g_mc_monitor_opr.t_mark, sizeof(g_mc_monitor_opr.t_mark));
		ser1WriteData(buf, sizeof(buf));
		#else
		// 计算超时百分比
		if (g_mc_monitor_opr.total_cnt > 0){
			g_mc_monitor_opr.t_mark.t1_ms.per = 
				(float)g_mc_monitor_opr.t_mark.t1_ms.cnt/g_mc_monitor_opr.total_cnt*100;		
			g_mc_monitor_opr.t_mark.t2_ms.per = 
				(float)g_mc_monitor_opr.t_mark.t2_ms.cnt/g_mc_monitor_opr.total_cnt*100;		
			g_mc_monitor_opr.t_mark.t3_ms.per = 
				(float)g_mc_monitor_opr.t_mark.t3_ms.cnt/g_mc_monitor_opr.total_cnt*100;
			g_mc_monitor_opr.t_mark.t4_ms.per = 
				(float)g_mc_monitor_opr.t_mark.t4_ms.cnt/g_mc_monitor_opr.total_cnt*100;
			g_mc_monitor_opr.t_mark.t5_ms.per = 
				(float)g_mc_monitor_opr.t_mark.t5_ms.cnt/g_mc_monitor_opr.total_cnt*100;
			g_mc_monitor_opr.t_mark.t6_ms.per = 
				(float)g_mc_monitor_opr.t_mark.t6_ms.cnt/g_mc_monitor_opr.total_cnt*100;
			g_mc_monitor_opr.t_mark.t7_ms.per = 
				(float)g_mc_monitor_opr.t_mark.t7_ms.cnt/g_mc_monitor_opr.total_cnt*100;
			g_mc_monitor_opr.t_mark.t8_ms.per = 
				(float)g_mc_monitor_opr.t_mark.t8_ms.cnt/g_mc_monitor_opr.total_cnt*100;
			g_mc_monitor_opr.t_mark.t9_ms.per = 
				(float)g_mc_monitor_opr.t_mark.t9_ms.cnt/g_mc_monitor_opr.total_cnt*100;
			g_mc_monitor_opr.t_mark.t10_ms.per = 
				(float)g_mc_monitor_opr.t_mark.t10_ms.cnt/g_mc_monitor_opr.total_cnt*100;
			g_mc_monitor_opr.t_mark.t20_ms.per = 
				(float)g_mc_monitor_opr.t_mark.t20_ms.cnt/g_mc_monitor_opr.total_cnt*100;
			g_mc_monitor_opr.t_mark.t30_ms.per = 
				(float)g_mc_monitor_opr.t_mark.t30_ms.cnt/g_mc_monitor_opr.total_cnt*100;
			g_mc_monitor_opr.t_mark.t40_ms.per = 
				(float)g_mc_monitor_opr.t_mark.t40_ms.cnt/g_mc_monitor_opr.total_cnt*100;
			g_mc_monitor_opr.t_mark.t50_ms.per = 
				(float)g_mc_monitor_opr.t_mark.t50_ms.cnt/g_mc_monitor_opr.total_cnt*100;
			g_mc_monitor_opr.t_mark.range_t50_ms.per = 
				(float)g_mc_monitor_opr.t_mark.range_t50_ms.cnt/g_mc_monitor_opr.total_cnt*100;
		}
		// log输出
		m_manager->dump_monitor_infor(m_manager);
		#endif		
		last_timestamp = cur_timestamp;
	}
	
	return 0;
}

static int32_t mc_sig_handler(void *sig, uint16_t reg, uint16_t len)
{

	return 0;
}

struct monitor_mananger *get_mc_monitor_manager(void)
{
	 return &g_mc_monitor_opr;
}


int32_t mc_monitor_register(void)
{
	int32_t ret = 0;
	
	//sr_kprintf("%s, oprEvent:%d\r\n", __FUNCTION__, sizeof(g_mc_monitor_opr)/sizeof(g_mc_monitor_opr[0]));
	ret = register_monitor_opr(&g_mc_monitor_opr);
	return ret;
}

