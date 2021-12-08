/**
 * Copyright (C) 2016-2021 Standard-robots, Inc
 *
 * @file 	board.c
 * @author	Neethan <lijun@standard-robots.com> / <neethan@foxmail.com>
 * @version V1.0.0
 * @date 	2021年 03月 29日 星期一 10:50:47 CST
 *
 * @brief   bsp board 
 */

#include "core\include\srtos.h"
#include "core\include\sr_system.h"
#include "drivers\include\board.h"
#include "drivers\include\drivers\drv_hwtimer.h"
#include "drivers\include\drivers\drv_uart.h"
#include "drivers\include\drivers\sr_serial.h"


/* LED - RGB */
#define GPIO_PORT_LED_RGB    	GPIOA	
#define GPIO_PORT_LED_R    		GPIOA			             
#define GPIO_CLK_LED_R 	    	RCC_AHB1Periph_GPIOA		
#define GPIO_PIN_LED_R			GPIO_Pin_5	

#define GPIO_PORT_LED_G    		GPIOA			             
#define GPIO_CLK_LED_G 	    	RCC_AHB1Periph_GPIOA		
#define GPIO_PIN_LED_G			GPIO_Pin_6

#define GPIO_PORT_LED_B    		GPIOA			             
#define GPIO_CLK_LED_B 	    	RCC_AHB1Periph_GPIOA		
#define GPIO_PIN_LED_B			GPIO_Pin_7


static CRC_HandleTypeDef hcrc;


struct sr_bsp_ops
{
	const char* name[8];
	int32_t (*init)(void);
	
};


static int32_t sr_bsp_init(void);


//static RCC_ClocksTypeDef RCC_Clocks;


static struct sr_bsp_ops bsp_ops = 
{
	.name 	= "src",
	.init	= sr_bsp_init,

};



//初始化PB1为输出.并使能时钟	    
//LED IO初始化
static void bsp_led_init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOF_CLK_ENABLE();           //开启GPIOF时钟
	
    GPIO_Initure.Pin=GPIO_PIN_9|GPIO_PIN_10; //PF9,10
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOF,&GPIO_Initure);
	
    HAL_GPIO_WritePin(GPIOF,GPIO_PIN_9,GPIO_PIN_SET);	//PF9置1，默认初始化后灯灭
    HAL_GPIO_WritePin(GPIOF,GPIO_PIN_10,GPIO_PIN_SET);	//PF10置1，默认初始化后灯灭
}

void sr_bsp_dev_open(uint8_t dev)
{
	switch (dev){
		case SR_LED_RUN:
			LED0 = 0;
			break;
	
		case SR_LED_CMD:
			LED1 = 0;
			break;
		default:
			break;
	}
}

void sr_bsp_dev_close(uint8_t dev)
{
	switch (dev){
		case SR_LED_RUN:
			LED0 = 1;
			break;
		case SR_LED_CMD:
			LED1 = 1;
			break;		
		default:
			break;
	}
}

void sr_bsp_dev_toggle(sr_bsp_dev_t dev)
{
	switch (dev){
		case SR_LED_RUN:
			LED0 = !LED0;
			break;
		
		case SR_LED_CMD:
			LED1 = !LED1;
			break;		
		default:
			break;
	}
}

//按键初始化函数
void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOA_CLK_ENABLE();           //开启GPIOA时钟
    __HAL_RCC_GPIOE_CLK_ENABLE();           //开启GPIOE时钟

    
    GPIO_Initure.Pin=GPIO_PIN_0;            //PA0
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      //输入
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //下拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
    
	GPIO_Initure.Pin=GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4; //PE2,3,4
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      //输入
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOE,&GPIO_Initure);
    
}

//下面的方式是通过位带操作方式读取IO
//#define KEY0        PHin(3) //KEY0按键PH3
//#define KEY1        PHin(2) //KEY1按键PH2
//#define KEY2        PCin(13)//KEY2按键PC13
//#define WK_UP       PAin(0) //WKUP按键PA0


