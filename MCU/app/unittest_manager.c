#include <stdlib.h>
#include "core\include\sr_system.h"
#include "..\app\include\unittest_manager.h"
 

extern char *eventmap2str(uint16_t type);

static struct unittest_operations *g_utest_ops_head = NULL;
static struct unittest_operations *g_cur_utest_ops = NULL;



int32_t utest_register_ops(struct unittest_operations *ops)
{
	struct unittest_operations *ptmp;
	
	if (!g_utest_ops_head){
		g_utest_ops_head = ops;
		ops->pNext = NULL;
	} else {
		ptmp = g_utest_ops_head;
		while (ptmp->pNext){
			ptmp = ptmp->pNext;
		}
		ptmp->pNext = ops;
		ops->pNext = NULL;
	}
	return 0;
}


struct unittest_operations *get_utest_handler(void)
{
	return g_cur_utest_ops;
}

static void utest_dump(struct unittest_operations *ops)
{
	sr_kprintf("\n\nTest case: %s\r\n", eventmap2str(ops->ut_map->event.type));
	sr_kprintf("target count: %d, tx: %d, rx: %d\r\n", 
				ops->ut_map->target_count, 
				ops->ut_map->real_count_tx, ops->ut_map->real_count_rx);
	sr_kprintf("average communication delay: %f ms, elapsed time: %d s\r\n", 
				ops->ut_map->diff_avr_timestamp/ops->ut_map->real_count_rx,  
				get_delta_time(ops->ut_map->start_timestamp, ops->ut_map->end_timestamp)/1000);
	
	if (ops->ut_map->expect == ops->ut_map->output){
		sr_kprintf("Test results passed\r\n");
	} else {
		sr_kprintf("Test results FAILED!!\r\n");
	}
}

int32_t utest_item_dispatch(void)
{
	int32_t ret = 0;
	static struct unittest_operations *ptmp = NULL;
	
	((ptmp == NULL)? (ptmp = g_utest_ops_head) : 0);
	
	if (ptmp && ptmp->dispatch){
		ret = ptmp->dispatch(ptmp);
		// 判断当前测试项是否已经结束。若结束，输出测试信息，然后切换到下一个测试项。
		if (ret < 0){
			utest_dump(ptmp);
			ptmp = ptmp->pNext;
			
			// 自动切换到下一个测试项
			if (ptmp && ptmp->dispatch){
				ret = ptmp->dispatch(ptmp);
			} else {
				return ret;
			}
		}
		g_cur_utest_ops = ptmp; // 保存当前测试项
	}

	return ret;
}

static int32_t utest_init(void)
{
	int32_t ret = 0;
	struct unittest_operations *ptmp = g_utest_ops_head;

	while (ptmp){
		if (ptmp->init){
			ret = ptmp->init(ptmp);
			if (ret < 0){
				sr_kprintf("error, %s %s\r\n", __FUNCTION__, ptmp->name);
			}
		}
		ptmp = ptmp->pNext;
	}	
	return ret;
}

void utest_items_list(void)
{
	struct unittest_operations *ptmp = g_utest_ops_head;

	sr_kprintf("%s:\r\n", __FUNCTION__);
	while (ptmp){
		sr_kprintf("%s\r\n", ptmp->name);
		ptmp = ptmp->pNext;
	}	
}


int32_t utest_destory(void)
{
	struct unittest_operations *ptmp = g_utest_ops_head;

	while (ptmp){
		// TODO
		ptmp = ptmp->pNext;
	}	
	return 0;
}

int32_t sr_utest_init(void)
{	
	int32_t ret = 0;
	
	ret = utest_register_cmd();
	ret = utest_register_state();
	ret = utest_register_param();
	
	ret = utest_init();

	utest_items_list();
	return ret;
}

