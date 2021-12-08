#include <stdio.h>
#include <string.h>
#include "drivers\include\drivers\monitor_manager.h"



struct monitor_mananger *g_ptmonitor_opr_head;


int32_t register_monitor_opr(struct monitor_mananger *ptmonitor_opr)
{
	struct monitor_mananger *ptTmp;

	if (!g_ptmonitor_opr_head) {
		g_ptmonitor_opr_head   = ptmonitor_opr;
		ptmonitor_opr->ptNext = NULL;
	}
	else {
		ptTmp = g_ptmonitor_opr_head;
		while (ptTmp->ptNext) {
			ptTmp = ptTmp->ptNext;
		}
		ptTmp->ptNext	  = ptmonitor_opr;
		ptmonitor_opr->ptNext = NULL;
	}

	return 0;
}

#if 0
void show_monitor_opr(void)
{
	int32_t i = 0;
	struct monitor_mananger * ptTmp = g_ptmonitor_opr_head;

	while (ptTmp){
		sr_kprintf("%02d %s\n", i++, ptTmp->name);
		if (ptTmp->dump_monitor_infor){
			ptTmp->dump_monitor_infor(ptTmp->name, &ptTmp->t_mark);
		}
		ptTmp = ptTmp->ptNext;
	}
}
#endif

void dump_monitor_infor(struct monitor_mananger *ptmonitor_opr)
{
	#if 0
	sr_kprintf("\r\n======%s======\r\n", __FUNCTION__);
	sr_kprintf("%s.total_cnt:%d\r\n", ptmonitor_opr->name, ptmonitor_opr->total_cnt);
	sr_kprintf("1ms     \t [%.2f%%] \t cnt:%d\r\n", ptmonitor_opr->t_mark.t1_ms.per, ptmonitor_opr->t_mark.t1_ms.cnt);
	sr_kprintf("2ms     \t [%.2f%%] \t cnt:%d\r\n", ptmonitor_opr->t_mark.t2_ms.per, ptmonitor_opr->t_mark.t2_ms.cnt);
	sr_kprintf("3ms     \t [%.2f%%] \t cnt:%d\r\n", ptmonitor_opr->t_mark.t3_ms.per, ptmonitor_opr->t_mark.t3_ms.cnt);
	sr_kprintf("4ms     \t [%.2f%%] \t cnt:%d\r\n", ptmonitor_opr->t_mark.t4_ms.per, ptmonitor_opr->t_mark.t4_ms.cnt);
	sr_kprintf("5ms     \t [%.2f%%] \t cnt:%d\r\n", ptmonitor_opr->t_mark.t5_ms.per, ptmonitor_opr->t_mark.t5_ms.cnt);
	sr_kprintf("6ms     \t [%.2f%%] \t cnt:%d\r\n", ptmonitor_opr->t_mark.t6_ms.per, ptmonitor_opr->t_mark.t6_ms.cnt);
	sr_kprintf("7ms     \t [%.2f%%] \t cnt:%d\r\n", ptmonitor_opr->t_mark.t7_ms.per, ptmonitor_opr->t_mark.t7_ms.cnt);
	sr_kprintf("8ms     \t [%.2f%%] \t cnt:%d\r\n", ptmonitor_opr->t_mark.t8_ms.per, ptmonitor_opr->t_mark.t8_ms.cnt);
	sr_kprintf("9ms     \t [%.2f%%] \t cnt:%d\r\n", ptmonitor_opr->t_mark.t9_ms.per, ptmonitor_opr->t_mark.t9_ms.cnt);
	sr_kprintf("10ms    \t [%.2f%%] \t cnt:%d\r\n", ptmonitor_opr->t_mark.t10_ms.per, ptmonitor_opr->t_mark.t10_ms.cnt);
	sr_kprintf("10~20ms \t [%.2f%%] \t cnt:%d\r\n", ptmonitor_opr->t_mark.t20_ms.per, ptmonitor_opr->t_mark.t20_ms.cnt);
	sr_kprintf("20~30ms \t [%.2f%%] \t cnt:%d\r\n", ptmonitor_opr->t_mark.t30_ms.per, ptmonitor_opr->t_mark.t30_ms.cnt);
	sr_kprintf("30~40ms \t [%.2f%%] \t cnt:%d\r\n", ptmonitor_opr->t_mark.t40_ms.per, ptmonitor_opr->t_mark.t40_ms.cnt);
	sr_kprintf("40~50ms \t [%.2f%%] \t cnt:%d\r\n", ptmonitor_opr->t_mark.t50_ms.per, ptmonitor_opr->t_mark.t50_ms.cnt);
	sr_kprintf("range 50ms\t [%.2f%%] \t cnt:%d\r\n", ptmonitor_opr->t_mark.range_t50_ms.per, ptmonitor_opr->t_mark.range_t50_ms.cnt);
	#endif
	
	sr_log2sros("\r\n======%s======\n", __FUNCTION__);
	sr_log2sros("%s.total_cnt:%d\r\n", ptmonitor_opr->name, ptmonitor_opr->total_cnt);
	sr_log2sros("1ms     \t [%.2f%%] \t cnt:%d\n", ptmonitor_opr->t_mark.t1_ms.per, ptmonitor_opr->t_mark.t1_ms.cnt);
	sr_log2sros("2ms     \t [%.2f%%] \t cnt:%d\n", ptmonitor_opr->t_mark.t2_ms.per, ptmonitor_opr->t_mark.t2_ms.cnt);
	sr_log2sros("3ms     \t [%.2f%%] \t cnt:%d\n", ptmonitor_opr->t_mark.t3_ms.per, ptmonitor_opr->t_mark.t3_ms.cnt);
	sr_log2sros("4ms     \t [%.2f%%] \t cnt:%d\n", ptmonitor_opr->t_mark.t4_ms.per, ptmonitor_opr->t_mark.t4_ms.cnt);
	sr_log2sros("5ms     \t [%.2f%%] \t cnt:%d\n", ptmonitor_opr->t_mark.t5_ms.per, ptmonitor_opr->t_mark.t5_ms.cnt);
	sr_log2sros("6ms     \t [%.2f%%] \t cnt:%d\n", ptmonitor_opr->t_mark.t6_ms.per, ptmonitor_opr->t_mark.t6_ms.cnt);
	sr_log2sros("7ms     \t [%.2f%%] \t cnt:%d\n", ptmonitor_opr->t_mark.t7_ms.per, ptmonitor_opr->t_mark.t7_ms.cnt);
	sr_log2sros("8ms     \t [%.2f%%] \t cnt:%d\n", ptmonitor_opr->t_mark.t8_ms.per, ptmonitor_opr->t_mark.t8_ms.cnt);
	sr_log2sros("9ms     \t [%.2f%%] \t cnt:%d\n", ptmonitor_opr->t_mark.t9_ms.per, ptmonitor_opr->t_mark.t9_ms.cnt);
	sr_log2sros("10ms    \t [%.2f%%] \t cnt:%d\n", ptmonitor_opr->t_mark.t10_ms.per, ptmonitor_opr->t_mark.t10_ms.cnt);
	sr_log2sros("10~20ms \t [%.2f%%] \t cnt:%d\n", ptmonitor_opr->t_mark.t20_ms.per, ptmonitor_opr->t_mark.t20_ms.cnt);
	sr_log2sros("20~30ms \t [%.2f%%] \t cnt:%d\n", ptmonitor_opr->t_mark.t30_ms.per, ptmonitor_opr->t_mark.t30_ms.cnt);
	sr_log2sros("30~40ms \t [%.2f%%] \t cnt:%d\n", ptmonitor_opr->t_mark.t40_ms.per, ptmonitor_opr->t_mark.t40_ms.cnt);
	sr_log2sros("40~50ms \t [%.2f%%] \t cnt:%d\n", ptmonitor_opr->t_mark.t50_ms.per, ptmonitor_opr->t_mark.t50_ms.cnt);
	sr_log2sros("range 50ms\t [%.2f%%] \t cnt:%d\n", ptmonitor_opr->t_mark.range_t50_ms.per, ptmonitor_opr->t_mark.range_t50_ms.cnt);
}


