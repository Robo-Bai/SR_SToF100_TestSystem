#include "arm_math.h"

#include "bsp\include\drv_rs485.h"
#include "drivers\imu\sr_imu.h"


#define  SR_USART2                   USART2
#define  SR_USART2_CLK               RCC_APB1Periph_USART2
#define  SR_USART2_GPIO_PORT		 GPIOD
#define  SR_USART2_BAUDRATE          115200

#define  SR_USART2_GPIO_CLK          (RCC_AHB1Periph_GPIOD)
#define  SR_USART2_TX_GPIO_PORT      GPIOD   
#define  SR_USART2_TX_GPIO_PIN       GPIO_Pin_5
#define  SR_USART2_RX_GPIO_PORT      GPIOD
#define  SR_USART2_RX_GPIO_PIN       GPIO_Pin_6

#define  SR_USART2_IRQ               USART2_IRQn
#define  SR_USART2_IRQHandler        USART2_IRQHandler


uint8_t RS485_RxCount_T = 0;
uint8_t RS485_RxBuff[100];
uint8_t RS485_RX_Flag =0;
uint8_t RS485_RxLength = 21;
uint8_t RS485_CRC_Buff[50];
uint16_t RS485_CRC = 0;

float electronic_scale_a;
float electronic_scale_b;
float electronic_scale_c;
float electronic_scale_d;
uint8_t electronic_rxok=0;
float flag = 0;


//CRC mothod1 ????·¨
// Compute the MODBUS RTU CRC
uint16_t ModRTU_CRC(uint8_t* buf, int len)
{
	uint16_t crc = 0xFFFF;
	int32_t pos,i;
	
	  for (pos = 0; pos < len; pos++) {
	    crc ^= (uint16_t)buf[pos];        // XOR byte into least sig. byte of crc
	  
	    for(i = 8; i != 0; i--) 
			{    // Loop over each bit
	      if((crc & 0x0001) != 0)					// If the LSB is set 
				{      
	        crc >>= 1;                    // Shift right and XOR 0xA001
	        crc ^= 0xA001;
	      }
	      else                            // Else LSB is not set
	        crc >>= 1;                    // Just shift right
	    }
	  }
  // Note, this number has low and high bytes swapped, so use it accordingly (or swap bytes)
  return crc;  
}

static uint8_t auchCRCLo[] = {
0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4,
0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD,
0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7,
0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE,
0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2,
0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB,
0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91,
0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88,
0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80,
0x40
};  
//??×??úCRC?μ±í
static uint8_t auchCRCHi[] = {
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40
};

uint16_t Modbus_CRC16(uint8_t *Buff_addr, uint16_t len)
{ 
	uint8_t uchCRCHi = 0xFF;             				// CRC??×??úμ?3?ê??ˉ
	uint8_t uchCRCLo = 0xFF;             				// CRCμí×??úμ?3?ê??ˉ
	uint16_t uIndex;                     				// CRC2é±íμ?????

	while (len--){
		uIndex = uchCRCLo ^ *Buff_addr++;      		// ????CRC
		uchCRCLo = uchCRCHi ^ auchCRCHi[uIndex];
		uchCRCHi = auchCRCLo[uIndex];
	}
	return(uchCRCHi <<8 | uchCRCLo);
}



RS485_TypeDef rs485_1;


void rs485_1_RT(uint8_t cmd) 
{
	RS485_1_RE = cmd;
	RS485_1_TE = cmd;
}

/*
 * 初始化rs485（UART2）与tk1通信
 *
 * @param[in]	baudRate：波特率
 * @param[out]	None.
 *
 * @return		0成功，其他失败
 */
