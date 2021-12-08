/**
 * Copyright (C) 2016-2021 Standard-robots, Inc
 *
 * @file 	drv_hwtimer.c
 * @author	Neethan <lijun@standard-robots.com> / <neethan@foxmail.com>
 * @date 	2019年 06月 19日 星期三 10:50:47 CST
 *
 * @brief   driver hwtimer 
 */

#include "drivers\include\board.h"
#include "drivers\include\drivers\drv_uart.h"
#include "drivers\include\drivers\drv_hwtimer.h"



static volatile uint32_t _sys_tick_ms = 0;
static TIM_HandleTypeDef TIM3_Handler;      //定时器句柄 
static TIM_HandleTypeDef TIM2_Handler;      //定时器句柄 


/*
 * 通用定时器3中断初始化
 * 定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
 * Ft=定时器工作频率,单位:Mhz
 * 定时器3!(定时器3挂在APB1上，时钟为HCLK/2)
 *
 * @param[in]	arr:自动重装值
  * @param[in]	psc:时钟预分频数
 * @param[out]	None
 *
 * @return		0成功，其他失败
 */
void tim3_init(uint16_t arr, uint16_t psc)
{  
    TIM3_Handler.Instance=TIM3;                          //通用定时器3
    TIM3_Handler.Init.Prescaler=psc;                     //分频系数
    TIM3_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
    TIM3_Handler.Init.Period=arr;                        //自动装载值
    TIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
    HAL_TIM_Base_Init(&TIM3_Handler);
    
    HAL_TIM_Base_Start_IT(&TIM3_Handler); //使能定时器3和定时器3更新中断：TIM_IT_UPDATE   
}

void tim2_init(uint16_t arr, uint16_t psc)
{  
    TIM2_Handler.Instance=TIM2;                          //通用定时器2
    TIM2_Handler.Init.Prescaler=psc;                     //分频系数
    TIM2_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
    TIM2_Handler.Init.Period=arr;                        //自动装载值
    TIM2_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
    HAL_TIM_Base_Init(&TIM2_Handler);
    
    HAL_TIM_Base_Start_IT(&TIM2_Handler); //使能定时器2和定时器2更新中断：TIM_IT_UPDATE   
}

//定时器底册驱动，开启时钟，设置中断优先级
//此函数会被HAL_TIM_Base_Init()函数调用
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM3) {
		__HAL_RCC_TIM3_CLK_ENABLE();            //使能TIM3时钟
		HAL_NVIC_SetPriority(TIM3_IRQn,3,0);    //设置中断优先级，抢占优先级3，子优先级0
		HAL_NVIC_EnableIRQ(TIM3_IRQn);          //开启ITM3中断   
        return;
	}

    if(htim->Instance == TIM2)
	{
		__HAL_RCC_TIM2_CLK_ENABLE();            //使能TIM2时钟
		HAL_NVIC_SetPriority(TIM2_IRQn,5,0);    //设置中断优先级，抢占优先级5，子优先级0
		HAL_NVIC_EnableIRQ(TIM2_IRQn);          //开启ITM2中断   
        return;
	}
}

//定时器3中断服务函数
void TIM3_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM3_Handler);
}

//定时器2中断服务函数
void TIM2_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM2_Handler);
}

//回调函数，定时器中断服务函数调用
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim==(&TIM3_Handler)){
        _sys_tick_ms++;
    }

    if (htim==(&TIM2_Handler)){
        sr_bsp_dev_toggle(SR_LED_RUN);
    }
}

uint32_t system_time_tick(void)
{
	return _sys_tick_ms;
}

// TIM1、TIM8~TIM11的时钟为APB2时钟的两倍即168M，TIM2~TIM7、TIM12~TIM14的
// 时钟为APB1的时钟的两倍即84M
int32_t sr_hw_timer_init(void)
{
    // 用于系统心跳
	tim2_init(10000-1, 8400-1);		///< 1s
	                                                                     
	// tim5_init(10-1,8400-1);		///< 1ms

    tim3_init(10-1,8400-1);       // 分频系数为8400-1，定时器3的频率为84M/8400=10K
                                  // 自动重装载为10-1，那么定时器周期就是(1/10k)*1000 = 0.001s

	return 0;
}

