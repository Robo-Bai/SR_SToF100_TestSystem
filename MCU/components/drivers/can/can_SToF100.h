#ifndef __CAN_STOF100_H
#define __CAN_STOF100_H

#include "sys.h"
#define CAN_TIME_OUT	37000//10ms,error < 1ms for 72M colock
//u8 CAN_Init(u8 tsjw,u8 tbs2,u8 );

/*#define CAN_BAUDRATE  1000*/    /*  1MBps   */
#define CAN_BAUDRATE  500       /* 500kBps */
/* #define CAN_BAUDRATE  250*/  /* 250kBps */
/* #define CAN_BAUDRATE  125*/  /* 125kBps */
/* #define CAN_BAUDRATE  100*/  /* 100kBps */ 
/* #define CAN_BAUDRATE  50*/   /* 50kBps  */ 
/* #define CAN_BAUDRATE  20*/   /* 20kBps  */ 
/* #define CAN_BAUDRATE  10*/   /* 10kBps  */ 


void CAN_Config(void);
void can1GetID(uint32_t id);
void can1GetPdoT(uint16_t pdo);
u8 CAN_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode);
uint8_t CAN1_Response(uint8_t status);
//uint8_t CAN1_Transmit(uint8_t* array);
int32_t test_can_tx(void);
void Action_Response(void);


void SH200_active(void);//SH200¶¯×÷


#endif