int32_t sr_rs485_init(uint32_t baudRate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_AHB1PeriphClockCmd(SR_USART2_GPIO_CLK, ENABLE);
	RCC_APB1PeriphClockCmd(SR_USART2_CLK, ENABLE);

	/* PA8--RE, PA15--TE */	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
#ifndef FULL_DUPLEX
	rs485_1_RT(RS485_R);
#endif
#ifdef FULL_DUPLEX
	RS485_1_RE = 0;
	RS485_1_TE = 1;
#endif

	GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_USART2); //GPIOD5 USART2->TX
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource6,GPIO_AF_USART2); //GPIOD6 USART2->RX 

	GPIO_InitStructure.GPIO_Pin = SR_USART2_TX_GPIO_PIN | SR_USART2_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(SR_USART2_GPIO_PORT, &GPIO_InitStructure);

	if (stCtlPara.emBaseType == Gulf_14 || stCtlPara.emBaseType == Gulf) {
		USART_InitStructure.USART_BaudRate = baudRate;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b; /* USART_WordLength_9b */
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No; 		/* USART_Parity_Even */
	}
	else if (stCtlPara.ActionType == FORK_LIFT) {
		USART_InitStructure.USART_BaudRate = baudRate;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b; /* USART_WordLength_9b */
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No; 		/* USART_Parity_Even */
	}
	else {
		USART_InitStructure.USART_BaudRate = baudRate;
		USART_InitStructure.USART_WordLength = USART_WordLength_9b; /* USART_WordLength_8b */
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_Even; 		/* USART_Parity_No */
	}
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(SR_USART2, &USART_InitStructure);
	
	USART_Cmd(SR_USART2, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
	
	USART_ClearFlag(SR_USART2, USART_FLAG_TC);
	USART_ClearFlag(SR_USART2, USART_FLAG_RXNE);
	USART_ITConfig(SR_USART2, USART_IT_RXNE, ENABLE);

	return 0;
}

