#ifndef __SR_SYSTEM_REGS_H__
#define __SR_SYSTEM_REGS_H__

#include <stdint.h>

typedef enum {
	MC_STATE,
	AC_STATE,
	SYS_STATE,
	ALL_STATE
} subtask_state_type;

// 区域块顺序必须 与regs_manager[]表一致
enum regs_block_t {
	REG_BLOCK_CMD,
	REG_BLOCK_STATE,
	REG_BLOCK_PARAM,
	REG_BLOCK_USER,
	REG_NULL
};


typedef enum
{
    REG_READ =  0x10,              /*!< Read register values and pass to protocol stack. */
    REG_WRITE  = 0x11             /*!< Update register values. */
} register_mode_t;



typedef struct reg_operations {
	int32_t (*init)(void);
	int32_t (*semaphore_take_manager)(void);
	int32_t (*semaphore_give_manager)(void);

	int32_t (*regs_manager_write)(struct reg_operations *reg_op, uint32_t *block, 
		int32_t* data, uint16_t regs_num);
	int32_t (*regs_manager_read)(struct reg_operations *reg_op,uint32_t *block, 
		int32_t* data, uint16_t regs_num);

} reg_operations_t;


/* 内存映射从低到高地址映射 */
enum memory_block_size {
	/* 通信协议定义配置参数为4k */
	MEM_BLOCK_PARAM_MC = 256,	/* 目前实际编址MC参数暂定256*4=1024B, 0 ~ 256-1 */
	MEM_BLOCK_PARAM_SYS = 256,	/* 目前实际编址MC参数暂定256*4=1024B, 256 ~ 512-1  */
	MEM_BLOCK_PARAM_AC = 256, 	/* 目前实际编址MC参数暂定256*4=1024B, 512 ~ 768-1 */
	

	/* 通信协议定义状态块为8k */
	MEM_BLOCK_STATE_HW = 512, 	/* 目前实际编址硬件状态暂定2KB, 768 ~ 1280-1 */
	MEM_BLOCK_STATE_SYS = 128,  /* 目前实际编址系统状态暂定512B,    	1280 ~ 1408-1 */
	MEM_BLOCK_STATE_MC = 256,	/* 目前实际编址MC状态暂定1KB,  1408 ~ 1664-1 */
	MEM_BLOCK_STATE_AC = 256,	/* 目前实际编址AC状态暂定1KB,  1664 ~ 1920-1 */

	/* 通信协议定义命令块为4k */
	MEM_BLOCK_CMD = 128, 		/* 目前实际编址命令寄存器暂定512B，1920 ~ 2048-1 */

	/* 通信协议定义为8k */
	MEM_BLOCK_USER = 256,		/* 目前实际编址命令寄存器暂定1KB, 2048 ~ 2304-1 */
};


#define BLOCK_PARAM_BASE_ADDR						(0x0000)
#define BLOCK_PARAM_OFFSET							(0x0FFF) /* 实际地址偏移 = 128*3 */
#define BLOCK_PARAM_END_ADDR						(BLOCK_PARAM_BASE_ADDR+BLOCK_PARAM_OFFSET)

#define BLOCK_STATE_BASE_ADDR						(0x1000)
#define BLOCK_STATE_OFFSET							(0x1FFF) /* 实际地址偏移 = HW+SYS+MC+AC */
#define BLOCK_STATE_END_ADDR						(BLOCK_STATE_BASE_ADDR+BLOCK_STATE_OFFSET)
#define BLOCK_STATE_SYS_BASE_ADDR					(0x2000)
#define BLOCK_STATE_MC_BASE_ADDR					(0x2400)
#define BLOCK_STATE_AC_BASE_ADDR					(0x2800)


#define BLOCK_CMD_START_ADDR						(0x3000)
#define BLOCK_CMD_OFFSET							(0x0FFF)
#define BLOCK_CMD_END_ADDR							(BLOCK_CMD_START_ADDR+BLOCK_CMD_OFFSET)

#define BLOCK_USER_START_ADDR						(0x4000)
#define BLOCK_USER_OFFSET							(0x1FFF)
#define BLOCK_USER_END_ADDR							(BLOCK_USER_START_ADDR+BLOCK_USER_OFFSET)


/* 命令(CMD)寄存器，严格按照此顺序，不能修改!! */
#define REG_CMD_OFFET								(8)		/* 命令字寄存器个数（1个寄存器占4字节） */
#define REG_CMD										(0x3000)


/****** 系统状态(SYSTEM STATE)寄存器，严格按照此顺序，不能修改!! ******/
#define REG_SYSTEM_STATE_INFO_OFFSET				(18) /* 此寄存器为系统状态信息总寄存器长度 */
#define REG_BOARD_NAME_OFFET						(2)
#define REG_BOARD_NAME								(0x2000)