void monitor_opr_update(const char *name, uint32_t cur_timstamp)
{
	struct monitor_mananger * ptTmp = g_ptmonitor_opr_head;

	while (ptTmp){
		if (strcmp(ptTmp->name, name) == 0){
			if (ptTmp->timestamp_update && ptTmp->timeout_report_handler){
				ptTmp->timestamp_update(cur_timstamp);
				ptTmp->timeout_report_handler(ptTmp);
			}
			return;			
		} else {
			ptTmp = ptTmp->ptNext;
		}
	}
}


static int32_t monitor_init(void)
{
	struct monitor_mananger * ptTmp = g_ptmonitor_opr_head;
	
	while (ptTmp) {
		if (ptTmp->init){
			ptTmp->init();
		}
		ptTmp = ptTmp->ptNext;
	}

	return 0;
}

// TODO
// 需要建立轻量级消息通知机制，底层只上报异常状态，不做逻辑处理!!
// CAN发送超时，写入故障码到寄存器
// 无错误码时，也要同步刷新设备状态 
// 特殊情况：当电机正常工作时，CAN总线突然断开，要人为强制构造错误码
// 并写入设备错误码寄存器，便于SROS及时更新状态显示
//	sr_reg_read(REG_ALL_FAULT_CODE, 1, &sys_fault_code);
//	sys_fault_code &= ~(SYS_FAULT_CODE_CAN_TX_TIMEOUT_BIT7);
//	sys_fault_code |= (SYS_FAULT_CODE_CAN_TX_TIMEOUT_BIT7);
//	sr_reg_write(REG_ALL_FAULT_CODE, 1, &sys_fault_code);
void signal_report(uint32_t sig, uint16_t reg, uint16_t regs_num)
{
	int32_t fault_code = 0;
	
	switch (reg){
		case REG_MC_FAULT_CODE:
		case REG_AC_FAULT_CODE:	
		case REG_SYS_FAULT_CODE:
			sr_reg_read(reg, 1, &fault_code);
			fault_code &= ~(sig);
			fault_code |= (sig);
			sr_reg_write(reg, regs_num, &fault_code);
			break;
		
		default:
			break;
	}
}

int32_t sr_monitor_init(void)
{
	int32_t ret;
	
	// 注册监控单元
	ret = system_monitor_register();
	ret = mc_monitor_register();
	
	// 初始化监控单元
	ret |= monitor_init();
	
	return ret;
}