void USART2_IRQHandler(void)
{
	uint8_t temp = 0;
	uint8_t i = 0;
	uint16_t x;
	
	if(stCtlPara.emBaseType==Gulf_14||stCtlPara.emBaseType==Gulf)
	{	
		if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET){//????
			USART2->SR &= ~USART_FLAG_RXNE;	//????????
			temp=USART2->DR;	//????????
			x=temp;
			if(rs485_1.RxOK == 0){
				switch(rs485_1.RxCount){
					case 0:{
						if(temp == 0x01){
							rs485_1.RxData[rs485_1.RxCount] = temp;
							rs485_1.RxCount++;
						} else{
							rs485_1.RxCount = 0;
						}
					}break;
					case 1:{
						if(temp == 0x04){
							rs485_1.RxData[rs485_1.RxCount] = temp;
							rs485_1.RxCount++;
						} else{
							rs485_1.RxCount = 0;
						}
					}break;
					case 2:{
						if(temp == 0x04){
							rs485_1.RxData[rs485_1.RxCount] = temp;
							rs485_1.RxCount++;
							RS485_RxCount_T = 0;
							RS485_CRC = 0;
						} else{
							rs485_1.RxCount = 0;
						}
					}break;
					case 3:{
						if(RS485_RxCount_T < 4){
							rs485_1.RxData[rs485_1.RxCount+RS485_RxCount_T] = temp;
							RS485_RxCount_T++;
						} else if(RS485_RxCount_T == 4){
							RS485_CRC += temp;
							RS485_RxCount_T++;
						} else if(RS485_RxCount_T == 5){
							RS485_CRC += temp*256;
							 //x=ModRTU_CRC(rs485_1.RxData,7);
							
							if(RS485_CRC == ModRTU_CRC(rs485_1.RxData,7)){
								rs485_1.RxOK = 1;
								rs485_1.RxLength = 7;
			
							}
							rs485_1.RxCount = 0;
							RS485_RxCount_T = 0;
							
						}	else{
							rs485_1.RxCount = 0;
							RS485_RxCount_T = 0;
						}
					}break;
					default:{
						rs485_1.RxCount = 0;
						rs485_1.RxXor = 0;
						rs485_1.RxXorCount = 0;
						RS485_RxCount_T = 0;
					}break;
				}
			}
		}
	}
	else if(stCtlPara.ActionType == FORK_LIFT)
	{
		if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET){//????
			USART2->SR &= ~USART_FLAG_RXNE;	//????????
			temp=USART2->DR;	//????????
			x=temp;
			if( 1){
				for(uint8_t i=0;i<=6;i++){
					rs485_1.RxData[i]=rs485_1.RxData[i+1];
				}
				rs485_1.RxData[7]=temp;
				switch(rs485_1.RxData[7]){
					case 'A':{
						flag=1.0f;
							for(int j=0;j<=7;j++)
						{
							if(rs485_1.RxData[j]==0x20)
							{
								rs485_1.RxData[j]='0';
							}
							else if(rs485_1.RxData[j]=='-')
							{
								rs485_1.RxData[j]='0';
								flag=-1.0f;
							}
								
						}
							electronic_scale_a=0.0f;
							electronic_scale_a=electronic_scale_a+((int)(rs485_1.RxData[0]-'0'))*1000.0f;
							electronic_scale_a=electronic_scale_a+((int)(rs485_1.RxData[1]-'0'))*100.0f;
							electronic_scale_a=electronic_scale_a+((int)(rs485_1.RxData[2]-'0'))*10.0f;
							electronic_scale_a=electronic_scale_a+((int)(rs485_1.RxData[3]-'0'))*1.0f;

							electronic_scale_a=electronic_scale_a+((int)(rs485_1.RxData[5]-'0'))*0.1f;
							electronic_scale_a=electronic_scale_a+((int)(rs485_1.RxData[6]-'0'))*0.01f;
							electronic_scale_a=flag*electronic_scale_a;

					}
					break;
					case 'B':{
						flag=1.0f;
							for(int j=0;j<=7;j++)
						{
							if(rs485_1.RxData[j]==0x20)
							{
								rs485_1.RxData[j]='0';
							}
							else if(rs485_1.RxData[j]=='-')
							{
								rs485_1.RxData[j]='0';
								flag=-1.0f;
							}
								
						}
							electronic_scale_b=0.0f;
							electronic_scale_b=electronic_scale_b+((int)(rs485_1.RxData[0]-'0'))*1000.0f;
							electronic_scale_b=electronic_scale_b+((int)(rs485_1.RxData[1]-'0'))*100.0f;
							electronic_scale_b=electronic_scale_b+((int)(rs485_1.RxData[2]-'0'))*10.0f;
							electronic_scale_b=electronic_scale_b+((int)(rs485_1.RxData[3]-'0'))*1.0f;

							electronic_scale_b=electronic_scale_b+((int)(rs485_1.RxData[5]-'0'))*0.1f;
							electronic_scale_b=electronic_scale_b+((int)(rs485_1.RxData[6]-'0'))*0.01f;
						electronic_scale_b=flag*electronic_scale_b;
						
					}
					break;
					case 'C':{
						flag=1.0f;
							for(int j=0;j<=7;j++)
						{
							if(rs485_1.RxData[j]==0x20)
							{
								rs485_1.RxData[j]='0';
							}
							else if(rs485_1.RxData[j]=='-')
							{
								rs485_1.RxData[j]='0';
								flag=-1.0f;
							}
								
						}
							electronic_scale_c=0.0f;
							electronic_scale_c=electronic_scale_c+((int)(rs485_1.RxData[0]-'0'))*1000.0f;
							electronic_scale_c=electronic_scale_c+((int)(rs485_1.RxData[1]-'0'))*100.0f;
							electronic_scale_c=electronic_scale_c+((int)(rs485_1.RxData[2]-'0'))*10.0f;
							electronic_scale_c=electronic_scale_c+((int)(rs485_1.RxData[3]-'0'))*1.0f;

							electronic_scale_c=electronic_scale_c+((int)(rs485_1.RxData[5]-'0'))*0.1f;
							electronic_scale_c=electronic_scale_c+((int)(rs485_1.RxData[6]-'0'))*0.01f;
						electronic_scale_c=flag*electronic_scale_c;
						
					}
					break;
					case 'D':{
						flag=1.0f;
							for(int j=0;j<=7;j++)
						{
							if(rs485_1.RxData[j]==0x20)
							{
								rs485_1.RxData[j]='0';
							}
							else if(rs485_1.RxData[j]=='-')
							{
								rs485_1.RxData[j]='0';
								flag=-1.0f;
							}
								
						}
							electronic_scale_d=0.0f;
							electronic_scale_d=electronic_scale_d+((int)(rs485_1.RxData[0]-'0'))*1000.0f;
							electronic_scale_d=electronic_scale_d+((int)(rs485_1.RxData[1]-'0'))*100.0f;
							electronic_scale_d=electronic_scale_d+((int)(rs485_1.RxData[2]-'0'))*10.0f;
							electronic_scale_d=electronic_scale_d+((int)(rs485_1.RxData[3]-'0'))*1.0f;

							electronic_scale_d=electronic_scale_d+((int)(rs485_1.RxData[5]-'0'))*0.1f;
							electronic_scale_d=electronic_scale_d+((int)(rs485_1.RxData[6]-'0'))*0.01f;
						electronic_scale_d=flag*electronic_scale_d;
						
						electronic_rxok=1;
					}
					break;
					default:
					break;

				}
			}
		}
	}
	else
	{
		if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET){//????
		USART2->SR &= ~USART_FLAG_RXNE;	//????????
		temp=USART2->DR;	//????????
		if(RS485_RX_Flag == 1){
			switch(rs485_1.RxCount){
				case 0:{
					if(RS485_RxCount_T < 21)
					{
						rs485_1.RxData[RS485_RxCount_T] = temp;
						RS485_RxCount_T++;
					}
					if(RS485_RxCount_T==21)
					{					
						
						if(rs485_1.RxOK == 0)
						{
							rs485_1.RxOK = 1;
							for(i = 0;i < RS485_RxLength;i++)
							{
								RS485_RxBuff[i] = rs485_1.RxData[i];
							}
						}
						rs485_1.RxCount = 0;
						RS485_RxCount_T = 0;
						RS485_RX_Flag =0;
					}
				}break;
				case 1:{				
				}break;
				case 2:{										
				}break;
				case 3:{					
				}break;
				default:{
					rs485_1.RxCount = 0;
					rs485_1.RxXor = 0;
					rs485_1.RxXorCount = 0;
					RS485_RxCount_T = 0;
				}break;
			}
		}
	}
		
	}

	if(USART_GetITStatus(USART2, USART_IT_TC) != RESET)
	{
		if(rs485_1.TxCount < (rs485_1.TxLength-1))
		{			
			rs485_1.TxCount++;
			USART_SendData(USART2,rs485_1.TxData[rs485_1.TxCount]);			
		}
		else
		{
			USART_ClearFlag(USART2, USART_FLAG_TC);//MUST Clear Flag first before ENABLE
			USART_ClearITPendingBit(USART2,USART_IT_TC);
			USART_ITConfig(USART2, USART_IT_TC, DISABLE);//????
			USART_ClearITPendingBit(USART2,USART_IT_TC);
			USART_ClearFlag(USART2, USART_FLAG_TC);//MUST Clear Flag first before ENABLE
#ifndef FULL_DUPLEX
	rs485_1_RT(RS485_R);
#endif
			rs485_1.TxOK = 1;
			rs485_1.TxCount = 0;
		}
	}

}


