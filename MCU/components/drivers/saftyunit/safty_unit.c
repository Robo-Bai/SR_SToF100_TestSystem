/**
 * Copyright (C) 2019-2021 Standard-robots, Inc
 *
 * @file 	safty_unit.c
 * @author	zyh <zyh@standard-robots.com> 
 * @version V1.0.0
 * @date 	2021年 03月 25日 星期四 10:50:47 CST
 *
 * @brief    safty unit driver.
 */

#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "drivers\include\drivers\sr_serial.h"
#include "drivers\saftyunit\safty_unit.h"


static volatile SaftyUnit_TypeDef su;

static void safty_unit_io_init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | 
    GPIO_Pin_4 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_9 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_5 | GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}

void safty_unit_init(void)
{
    safty_unit_io_init();
    SU_DO_ESTOP_RESET = SU_DIO_LOW;//default low
    SU_DO_ST_ESTOP_TRIG = SU_DIO_HIGH;//default high, not trig
    SU_DO_ESTOP_STATUS = SU_DIO_LOW;//default low,trig
    su.EmReset = EMERGENCY_RESET_DISABLE;
    su.EmState = STATE_EMERGENCY_NA;
	su.EmSrosTrig = EMERGENCY_SROS_TRIG_NONE;
    su.EmSource = 0;
    su.EmRecoverable = EMERGENCY_RECOVERABLE_YES;
    su.EmErrorCode = 0;
    su.EmCmdMonitorFlag = 0;
    su.EmSrosTimeoutFlag = 0;
}

void safty_unit_reset_active(void)
{
	su.EmReset = EMERGENCY_RESET_ENABLE;
}

static void safty_unit_reset_control(uint16_t nms)
{
    static uint8_t s = 255;
    static uint32_t told = 0;
     
    switch(s){
        case 0:{
            told = xTaskGetTickCount();
            SU_DO_ESTOP_RESET = SU_DIO_HIGH;
            s = 1;
        }break;
        case 1:{
            if(get_delta_time(told,xTaskGetTickCount()) >= nms){
                s = 2;
            }
        }break;
        case 2:{
            SU_DO_ESTOP_RESET = SU_DIO_LOW;
            s = 3;
        }break;
        default:break;
    }
    
    if(su.EmReset == EMERGENCY_RESET_ENABLE){
        s = 0;
        su.EmReset = EMERGENCY_RESET_DISABLE;//clear
    }
}

/**
 * @brief 发送安全单元数据
 */
static void send_safty_unit_data(uint8_t msg_type, uint8_t em_satate, 
	uint16_t em_source, uint16_t em_error_code) 
{
	const uint8_t len = 1 + 1 + 2*2;
	uint8_t send_buff[len];
    
	uint8_t send_offset = 0;
	send_buff[0] = msg_type;

	send_offset = 1;
	ENCODE_FIELD(em_satate);
	ENCODE_FIELD(em_source);
    ENCODE_FIELD(em_error_code);
        
	sr_msg_package(send_buff, len);
}


static void safty_unit_send2sros(uint8_t msg_type, uint16_t nms)
{
    static uint8_t s = 0;
    static volatile uint32_t told = 0;
    
    switch(s){
        case 0:{
            told = xTaskGetTickCount();
            s = 1;
        }break;
        case 1:{
            if(get_delta_time(told,xTaskGetTickCount()) >= nms){
                s = 2;
            }
        }break;
        case 2:{
            send_safty_unit_data(msg_type,su.EmState,su.EmSource,su.EmErrorCode);
            s = 0;
        }break;
        default:break;
    }
}


uint8_t source = 0;
void safty_unit_recoverable_check(void)
{
    
    source = (uint8_t)((SU_DI_SOURCE_TK1<<3) | (SU_DI_SOURCE_NXP<<2) |
     (SU_DI_SOURCE_ST<<1) | (SU_DI_SOURCE_ESI4<<0));
    
    if((source & 0X0F) == 0X0F){
        su.EmRecoverable = EMERGENCY_RECOVERABLE_YES;
    } else{
        su.EmRecoverable = EMERGENCY_RECOVERABLE_NO;
    }
}

void safty_unit_state_update(void)
{
    switch(su.EmState){
        case STATE_EMERGENCY_NA:{
            if(SU_DI_ESTOP_OUT_ACK == SU_DIO_HIGH){
                su.EmState = STATE_EMERGENCY_NORMAL;
            } else if(SU_DI_ESTOP_OUT_ACK == SU_DIO_LOW){
                su.EmState = STATE_EMERGENCY_TRIGERED;
            }
        }break;
        case STATE_EMERGENCY_NORMAL:{
            if(SU_DI_ESTOP_OUT_ACK == SU_DIO_LOW){
                su.EmState = STATE_EMERGENCY_TRIGERED;
            }
        }break;
        case STATE_EMERGENCY_TRIGERED:{
            if(su.EmRecoverable == EMERGENCY_RECOVERABLE_YES){
                su.EmState = STATE_EMERGENCY_RECOVERABLE;
            }
        }break;
        case STATE_EMERGENCY_RECOVERABLE:{
            if(SU_DI_ESTOP_OUT_ACK == SU_DIO_HIGH){
				su.EmSource = 0;//clear
                su.EmState = STATE_EMERGENCY_NORMAL;
            } else if(su.EmRecoverable == EMERGENCY_RECOVERABLE_NO){
                su.EmState = STATE_EMERGENCY_TRIGERED;
            }
        }break;
        default:break;
    }
}

