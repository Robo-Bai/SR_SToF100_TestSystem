#include <string.h>
#include <stdlib.h>
#include "drivers\include\drivers\drv_hwtimer.h"
#include "drivers\include\drivers\sr_serial.h"
#include "..\app\include\unittest_manager.h"
#include "..\app\include\ev_process.h"
 



 // 参数寄存器 60*4 = 240Bytes
static int32_t state_msg[][3] = {

	/* 寄存器地址，参数个数， 参数 */
	{0x0001, 1, 0x11223344},  /* 运动控制参数区 */

	
};


static struct reg_map reg_msg[] = {
	{
		.type			= 0x07,
		.options		= 0x11,
		.reg			= 0,
		.count			= 0,
		.msgto			= state_msg,
	},

};

static struct utest_map ut_list_state[] = {
	{
		.event	= {
            .type 		= EV_PARAM,
			.items		= sizeof(state_msg)/sizeof(state_msg[0]), /* 共有测试项 */
            .offset		= sizeof(state_msg[0]),
            .regs		= reg_msg,
        },
		.target_count	= 1000000,
		.real_count_rx	= 0,
		.real_count_tx	= 0,
		.cur_timestamp	= 0,
		.last_timestamp	= 0,
		.diff_avr_timestamp	= 0,
		.start_timestamp = 0,
		.end_timestamp	= 0,
		.timeout		= 20,	/* 设定读取状态寄存器等待超时为10ms */
		.input			= 1,
		.output			= 0,
		.expect			= 1,
	},
	
};

extern bool tx_flag;

static int32_t param_handler_callback(void *ops, void *m, uint16_t len)
{
	struct unittest_operations *uops = ops;
	struct reg_map *msg = (struct reg_map *)m;

	if (uops->ut_map->event.regs->reg == msg->reg){
		uops->ut_map->cur_timestamp = system_time_tick();
		uops->ut_map->real_count_rx++;;
		/* 此处单次接收不做除运算，提高整体性能 */
		uops->ut_map->diff_avr_timestamp = (uops->ut_map->diff_avr_timestamp + 
					get_delta_time(uops->ut_map->last_timestamp, uops->ut_map->cur_timestamp));
		
		if (uops->ut_map->real_count_rx == uops->ut_map->target_count){
			uops->ut_map->output = 1;
			uops->ut_map->end_timestamp = system_time_tick();
			print_check(true);
		}
		
		print_check(false);
				
//		if (uops->ut_map->real_count_rx%1 == 0){
//			sr_kprintf("%s, reg=0x%02x, count_tx=%d,  count_rx=%d, diff_avr_timestamp=%f ms\r\n", __FUNCTION__, msg->reg, 
//						uops->ut_map->real_count_tx, uops->ut_map->real_count_rx, 
//						uops->ut_map->diff_avr_timestamp/uops->ut_map->real_count_rx);
//			//sr_data_print("m", (uint8_t *)m, len);
//		}
		tx_flag = true;
	}
	
	return 0;
} 

static int32_t utest_param_init(struct unittest_operations *uops)
{
	// TODO
	uops->ut_map->handler_callback = param_handler_callback;
	
	return 0;
}


extern qevent qev_tx;

static int32_t utest_param_dispatch( struct unittest_operations *uops)
{
	msg_event msg;
	static uint16_t cur_ietms = 0;	// 当前测试项
	
//	sr_kprintf("%s, ev->type=0x%02x, ev->items=%d\r\n", __FUNCTION__, \
				uops->ut_map->event.type, uops->ut_map->event.items);
	
	if (uops->ut_map->real_count_tx < uops->ut_map->target_count){

		if(get_delta_time(uops->ut_map->last_timestamp, uops->ut_map->cur_timestamp) > uops->ut_map->timeout){
			uops->ut_map->output = 0;
			sr_kprintf("error, %s timeout %d ms\r\n", __FUNCTION__, 
						get_delta_time(uops->ut_map->last_timestamp, uops->ut_map->cur_timestamp));
			return -1;
		}

		memset(&msg, 0, sizeof(msg));
		msg.type = uops->ut_map->event.type;

		// 减去2*4，是消息体占用了2个寄存器，这两个寄存器是参数起始地址和参数个数
		msg.len = offsetof(struct reg_map, msgto)/sizeof(uint8_t) + uops->ut_map->event.offset - 2*4;

		(cur_ietms < uops->ut_map->event.items ? 0 : (cur_ietms = 0));
		memcpy(&uops->ut_map->event.regs->reg, 
			(uint8_t *)((uint8_t *)uops->ut_map->event.regs->msgto+cur_ietms*uops->ut_map->event.offset), 
			sizeof(uops->ut_map->event.regs->reg));
		memcpy(&uops->ut_map->event.regs->count, 
			(uint8_t *)((uint8_t *)uops->ut_map->event.regs->msgto+4+cur_ietms*uops->ut_map->event.offset), 
			sizeof(uops->ut_map->event.regs->count));

		// sr_kprintf("%s, reg=0x%04x, count=%d, msg.len=%d\r\n", __FUNCTION__, \
		// 		uops->ut_map->event.regs->reg, uops->ut_map->event.regs->count, msg.len);
		memcpy(&msg.data, (uint8_t *)uops->ut_map->event.regs, offsetof(struct reg_map, msgto)/sizeof(uint8_t));
		memcpy(&msg.data[offsetof(struct reg_map, msgto)/sizeof(uint8_t)], 
			(uint8_t *)((uint8_t *)uops->ut_map->event.regs->msgto + 2*4 + cur_ietms*uops->ut_map->event.offset), 
			uops->ut_map->event.offset);
		
		cur_ietms += 1;
		uops->ut_map->real_count_tx++;
		uops->ut_map->last_timestamp = system_time_tick(); // 此处记录时间戳不太准确,因为数据只是放入事件队列并没有立即发出去
		(uops->ut_map->start_timestamp <= 0 ? (uops->ut_map->start_timestamp = system_time_tick()): 0 );
		return event_dispatch(&qev_tx, &msg);
	}
	return -2;
}

static struct unittest_operations param_ops = {
	.name			= "param",
	.ut_map			= ut_list_state,
	.init			= utest_param_init,
	.dispatch		= utest_param_dispatch,
};


int32_t utest_register_param(void)
{
	// sr_kprintf("%s\r\n", cmd_ops.name);
	// sr_kprintf("%s, type=0x%x, offset=0x%x\r\n", __FUNCTION__, cmd_ops.u_map->event.type, cmd_ops.u_map->event.offset);
	
	return utest_register_ops(&param_ops);
}

