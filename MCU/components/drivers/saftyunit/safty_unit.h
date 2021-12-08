#ifndef __SAFTY_UNIT_H
#define __SAFTY_UNIT_H

#include "bsp\include\drv_common.h"

#define SU_DO_ST_ESTOP_TRIG     PEout(5)
#define SU_DO_ESTOP_RESET       PEout(8)
#define SU_DO_ESTOP_STATUS      PEout(0)

#define SU_DI_ESTOP_STATUS      PEin(0)
#define SU_DI_ESTOP_STATUS_ACK  PEin(14)
#define SU_DI_ESTOP_OUT_ACK     PEin(15)
#define SU_DI_ESTOP_LOGIC       PEin(6)
#define SU_DI_SOURCE_TK1        PEin(9)
#define SU_DI_SOURCE_NXP        PEin(7)
#define SU_DI_SOURCE_ST         PEin(5)
#define SU_DI_SOURCE_ESI4       PEin(4)
#define SU_DI_SOURCE_ESI3       PEin(3)
#define SU_DI_SOURCE_ESI2       PEin(2)
#define SU_DI_SOURCE_ESI1       PEin(1)

#define SU_DIO_HIGH             1
#define SU_DIO_LOW              0

#define SU_RESET_PULSE_PERIOD_MS       200
#define SU_SEND_DATA_PERIOD_MS         200
#define SU_SROS_TRIG_LATCH_PERIOD_MS   100
#define SU_SROS_TIMEOUT_PERIOD_MS      10000


typedef enum {
     STATE_EMERGENCY_NA = 0x00,//not avaliable
     STATE_EMERGENCY_NORMAL = 0x01,//normal
     STATE_EMERGENCY_TRIGERED = 0x02,//trigged
     STATE_EMERGENCY_RECOVERABLE = 0x03,//recoverable
}EmergencyState;

typedef enum {
     EMERGENCY_RESET_DISABLE = 0x00,
     EMERGENCY_RESET_ENABLE = 0x01,
}EmergencyReset;

typedef enum {
     EMERGENCY_RECOVERABLE_NO = 0,
     EMERGENCY_RECOVERABLE_YES = 1,
}EmergencyRecoverable;

typedef enum {
     EMERGENCY_SROS_TRIG_NONE = 0,
     EMERGENCY_SROS_TRIG_YES = 1,
}EmergencySrosTrig;
typedef struct{
	EmergencyReset EmReset;
    EmergencyState EmState;
	EmergencyRecoverable EmRecoverable;
	EmergencySrosTrig EmSrosTrig;
	uint32_t EmSrosTick;
	uint16_t EmSource;
    uint16_t EmErrorCode;
    uint8_t EmCmdMonitorFlag;
    uint8_t EmSrosTimeoutFlag;
}SaftyUnit_TypeDef;

void safty_unit_init(void);
void safty_unit(uint8_t msg_type);
void safty_unit_reset_active(void);
void safty_unit_sros_trigger_active(void);
void safty_unit_sros_heart_beat_receive(const uint8_t* data);


#endif