void safty_unit_sros_heart_beat_receive(const uint8_t* data)
{
	su.EmCmdMonitorFlag = 1;
	su.EmSrosTick = (uint32_t)((data[3]<<24) | (data[2]<<16) | (data[1]<<8) | (data[0]<<0));
}


void safty_unit_sros_timeout_monitor(uint16_t nms)
{
	static uint8_t s = 0;
    static uint32_t told = 0;
    uint32_t deltaT = 0;
	
	if(su.EmCmdMonitorFlag == 1){
		s = 1;
        told = xTaskGetTickCount();
        su.EmCmdMonitorFlag = 0;
    }
	
	if(s == 1){
		deltaT = get_delta_time(told,xTaskGetTickCount());
		if(deltaT > nms){
			su.EmSrosTimeoutFlag = 1;
		} else{
			su.EmSrosTimeoutFlag = 0;
		}
	} 
}

void safty_unit_sros_trigger_active(void)
{
	su.EmSrosTrig = EMERGENCY_SROS_TRIG_YES;
}
static void safty_unit_sros_trigger(uint16_t nms)
{
	static uint8_t s = 255;
	static uint32_t told = 0;
    uint32_t deltaT = 0;
	
	switch(s){
		case 0:{
			told = xTaskGetTickCount();
			s = 1;
		}break;
		case 1:{
			deltaT = get_delta_time(told,xTaskGetTickCount());
			if(deltaT >= nms){
				su.EmSrosTrig = EMERGENCY_SROS_TRIG_NONE;
				s = 255;
			}
		}break;
		default:break;
	}
	
	if(s == 255){
		if(su.EmSrosTrig == EMERGENCY_SROS_TRIG_YES){
			s = 0;
		}
	}
}

void safty_unit_stmcu_trigger_control(void)
{
    if(su.EmSrosTimeoutFlag == 1){
        SU_DO_ST_ESTOP_TRIG = SU_DIO_LOW;//triger
    } else{
        SU_DO_ST_ESTOP_TRIG = SU_DIO_HIGH;//none triger
    }
	
	if(su.EmSrosTrig == EMERGENCY_SROS_TRIG_YES){
		SU_DO_ST_ESTOP_TRIG = SU_DIO_LOW;//triger
	} else{
		SU_DO_ST_ESTOP_TRIG = SU_DIO_HIGH;//none triger
	}
}

void safty_unit_estop_copy_control(void)
{
    if(SU_DI_ESTOP_OUT_ACK == SU_DIO_HIGH){
        SU_DO_ESTOP_STATUS = SU_DIO_HIGH;
    } else if(SU_DI_ESTOP_OUT_ACK == SU_DIO_LOW){
        SU_DO_ESTOP_STATUS = SU_DIO_LOW;
    }
}

void safty_unit_error_code_update(void)
{
    if(SU_DI_ESTOP_LOGIC != SU_DI_ESTOP_OUT_ACK){
        su.EmErrorCode |= (uint16_t)(1<<0);
    } else{
        su.EmErrorCode &= (uint16_t)(~(1<<0));
    }
    
    if(SU_DI_ESTOP_STATUS != SU_DI_ESTOP_STATUS_ACK){
        su.EmErrorCode |= (uint16_t)(1<<1);
    } else{
        su.EmErrorCode &= (uint16_t)(~(1<<1));
    }
}

void safty_unit_source_update(void)
{
    uint8_t em_source_ex = 0;
    em_source_ex = (uint8_t)((SU_DI_SOURCE_ESI4<<3) | (SU_DI_SOURCE_ESI3<<2) 
    | (SU_DI_SOURCE_ESI2<<1) | (SU_DI_SOURCE_ESI1<<0));
    
    switch(em_source_ex){
        case 0x07:{//0111
            su.EmSource |= (uint16_t)(1<<9);
        }break;
        case 0x03:{//0011
            su.EmSource |= (uint16_t)(1<<0);;
        }break;
        case 0x01:{//0001
            su.EmSource |= (uint16_t)(1<<4);
        }break;
        case 0x00:{//0000
            su.EmSource |= (uint16_t)(1<<8);
        }break;
        default:break;
    }
      
    if(SU_DI_ESTOP_LOGIC == SU_DIO_LOW){
        su.EmSource |= (uint16_t)(1<<10);
    }
    if(SU_DI_SOURCE_NXP == SU_DIO_LOW){
        su.EmSource |= (uint16_t)(1<<11);
    }
    if(SU_DI_SOURCE_ST == SU_DIO_LOW){
        su.EmSource |= (uint16_t)(1<<12);
    }
    if(SU_DI_SOURCE_TK1 == SU_DIO_LOW){
        su.EmSource |= (uint16_t)(1<<13);
    }
	if(su.EmSrosTrig == EMERGENCY_SROS_TRIG_YES){
		su.EmSource |= (uint16_t)(1<<14);
	}
    if(su.EmSrosTimeoutFlag == 1){
        su.EmSource |= (uint16_t)(1<<15);
    }
}

void safty_unit(uint8_t msg_type)
{
    safty_unit_reset_control(SU_RESET_PULSE_PERIOD_MS);
    safty_unit_sros_timeout_monitor(SU_SROS_TIMEOUT_PERIOD_MS);
	safty_unit_sros_trigger(SU_SROS_TRIG_LATCH_PERIOD_MS);
    safty_unit_stmcu_trigger_control();
    safty_unit_recoverable_check();
    safty_unit_source_update();
    safty_unit_state_update();
    safty_unit_error_code_update();
	
    safty_unit_send2sros(msg_type, SU_SEND_DATA_PERIOD_MS);
}