//下面的方式是通过直接操作HAL库函数方式读取IO
#define KEY0        HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_4)  //KEY0按键PE4
#define KEY1        HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_3)  //KEY1按键PE3
#define KEY2        HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_2) 	//KEY2按键PE2
#define WK_UP       HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)  //WKUP按键PA0

#define KEY0_PRES 	1
#define KEY1_PRES	2
#define KEY2_PRES	3
#define WKUP_PRES   4

//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//0，没有任何按键按下
//1，WKUP按下 WK_UP
//注意此函数有响应优先级,KEY0>KEY1>KEY2>WK_UP!!
uint8_t KEY_Scan(uint8_t mode)
{
    static uint8_t key_up=1;     //按键松开标志
    if(mode==1)key_up=1;    //支持连按
    if(key_up&&(KEY0==0||KEY1==0||KEY2==0||WK_UP==1))
    {
        delay_ms(10);
        key_up=0;
        if(KEY0==0)       return KEY0_PRES;
        else if(KEY1==0)  return KEY1_PRES;
        else if(KEY2==0)  return KEY2_PRES;
        else if(WK_UP==1) return WKUP_PRES;          
    }else if(KEY0==1&&KEY1==1&&KEY2==1&&WK_UP==0)key_up=1;
    return 0;   //无按键按下
}

#if 0
/*
 * SYSCLK=168000000 Hz
 * HCLK=168000000 Hz
 * PCLK1=42000000 Hz
 * PCLK2=84000000 Hz
 */
static int32_t sr_system_clock_get(void)
{
	RCC_GetClocksFreq(&RCC_Clocks);
	sr_kprintf("sys_clk=%d Hz\r\n", RCC_Clocks.SYSCLK_Frequency);
	sr_kprintf("hclk=%d Hz\r\n", RCC_Clocks.HCLK_Frequency);
	sr_kprintf("pclk1=%d Hz\r\n", RCC_Clocks.PCLK1_Frequency);
	sr_kprintf("pclk2=%d Hz\r\n", RCC_Clocks.PCLK2_Frequency);
	
	return 0;
}
#endif


/**
  * @brief CRC Initialization Function
  * @param None
  * @retval None
  */
static void crc_init(void)
{

  /* USER CODE BEGIN CRC_Init 0 */

  /* USER CODE END CRC_Init 0 */

  /* USER CODE BEGIN CRC_Init 1 */

  /* USER CODE END CRC_Init 1 */
  hcrc.Instance = CRC;
  if (HAL_CRC_Init(&hcrc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CRC_Init 2 */

  /* USER CODE END CRC_Init 2 */

}

static int32_t sr_bsp_init(void)
{

	sr_serial_register();
	sr_hw_timer_init();
	bsp_led_init();

	
	return 0;
}

static int32_t register_bsp(struct sr_bsp_ops *ops)
{
	int32_t ret = -1;

	ret = ops->init();
	if (ret < 0 ){
		sr_kprintf("Failed to bsp init, ret = %d\n", ret);
		return -1;
	}

	return 0;
}

/*
 * 板级硬件初始化
 *
 * @param[in]	None
 * @param[out]	None
 *
 * @return		0成功，其他失败
 */
static int32_t sr_hw_board_init(void)
{
	int32_t ret = -1;
	
	ret = register_bsp(&bsp_ops);
	if (ret < 0 ){
		sr_kprintf("Failed to register_bsp_ops, ret = %d\n", ret);
		return -1;
	}

//	ret = register_bsp_can();
//	if (ret < 0 ){
//		sr_kprintf("Failed to register_can_ops, ret = %d\n", ret);
//		return -2;
//	}	

	return 0;
}



int32_t sr_startup(void)
{
    __disable_irq(); // __set_PRIMASK(1);    //关闭总中断
	
    HAL_Init();                    	//初始化HAL库    
    Stm32_Clock_Init(336,8,2,7);  	//设置时钟,168Mhz
	delay_init(168);               	//初始化延时函数
	crc_init();
	
    // 板级硬件初始化
	if (sr_hw_board_init() < 0){
		return ERROR;
	}
	
    // 打印系统信息（固件版本、sdk 分支、编译日期等）
	//sr_show_version();
	
	return 0;
}

void sr_start_scheduler(void)
{
	__enable_irq();
}