#define REG_HW_VERSION_OFFET						(1)
#define REG_HW_VERSION								(REG_BOARD_NAME+REG_BOARD_NAME_OFFET)

#define REG_HW_SERIAL_NO_OFFET						(4)
#define REG_HW_SERIAL_NO							(REG_HW_VERSION+REG_HW_VERSION_OFFET)

#define REG_KERNER_VERSION_OFFET					(1)
#define REG_KERNER_VERSION							(REG_HW_SERIAL_NO+REG_HW_SERIAL_NO_OFFET)

#define REG_FM_VERSION_OFFET						(1)
#define REG_FM_VERSION								(REG_KERNER_VERSION+REG_KERNER_VERSION_OFFET)

#define REG_FM_COMMIT_ID_OFFET						(1)
#define REG_FM_COMMIT_ID							(REG_FM_VERSION+REG_FM_VERSION_OFFET)

#define REG_FM_COMMIT_BRANCH_OFFET					(4)
#define REG_FM_COMMIT_BRANCH						(REG_FM_COMMIT_ID+REG_FM_COMMIT_ID_OFFET)

#define REG_FM_COMPILE_DATE_OFFET					(1)	
#define REG_FM_COMPILE_DATE							(REG_FM_COMMIT_BRANCH+REG_FM_COMMIT_BRANCH_OFFET)

#define REG_SYSTEM_RESVERED1_OFFET					(2)
#define REG_SYSTEM_RESVERED1						(REG_FM_COMPILE_DATE+REG_FM_COMPILE_DATE_OFFET)

#define REG_SYSTEM_STATE_OFFSET						(16) /* 此寄存器为系统状态总偏移 */
#define REG_REQ_SEQ_OFFET							(1)
#define REG_REQ_SEQ									(REG_SYSTEM_RESVERED1+REG_SYSTEM_RESVERED1_OFFET)

#define REG_HEART_OFFET								(1)	
#define REG_HEART									(REG_REQ_SEQ+REG_REQ_SEQ_OFFET)

#define REG_SYSTEM_STATE_OFFET						(1)
#define REG_SYSTEM_STATE							(REG_HEART+REG_HEART_OFFET)

#define REG_SYSTEM_STATE_CODE_OFFET					(1)
#define REG_SYSTEM_STATE_CODE						(REG_SYSTEM_STATE+REG_SYSTEM_STATE_OFFET)

#define REG_SUBTASK_MC_STATE_OFFET					(1)
#define REG_SUBTASK_MC_STATE						(REG_SYSTEM_STATE_CODE+REG_SYSTEM_STATE_CODE_OFFET)

#define REG_SUBTASK_AC_STATE_OFFET					(1)
#define REG_SUBTASK_AC_STATE						(REG_SUBTASK_MC_STATE+REG_SUBTASK_MC_STATE_OFFET)

#define REG_TOTAL_POWER_CYCLE_OFFET					(1)
#define REG_TOTAL_POWER_CYCLE						(REG_SUBTASK_AC_STATE+REG_SUBTASK_AC_STATE_OFFET)

#define REG_TOTAL_POWERON_TIME_OFFET				(1)	
#define REG_TOTAL_POWERON_TIME						(REG_TOTAL_POWER_CYCLE+REG_TOTAL_POWER_CYCLE_OFFET)

#define REG_TOTAL_MILEAGE_OFFET						(1)
#define REG_TOTAL_MILEAGE							(REG_TOTAL_POWERON_TIME+REG_TOTAL_POWERON_TIME_OFFET)

#define REG_TOTAL_DEVICES_OFFET						(1)
#define REG_TOTAL_DEVICES							(REG_TOTAL_MILEAGE+REG_TOTAL_MILEAGE_OFFET)

#define REG_GPIO_INPUT_OFFET						(1)
#define REG_GPIO_INPUT								(REG_TOTAL_DEVICES+REG_TOTAL_DEVICES_OFFET)

#define REG_GPIO_OUTPUT_OFFET						(1)
#define REG_GPIO_OUTPUT								(REG_GPIO_INPUT+REG_GPIO_INPUT_OFFET)

#define REG_SYSTEM_RESVERED2_OFFET					(4)
#define REG_SYSTEM_RESVERED2						(REG_GPIO_OUTPUT+REG_GPIO_OUTPUT_OFFET)

/* 故障码寄存器 */
#define REG_ALL_FAULT_CODE_OFFSET					(8)
#define REG_SYS_FAULT_CODE							(REG_SYSTEM_RESVERED2+REG_SYSTEM_RESVERED2_OFFET)
#define REG_MC_FAULT_CODE							(REG_SYS_FAULT_CODE+1)
#define REG_AC_FAULT_CODE							(REG_MC_FAULT_CODE+1)