/**************************????********************************************
*????:	ser6ReadData(uint8_t* array,uint8_t* length)
*?  ?:	read rs485_1 data
*????:	array-????,?????????????
						length-???????
*******************************************************************************/
uint8_t rs485_1_ReadData(uint8_t* array){
	uint8_t i = 0;
	if(rs485_1.RxOK == 1){
		L14_MoveODO_OK=1;
		for(i=0;i<rs485_1.RxLength;i++){array[i] = rs485_1.RxData[i];}
		rs485_1.RxOK = 0;
		return rs485_1.RxLength;
	} else {
		return 0;
	}
}
//????????,1-??,0-???
uint8_t rs485_1_WriteAvaliable(void)
{
	return rs485_1.TxOK;
}


/**************************????********************************************
*????:	ser6WriteData(uint8_t* array,uint8_t length)
*?  ?:	usart send data
*????:	array-????,???????
						length-????array??????,length <=100
*******************************************************************************/
uint8_t rs485_1_WriteData(uint8_t* array,uint8_t length){
	uint8_t i = 0;
	if((rs485_1_WriteAvaliable() == 1) && (length > 0) && (length <= RS485_MAX_LENGTH)){
#ifndef FULL_DUPLEX
			rs485_1_RT(RS485_T);//
#endif
		for(i=0;i<length;i++){rs485_1.TxData[i] = array[i];}
		rs485_1.TxCount = 0;
		rs485_1.TxLength = length;
		rs485_1.TxOK = 0;
		USART_ClearFlag(USART2, USART_FLAG_TC);//MUST Clear Flag first before ENABLE
		USART_ClearITPendingBit(USART2,USART_IT_TC);
		USART_ITConfig(USART2, USART_IT_TC, ENABLE);//??????
		USART_ClearITPendingBit(USART2,USART_IT_TC);
		USART_ClearFlag(USART2, USART_FLAG_TC);//MUST Clear Flag first before ENABLE
		USART_SendData(USART2,rs485_1.TxData[rs485_1.TxCount]);
		return 1;
	} else {
		return 0;
	}
}

