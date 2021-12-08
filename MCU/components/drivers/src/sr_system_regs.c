#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>
#include <stdio.h>
#include <string.h>
#include "srtos\srtos.h"
#include "srtos\err_code.h"
#include "drivers\include\sr_system_regs.h"



static int32_t semaphore_take_manage(void);
static int32_t semaphore_give_manage(void);
static int32_t regs_write(reg_operations_t *reg_op,  uint32_t *block, 
									int32_t* data, uint16_t regs_num);
static int32_t regs_read(reg_operations_t *reg_op,	uint32_t *block, 
									int32_t* data, uint16_t regs_num);
static int32_t init_manager(void);


#define VIR_MEMORY_MAX_SIZE			(MEM_BLOCK_PARAM_MC + MEM_BLOCK_PARAM_AC \
										+ MEM_BLOCK_PARAM_SYS + MEM_BLOCK_STATE_HW \
										+ MEM_BLOCK_STATE_SYS + MEM_BLOCK_STATE_MC \
										+ MEM_BLOCK_STATE_MC + MEM_BLOCK_CMD	\
										+ MEM_BLOCK_USER)

static volatile int32_t vir_memory[VIR_MEMORY_MAX_SIZE] = {0};
static SemaphoreHandle_t semaphore_manager = NULL;

static reg_operations_t reg_ops = {
	.init							= init_manager,
	.semaphore_take_manager 		= semaphore_take_manage,
	.semaphore_give_manager 		= semaphore_give_manage,
	.regs_manager_write				= regs_write,
	.regs_manager_read				= regs_read,
};

static int32_t init_manager(void)
{
	if (!semaphore_manager){
		semaphore_manager = xSemaphoreCreateMutex();
		if (!semaphore_manager) {
			sr_kprintf("%s, error semaphore_manager\r\n", __FUNCTION__);
			return -1;
		}
	}

	return 0;
}

int32_t sr_reg_manager_init(void)
{
	return init_manager();
}

static int32_t semaphore_take_manage(void)
{
	return xSemaphoreTake(semaphore_manager, portMAX_DELAY);
}

static int32_t semaphore_give_manage(void)
{
	return xSemaphoreGive( semaphore_manager );
}


reg_operations_t* reg_operations_get(void)
{
	return &reg_ops;
}
							

static int32_t regs_write(reg_operations_t *reg_op,  uint32_t *block, 
							int32_t* data, uint16_t regs_num)
{
	if (!block) {
		sr_kprintf("%s, block is mull\r\n", __FUNCTION__);
		return -1;
	}

	if (reg_op->semaphore_take_manager() == pdTRUE ){
		memcpy(block, (void*)data, regs_num<<2);
		//sr_data_print("regs_write", (const uint8_t *)data, sizeof(uint32_t)*regs_num);
		reg_op->semaphore_give_manager();
	}
	return 0;
}

static int32_t regs_read(reg_operations_t *reg_op,  uint32_t *block, 
							int32_t* data, uint16_t regs_num)
{
	if (!block) {
		sr_kprintf("%s, block is null\r\n", __FUNCTION__);
		return -1;
	}

	if (reg_op->semaphore_take_manager() == pdTRUE ){
		memcpy(data, block, regs_num<<2);
		//sr_data_print("regs_read", (const uint8_t *)data, sizeof(uint32_t)*regs_num);
		reg_op->semaphore_give_manager();
	}
	return 0;
}