/* 硬件(HW)设备状态寄存器，严格按照此顺序 */
#define REG_CPU_USAGE_OFFET							(1)
#define REG_CPU_USAGE								(BLOCK_STATE_BASE_ADDR)

#define REG_CPU_MEM_OFFET							(1)
#define REG_CPU_MEM									(REG_CPU_USAGE+REG_CPU_USAGE_OFFET)

#define REG_CPU_RESERVED_OFFET						(2)
#define REG_CPU_RESERVED							(REG_CPU_MEM+REG_CPU_MEM_OFFET)

#define REG_WDT_CORE_FAULT_OFFET					(1)	
#define REG_WDT_CORE_FAULT							(REG_CPU_RESERVED+REG_CPU_RESERVED_OFFET)

#define REG_WDT_LAST_POS_OFFET						(1)	
#define REG_WDT_LAST_POS							(REG_WDT_CORE_FAULT+REG_WDT_CORE_FAULT_OFFET)

#define REG_WDT_RESERVED_OFFET						(2)
#define REG_WDT_RESERVED							(REG_WDT_LAST_POS+REG_WDT_LAST_POS_OFFET)

#define REG_ALL_DEVICE_STATUS_OFFET					(16)
#define REG_ALL_DEVICE_STATUS						(REG_WDT_RESERVED+REG_WDT_RESERVED_OFFET)

#define REG_DEVICE_BASE_OFFET						(32)
#define REG_DEVICE_BASE								(REG_ALL_DEVICE_STATUS+REG_ALL_DEVICE_STATUS_OFFET)


/* 参数块，包含（运动控制、动作控制、系统参数寄存器） */
/* 系统参数寄存器，使用以下的配置项逻辑地址 */
#define REG_PARA_SYS_OFFSET							(256)
#define REG_PARA_SYS								(0x0201)

#define REG_PARA_SYS_COORDINATE						(REG_PARA_SYS)


typedef enum {
	SYS_INVALID = 0,
	SYS_INITING,
	SYS_IDLE,
	SYS_BUSY,
	SYS_ERROR
} run_state_t;

typedef enum {
	START_NONE = 0,
	START_SUCCESS,
	START_BUSY,
	START_AC_ID_NONE,
	START_AC_PARA_1_NONE,
	START_AC_PARA_2_NONE
} task_start_result_t;

typedef enum {
	RUN_NONE = 0,
	RUN_WAITTING = 0x02,
	RUN_RUNNING = 0x03,
	RUN_PAUSED = 0x04,
	RUN_DONE = 0x05,
	RUN_CANCELLING = 0x06
} task_run_state_t;

typedef enum {
	END_NONE = 0,
	END_OK,
	END_CANCELED,
	END_FAILED
} task_end_result_t;

#pragma pack(4)
typedef struct { 
	int32_t task_no;		/* 当前正在执行的任务编号 */
	int32_t task_id;		/* 任务ID */
	int32_t task_p0;		/* 任务参数p0 */
	int32_t task_p1;		/* 任务参数p1 */
	task_run_state_t	run_state;
	task_end_result_t	end_result;
	int32_t				result_code;
	int32_t 			reserved[4];
} sr_new_task_t;


typedef struct {
	uint32_t	req_seq;
	uint32_t	heart;
	run_state_t run_state;
	uint32_t 	error_code;
	uint32_t 	subtask_mc_state;
	uint32_t 	subtask_ac_state;
	uint32_t	total_power_cycle;
	uint32_t	total_poweron_time;
	uint32_t	total_mileage;
	uint32_t	total_devices;
	uint32_t	gpio_input;
	uint32_t	gpio_output;
	int32_t		reserver[4];
} sr_system_state_t;

typedef struct {
	uint32_t	hw_version;
	uint32_t	hw_serial_no[4];
	uint32_t	kernel_version;
	uint32_t	fm_version;
	uint32_t	fm_commit_id[2];
	uint32_t	fm_compile_date;

} sr_system_info_t;
#pragma pack()



uint32_t* mem_block_remapping(enum regs_block_t block, uint16_t reg);
enum regs_block_t regs_block_get(uint16_t reg);

reg_operations_t* reg_operations_get(void);
int32_t sr_reg_manager_init(void);

int32_t sr_reg_write(uint16_t reg, uint16_t regs_num, int32_t* data);
int32_t sr_reg_read(uint16_t reg, uint16_t regs_num, int32_t* data);


#endif /* __SR_SYSTEM_REGS_H__ */