void getPowerData(void)
{
	
	uint8_t cmd[2] = {0xc8,0x37};

	RS485_RX_Flag=1;
	rs485_1_WriteData(cmd,2);
	
}
void getPower1Data(void)
{
	uint8_t cmd[2] = {0xc9,0x36};

		RS485_RX_Flag=1;
		rs485_1_WriteData(cmd,2);
}




float CorrectX;
float CorrectY;
float CorrectQ;
uint8_t PGV_ReceiveOK=0;
uint8_t PGV_ReceiveOK1=0; // 上视PGV的控制位
float delta_x=0;
float delta_y=0;
float LineSlope;

void PGV_Trans(void)
{
	if(stNav.emBaseType==DIFF)
	{
		if(stCtlPara.IMUorNot==TRUE)
		{
			CorrectQ = sr_convert_angle(sr_convert_angle(-PGV_AngleValue*0.1f/180.0f*PI)+LineSlope);
			CorrectX =0.001f*(PGV_XPosition*0.1f*cosf(LineSlope)-PGV_YPosition*0.1f*sinf(LineSlope)+delta_x*1000-140*cosf(stRobot.stPot.ssPosQ));
			CorrectY = 0.001f*(PGV_XPosition*0.1f*sinf(LineSlope)+PGV_YPosition*0.1f*cosf(LineSlope)+delta_y*1000-140*sinf(stRobot.stPot.ssPosQ));				
			Current_Angle=CorrectQ;
		}
		else
		{
			CorrectQ = sr_convert_angle(sr_convert_angle(-PGV_AngleValue*0.1f/180.0f*PI)+LineSlope);
		CorrectX =0.001f*(PGV_XPosition*0.1f*cosf(LineSlope)-PGV_YPosition*0.1f*sinf(LineSlope)+delta_x*1000-140*cosf(stRobot.stPot.ssPosQ));
		CorrectY = 0.001f*(PGV_XPosition*0.1f*sinf(LineSlope)+PGV_YPosition*0.1f*cosf(LineSlope)+delta_y*1000-140*sinf(stRobot.stPot.ssPosQ));				
		ODO_Current_Angle=CorrectQ;
		}
		
		PGV_ReceiveOK =1;
	}
}


int32_t PGV_XPosition;//单位0.1mm
int16_t PGV_YPosition;//单位0.1mm
uint16_t PGV_AngleValue;//单位0.1�?
int32_t PGV_XPosition1; // 上视PGV的数�?
int16_t PGV_YPosition1;
uint16_t PGV_AngleValue1;
uint8_t XPositionFillFlag;
uint8_t YPositionFillFlag;
int32_t PGV_TagNumber;
int32_t PGV_TagNumber1;
uint8_t Tag_Enable;//表示已经�?测到二维�?
int32_t PGV_ColorError;
uint8_t ColorErrorEnable;
uint16_t PGV_AngleValue_Color;
uint8_t YPositionFillFlag_Color;
uint8_t PGV_Addr;
uint8_t NL;
uint8_t Tag_Enable1;
uint16_t PGV_ErrCode;
uint16_t PGV_ErrCode1;
uint8_t UnpperPGVEnable=0;
uint8_t DownPGVEnable=0;



