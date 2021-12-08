#ifndef __MONITOR_MANAGER_H
#define __MONITOR_MANAGER_H

#include "bsp\include\drv_common.h"
#include "drivers\include\sr_system_regs.h"
#include "..\components\drivers\include\sr_sys_fault_code.h"


//#define sr_kprintf(format, ...)		printf(format, ##__VA_ARGS__)


#define TIMESTAMP_1MS		1
#define TIMESTAMP_2MS		2
#define TIMESTAMP_3MS		3
#define TIMESTAMP_4MS		4
#define TIMESTAMP_5MS		5
#define TIMESTAMP_6MS		6
#define TIMESTAMP_7MS		7
#define TIMESTAMP_8MS		8
#define TIMESTAMP_9MS		9
#define TIMESTAMP_10MS		10
#define TIMESTAMP_20MS		20
#define TIMESTAMP_30MS		30
#define TIMESTAMP_40MS		40
#define TIMESTAMP_50MS		50


struct timestamp_mark {
	uint32_t diff;
	uint32_t cnt;
	float per; // 超时百分比:cnt/total_cnt
};

struct monitor_timestamp_mark {
	struct timestamp_mark t1_ms;
	struct timestamp_mark t2_ms;
	struct timestamp_mark t3_ms;
	struct timestamp_mark t4_ms;
	struct timestamp_mark t5_ms;
	struct timestamp_mark t6_ms;
	struct timestamp_mark t7_ms;
	struct timestamp_mark t8_ms;
	struct timestamp_mark t9_ms;
	struct timestamp_mark t10_ms;
	struct timestamp_mark t20_ms;
	struct timestamp_mark t30_ms;
	struct timestamp_mark t40_ms;
	struct timestamp_mark t50_ms;
	struct timestamp_mark range_t50_ms;
};

struct monitor_mananger {
	const char *name;
	uint32_t total_cnt;
	uint32_t last_timestamp;
	struct monitor_timestamp_mark t_mark;
	void (*init)(void);
	bool (*timestamp_update)(const uint32_t cur);
	bool (*timeout_report_handler)(struct monitor_mananger *m_manager);
	void (*dump_monitor_infor)(struct monitor_mananger *ptmonitor_opr);
	int32_t (*sig_handler)(void *sig, uint16_t reg, uint16_t len);
	struct monitor_mananger *ptNext;
};


int32_t sr_monitor_init(void);
int32_t register_monitor_opr(struct monitor_mananger *ptInputOpr);

int32_t system_monitor_register(void);
struct monitor_mananger *get_system_monitor_manager(void);
void monitor_opr_update(const char *name, uint32_t cur_timstamp);
void dump_monitor_infor(struct monitor_mananger *ptmonitor_opr);
int32_t mc_monitor_register(void);
void signal_report(uint32_t sig, uint16_t reg, uint16_t regs_num);


#endif // __MONITOR_MANAGER_H