uint32_t* mem_block_remapping(enum regs_block_t block, uint16_t reg)
{
	uint16_t offset = 0;
	uint16_t base = 0;
	uint32_t *mem = NULL;

	taskENTER_CRITICAL();
	switch (block) {
		case REG_BLOCK_PARAM:
			offset = (reg&0xFFFF) - BLOCK_PARAM_BASE_ADDR;
			base = 0;
			mem = (uint32_t*)&vir_memory[base+offset];
			//sr_kprintf("PARAM_MC: offset=%d, base=0x%x, mem=0x%p\r\n", 
			//	offset, base, mem);
			break;
						
		case REG_BLOCK_STATE:
			if (reg < BLOCK_STATE_SYS_BASE_ADDR){
				offset = reg - BLOCK_STATE_BASE_ADDR;
				base = MEM_BLOCK_PARAM_MC+MEM_BLOCK_PARAM_AC+MEM_BLOCK_PARAM_SYS;
			
			} else if (reg < BLOCK_STATE_MC_BASE_ADDR){
				offset = reg - BLOCK_STATE_SYS_BASE_ADDR;
				base = MEM_BLOCK_PARAM_MC+MEM_BLOCK_PARAM_AC+MEM_BLOCK_PARAM_SYS
						+MEM_BLOCK_STATE_HW;

			} else if (reg < BLOCK_STATE_AC_BASE_ADDR){
				offset = reg - BLOCK_STATE_MC_BASE_ADDR;
				base = MEM_BLOCK_PARAM_MC+MEM_BLOCK_PARAM_AC+MEM_BLOCK_PARAM_SYS
						+ MEM_BLOCK_STATE_HW + MEM_BLOCK_STATE_SYS;	
				
			} else if (reg < (BLOCK_STATE_AC_BASE_ADDR + 0x400)){
				offset = reg - BLOCK_STATE_AC_BASE_ADDR;
				base = MEM_BLOCK_PARAM_MC+MEM_BLOCK_PARAM_AC+MEM_BLOCK_PARAM_SYS
						+MEM_BLOCK_STATE_HW + MEM_BLOCK_STATE_SYS + MEM_BLOCK_STATE_MC;
			}

			mem = (uint32_t*)&vir_memory[base+offset];
			//sr_kprintf("BLOCK_STATE: offset=%d, base=0x%x, mem=0x%p\r\n", 
			//	offset, base, mem);
			break;	
		case REG_BLOCK_CMD:
			offset = reg - BLOCK_CMD_START_ADDR;
			base = MEM_BLOCK_PARAM_MC+MEM_BLOCK_PARAM_AC+MEM_BLOCK_PARAM_SYS
						+ MEM_BLOCK_STATE_HW + MEM_BLOCK_STATE_SYS + MEM_BLOCK_STATE_MC
						+ MEM_BLOCK_STATE_AC;
			mem = (uint32_t*)&vir_memory[base+offset];
			break;
		case REG_BLOCK_USER:
			offset = reg - BLOCK_USER_START_ADDR;
			base = MEM_BLOCK_PARAM_MC+MEM_BLOCK_PARAM_AC+MEM_BLOCK_PARAM_SYS
						+ MEM_BLOCK_STATE_HW + MEM_BLOCK_STATE_SYS + MEM_BLOCK_STATE_MC
						+ MEM_BLOCK_STATE_AC + MEM_BLOCK_CMD;
			mem = (uint32_t*)&vir_memory[base+offset];
			break;

		default:
			break;
	}
	taskEXIT_CRITICAL();
	return mem;
}

enum regs_block_t regs_block_get(uint16_t reg)
{
	if (reg >= BLOCK_CMD_START_ADDR && reg <= BLOCK_CMD_END_ADDR){
		return REG_BLOCK_CMD;
	}
	
	if (reg >= BLOCK_STATE_BASE_ADDR && reg <= BLOCK_STATE_END_ADDR){
		return REG_BLOCK_STATE;
	}

	if (reg <= BLOCK_PARAM_END_ADDR){
		return REG_BLOCK_PARAM;
	}
	if (reg >= BLOCK_USER_START_ADDR && reg <= BLOCK_USER_END_ADDR){
		return REG_BLOCK_USER;
	}
	return REG_NULL;
}


int32_t sr_reg_read(uint16_t reg, uint16_t regs_num, int32_t* data)
{
	return reg_ops.regs_manager_read(&reg_ops, 
							mem_block_remapping(regs_block_get(reg), reg),
							data, regs_num);
}

int32_t sr_reg_write(uint16_t reg, uint16_t regs_num, int32_t* data)
{
	int32_t ret = -1;
	
	if (reg >= BLOCK_STATE_BASE_ADDR && reg < 0x2D00){ // 动作状态寄存器末端地址 0x2900+0x400 = 0x2D00
		ret = reg_ops.regs_manager_write(&reg_ops, 
								mem_block_remapping(regs_block_get(reg), reg),
								data, regs_num);
		if (ret < 0){
			ret = reg_ops.regs_manager_write(&reg_ops, 
										mem_block_remapping(regs_block_get(reg), reg),
										data, regs_num);
		}
		return ret;
	} else {
		return E_ERROR;
	}
}