uint8_t PGV_Position=0;
uint8_t PGV_CAN_OK=0;
ST_PGV  RS485UpPGV;
ST_PGV  RS485DownPGV;
//读取pgv的�?��?�现在主要有4种pgv分别为白�?485接口pgv�?
//白光can接口pgv，红光can接口pgv以及自研的pgv�?
//安装位置主要有上视pgv用于对接货架，下视pgv用于二次定位�?
//各项数据保存在st_pgv的结构体中�??
void ReadPGV(void)
{
		uint8_t PGV_Type = 0;		
			if(rs485_1.RxOK ==1){
				rs485_1.RxOK=0;
				PGV_Type = (RS485_RxBuff[0]&0X30)>>4;
				XPositionFillFlag=RS485_RxBuff[2]&0x04;
				YPositionFillFlag=RS485_RxBuff[6]&0x40;
				if(PGV_Type == 0)//下视PGV
				{
					DownPGVEnable=1;//下视pgv的一个状态，用于判断是否有下视pgv�?
					PGV_ErrCode = ((uint16_t)RS485_RxBuff[1]<<8+RS485_RxBuff[0]);
					Tag_Enable = RS485_RxBuff[1]&0x40;//该�?�为64则表示检测到二维码，�?0则表示没有检测到
					XPositionFillFlag=RS485_RxBuff[2]&0x04;
					YPositionFillFlag=RS485_RxBuff[6]&0x40;
					YPositionFillFlag_Color=RS485_RxBuff[6]&0x40;
					ColorErrorEnable = RS485_RxBuff[0]&0x02;//该�?�为0则表示检测到色带，如果不为零则没有检测到�?
					NL=RS485_RxBuff[1]&0x04;
						if(XPositionFillFlag==0)
						{
						PGV_XPosition=(int32_t)(((RS485_RxBuff[2]&0x07)*0x80*0x4000+ RS485_RxBuff[3]*0x4000+ RS485_RxBuff[4] *0x80+RS485_RxBuff[5] )&0x00ffffff);	
						}
						else
						{
						PGV_XPosition=(int32_t)(((RS485_RxBuff[2]&0x07)*0x80*0x4000+RS485_RxBuff[3]*0x4000+ RS485_RxBuff[4] *0x80+RS485_RxBuff[5] )|0xff000000);
						}
						if(YPositionFillFlag==0)
						{
							PGV_YPosition=(int16_t)((RS485_RxBuff[6]*0x80+RS485_RxBuff[7])&0x3fff);
						}
						else
						{
						PGV_YPosition=(int16_t)((RS485_RxBuff[6]*0x80+RS485_RxBuff[7])|0xc000);
						}
						PGV_AngleValue=(uint16_t)((RS485_RxBuff[10]*0x80+RS485_RxBuff[11])&0x3fff);
					PGV_TagNumber=(int32_t)((RS485_RxBuff[14]*0x80*0x4000+RS485_RxBuff[15]*0x4000+ RS485_RxBuff[16] *0x80+RS485_RxBuff[17] )&0x00ffffff);						
					
					if(PGV_TagNumber!=0)				
					{
						PGV_Trans();
					}
					else
					{
						PGV_ReceiveOK =0;//如果没有�?测二维码，或者二维码的id�?0.则该值为0不矫正�??
					}
					StationPrecisionCheck();


					RS485DownPGV.warncode=((uint16_t)RS485_RxBuff[1]<<8+RS485_RxBuff[0]);
					RS485DownPGV.Tag_Enable = RS485_RxBuff[1]&0x40;//该�?�为64则表示检测到二维码，�?0则表示没有检测到
					RS485DownPGV.TagNumber=(int32_t)((RS485_RxBuff[14]*0x80*0x4000+RS485_RxBuff[15]*0x4000+ RS485_RxBuff[16] *0x80+RS485_RxBuff[17] )&0x00ffffff);
					RS485DownPGV.AngleValue=(uint16_t)((RS485_RxBuff[10]*0x80+RS485_RxBuff[11])&0x3fff);
					if(XPositionFillFlag==0)
					{
						RS485DownPGV.XPosition=(int32_t)(((RS485_RxBuff[2]&0x07)*0x80*0x4000+ RS485_RxBuff[3]*0x4000+ RS485_RxBuff[4] *0x80+RS485_RxBuff[5] )&0x00ffffff);	
					}
					else
					{
						RS485DownPGV.XPosition=(int32_t)(((RS485_RxBuff[2]&0x07)*0x80*0x4000+RS485_RxBuff[3]*0x4000+ RS485_RxBuff[4] *0x80+RS485_RxBuff[5] )|0xff000000);
					}
					if(YPositionFillFlag==0)
					{
						RS485DownPGV.YPosition=(int16_t)((RS485_RxBuff[6]*0x80+RS485_RxBuff[7])&0x3fff);
					}
					else
					{
						RS485DownPGV.YPosition=(int16_t)((RS485_RxBuff[6]*0x80+RS485_RxBuff[7])|0xc000);
					}

				}
				else // 上视PGV
				{
					UnpperPGVEnable=1;
					PGV_ErrCode1= ((uint16_t)RS485_RxBuff[1]<<8+RS485_RxBuff[0]);
					Tag_Enable1 = RS485_RxBuff[1]&0x40;//该�?�为64则表示检测到二维码，�?0则表示没有检测到
					if(Tag_Enable1==64)
					{
						if(XPositionFillFlag==0)
						{
						PGV_XPosition1=(int32_t)(((RS485_RxBuff[2]&0x07)*0x80*0x4000+ RS485_RxBuff[3]*0x4000+ RS485_RxBuff[4] *0x80+RS485_RxBuff[5] )&0x00ffffff);	
						}
						else
						{
						PGV_XPosition1=(int32_t)(((RS485_RxBuff[2]&0x07)*0x80*0x4000+RS485_RxBuff[3]*0x4000+ RS485_RxBuff[4] *0x80+RS485_RxBuff[5] )|0xff000000);
						}
						if(YPositionFillFlag==0)
						{
							PGV_YPosition1=(int16_t)((RS485_RxBuff[6]*0x80+RS485_RxBuff[7])&0x3fff);
						}
						else
						{
						PGV_YPosition1=(int16_t)((RS485_RxBuff[6]*0x80+RS485_RxBuff[7])|0xc000);
						}
						PGV_AngleValue1=(uint16_t)((RS485_RxBuff[10]*0x80+RS485_RxBuff[11])&0x3fff);
						PGV_TagNumber1=(int32_t)((RS485_RxBuff[14]*0x80*0x4000+RS485_RxBuff[15]*0x4000+ RS485_RxBuff[16] *0x80+RS485_RxBuff[17] )&0x00ffffff);
							
						if(PGV_TagNumber1 != 0)
						{
							PGV_ReceiveOK1 = 1;
						}
						else
						{
							PGV_ReceiveOK1 = 0;
						}
					}
					else
					{
						PGV_TagNumber1 = 0;
					}
					RS485UpPGV.warncode=((uint16_t)RS485_RxBuff[1]<<8+RS485_RxBuff[0]);
					RS485UpPGV.Tag_Enable = RS485_RxBuff[1]&0x40;//该�?�为64则表示检测到二维码，�?0则表示没有检测到
					RS485UpPGV.TagNumber=(int32_t)((RS485_RxBuff[14]*0x80*0x4000+RS485_RxBuff[15]*0x4000+ RS485_RxBuff[16] *0x80+RS485_RxBuff[17] )&0x00ffffff);
					RS485UpPGV.AngleValue=(uint16_t)((RS485_RxBuff[10]*0x80+RS485_RxBuff[11])&0x3fff);
					if(XPositionFillFlag==0)
					{
						RS485UpPGV.XPosition=(int32_t)(((RS485_RxBuff[2]&0x07)*0x80*0x4000+ RS485_RxBuff[3]*0x4000+ RS485_RxBuff[4] *0x80+RS485_RxBuff[5] )&0x00ffffff);	
					}
					else
					{
						RS485UpPGV.XPosition=(int32_t)(((RS485_RxBuff[2]&0x07)*0x80*0x4000+RS485_RxBuff[3]*0x4000+ RS485_RxBuff[4] *0x80+RS485_RxBuff[5] )|0xff000000);
					}
					if(YPositionFillFlag==0)
					{
						RS485UpPGV.YPosition=(int16_t)((RS485_RxBuff[6]*0x80+RS485_RxBuff[7])&0x3fff);
					}
					else
					{
						RS485DownPGV.YPosition=(int16_t)((RS485_RxBuff[6]*0x80+RS485_RxBuff[7])|0xc000);
					}


				}	
			}
	

			//用于判断pgv是否接收到数据�??
			if(stCtlPara.DownPGV==2||stCtlPara.DownPGV==3)
			{
			if(Tag_Enable==64){
				if(PGV_TagNumber!=0)				
				{
					PGV_Trans();
				}
				else
				{
					PGV_ReceiveOK =0;//如果没有�?测二维码，或者二维码的id�?0.则该值为0不矫正�??
				}
			}
			else 
			{
				PGV_ReceiveOK =0;
			}
			}
			if(stCtlPara.UpperPGV==2||stCtlPara.UpperPGV==3)
			{
				if(Tag_Enable1==64)
				{			
					if(PGV_TagNumber1 != 0)
					{
						PGV_ReceiveOK1 = 1;
					}
					else
					{
						PGV_ReceiveOK1 = 0;
					}
				}
				else
				{
					PGV_TagNumber1 = 0;
				}
			}
		
		
		
		
}

