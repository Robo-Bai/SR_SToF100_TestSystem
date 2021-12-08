#ifndef __UNITTEST_MANAGER_H
#define __UNITTEST_MANAGER_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>


 typedef int32_t (*handler)(uint8_t *data, uint16_t len);

#pragma pack(1)
struct reg_map { 
	uint8_t type; // 消息类型
	uint8_t options;	// 读/写
	uint16_t reg;
	uint16_t count;
	void *msgto;
};
#pragma pack()

struct uevent {
	uint16_t type;	// 事件类型
	uint16_t items;
	uint16_t offset;
	struct reg_map *regs;
};

struct utest_map {
	struct uevent event;
	int32_t (*handler_callback)(void *uops, void *msg, uint16_t len);		/* 解析接收到的数据 */
	int32_t target_count;
	int32_t real_count_tx;
	uint32_t real_count_rx;
	uint32_t start_timestamp;
	uint32_t end_timestamp;
	uint32_t cur_timestamp;
	uint32_t last_timestamp;
	float diff_avr_timestamp;
	uint32_t timeout;
	uint8_t input;
	uint8_t output;
	uint8_t expect;
};



struct unittest_operations {
	const char *name;
	struct utest_map *ut_map;
	
	int32_t (*init)(struct unittest_operations *);
	int32_t (*data_stream_handler)(struct unittest_operations *);
	int32_t (*dispatch)(struct unittest_operations *);
	// 测试输出-测试输出-测试预期-测试功能函数指针
	int32_t (*utest)(struct unittest_operations *);
	void (*dump)(struct unittest_operations *);
	void (*destory)(void);
	struct unittest_operations *pNext;
};


int32_t sr_utest_init(void);
struct unittest_operations *get_utest_handler(void);
int32_t utest_register_ops(struct unittest_operations *ops);
int32_t utest_item_dispatch(void);
int32_t utest_destory(void);
void utest_items_list(void);

int32_t utest_register_cmd(void);
int32_t utest_register_state(void);
int32_t utest_register_param(void);


#endif // __UNITTEST_MANAGER_H 
