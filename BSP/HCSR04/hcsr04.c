/**
 ******************************************************************************
 * @file    hcsr04.c
 * @brief   HC-SR04 超声波测距模块驱动
 *
 * 使用方式:
 *   1. 初始化: HCSR04_Init();
 *   2. 触发测量: HCSR04_Trigger();
 *   3. 等待数据就绪: while(!HCSR04_IsDataReady());
 *   4. 读取距离: float dist = HCSR04_GetDistanceCm();
 *   5. 清除标志: HCSR04_ClearDataReady();
 *
 * 或者简单使用:
 *   HCSR04_Trigger();
 *   HAL_Delay(100);  // 等待测量完成
 *   float dist = HCSR04_GetDistanceCm();
 *
 * 注意:
 *   - 测量范围: 2-400cm
 *   - 超出范围返回 -1.0
 *   - 建议测量间隔 > 60ms
 ******************************************************************************
 */

#include "hcsr04.h"

// 上升沿捕获的时间（us）
static volatile uint32_t upEdge = 0;
// 下降沿捕获的时间（us）
static volatile uint32_t downEdge = 0;
// 单位cm
static volatile float distance = -1.0f;
// 数据就绪标志
static volatile uint8_t data_ready = 0;

void HCSR04_Init(void)
{
    HAL_GPIO_WritePin(HCSR04_TRIG_PORT, HCSR04_TRIG_PIN, GPIO_PIN_RESET);
    HAL_TIM_IC_Start(&HCSR04_ECHO_TIMER_HANDLE, TIM_CHANNEL_1);
    HAL_TIM_IC_Start_IT(&HCSR04_ECHO_TIMER_HANDLE, TIM_CHANNEL_2);
    data_ready = 0;
}

void HCSR04_Trigger(void)
{
    data_ready = 0;
    HAL_GPIO_WritePin(HCSR04_TRIG_PORT, HCSR04_TRIG_PIN, GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(HCSR04_TRIG_PORT, HCSR04_TRIG_PIN, GPIO_PIN_RESET);
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM4 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
    {
        upEdge = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
        downEdge = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
        distance = (float)((downEdge - upEdge) * 0.034 / 2);
        if (distance > 400.0f || distance < 2.0f)
        {
            distance = -1.0f;
        }
        data_ready = 1;
    }
}

float HCSR04_GetDistanceCm(void)
{
    return distance;
}

uint8_t HCSR04_IsDataReady(void)
{
    return data_ready;
}

void HCSR04_ClearDataReady(void)
{
    data_ready = 0;
}