uint8_t GetPGVUpState485(void){
	//首先确认rs485是否过校验，如果没有过校验则表示没有该设备�?�其次，如果过了校验，然后判断错误类型，没有错误则表示正常�??
	uint8_t Return;
	if(UnpperPGVEnable==0){
		Return=DEVICE_NONE;
	}
	else{
		Return=DEVICE_OK;
	}
	return Return;
}
uint8_t GetPGVDownState485(void){
	//首先确认rs485是否过校验，如果没有过校验则表示没有该设备�?�其次，如果过了校验，然后判断错误类型，没有错误则表示正常�??
	uint8_t Return;
	if(DownPGVEnable==0){
		Return=DEVICE_NONE;
	}
	else{
		Return=DEVICE_OK;
	}
	return Return;
}

void requestTrackEncoder(void){
	uint8_t array[8] = {0x01,0x04,0x00,0x44,0x00,0x02,0x31,0xDE};	
	rs485_1_WriteData(array,8);
	
}
void getTrackEncoder(int32_t* enc){
	uint8_t length = 0;
	uint8_t array[10];
	length = rs485_1_ReadData(array);
	if(length != 0){
		*enc = (int32_t)((array[3]<<24)|(array[4]<<16)|(array[5]<<8)|(array[6]));
	}
}

