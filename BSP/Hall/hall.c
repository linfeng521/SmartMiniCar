/**
 ******************************************************************************
 * @file    hall.c
 * @brief   霍尔测速模块驱动
 * @describe 低速测速:测周期，不是数脉冲,一个脉冲对应转子旋转一圈
 ******************************************************************************
 */
#include "hall.h"

static volatile uint32_t hall_pulse = 0;

static uint32_t hall_rpm = 0;

static uint32_t last_tick = 0;

void Hall_Init(void)
{
    hall_pulse = 0;
    hall_rpm = 0;
}

/*
    EXTI中断调用
*/
void Hall_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == GPIO_PIN_0)
    {
        uint32_t now = HAL_GetTick();
				//每来一个脉冲下降沿，直接更新 RPM
        hall_pulse++;

        if(last_tick != 0)
        {
					  //dt 两次下降沿之间的时间,单位ms
            uint32_t dt = now - last_tick;

            if(dt > 0)
            {
							hall_rpm = 60000 / dt;
            }
        }

        last_tick = now;
    }
}


uint32_t Hall_GetPulse(void)
{
    return hall_pulse;
}

// 转每分钟rpm
uint32_t Hall_GetRPM(void)
{
    return hall_rpm;
}

// 转每秒rps
float Hall_GetRPS(void){
	  return hall_rpm/60.0f;
}