uint8_t StationCheckError;
void StationPrecisionCheck(void)
{
	//Oasis根据下视PGV�?测到达站点后的精度是否满足要�?
	float XError;//单位mm
	float YError;//单位mm
	float AngleError;//单位rad
	XError=stCtlPara.DoubleRollerXError;
	YError=stCtlPara.DoubleRollerYError;
	AngleError=stCtlPara.DoubleRollerAngleError;
	if(Tag_Enable==64&&PGV_TagNumber==0&&stNav.emMcState==MC_DEFAULT)
	{
		if(fabs(PGV_XPosition)*0.1<XError&&fabs(PGV_YPosition)*0.1<YError&&fabs(sr_convert_angle(PGV_AngleValue*0.1f/180.0f*PI))<AngleError)
		{
			StationCheckError=1;
		}
		else 
		{
			StationCheckError=2;
		}
	}
	else 
	{
		StationCheckError=0;
	}
}


float  GetElectronicScale(void)
{
	static float kg;
	static uint32_t time_count;
	time_count++;
	if(electronic_rxok==1)
	{
		time_count=0;
		electronic_rxok=0;
		kg= electronic_scale_a+electronic_scale_b+electronic_scale_c+electronic_scale_d;
		
	}
	if(time_count>200)
	{
		kg=10000;
	}
	return kg;
}



