/*
* ******************************************************************************
* @file encoder.c 
* @brief 编码器驱动 
* @describe 使用固定时间窗口计数 
* *****************************************************************************
*/
#include "encoder.h"


static uint16_t left_last_count = 0;
static uint16_t right_last_count = 0;

static int16_t left_last_delta = 0;
static int16_t right_last_delta = 0;

static uint32_t left_last_tick = 0;
static uint32_t right_last_tick = 0;


static uint16_t Encoder_ReadNow(Encoder_TypeDef encoder)
{
	  // 左电机对应编码器
    if (encoder == ENCODER_LEFT)
    {
        return (uint16_t)__HAL_TIM_GET_COUNTER(&htim2);
    }
		// 右电机对应编码器
    return (uint16_t)__HAL_TIM_GET_COUNTER(&htim3);
}


void Encoder_Init(void)
{
    uint32_t now;

		// 启动定时器编码器模式
    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);


		// 重置计数
    __HAL_TIM_SET_COUNTER(&htim2, 0);
    __HAL_TIM_SET_COUNTER(&htim3, 0);


    now = HAL_GetTick();


    left_last_count = 0;
    right_last_count = 0;

    left_last_delta = 0;
    right_last_delta = 0;

    left_last_tick = now;
    right_last_tick = now;
}


uint16_t Encoder_GetCount(Encoder_TypeDef encoder)
{
    return Encoder_ReadNow(encoder);
}


int16_t Encoder_GetDeltaCount(Encoder_TypeDef encoder)
{
    uint16_t now;

    int16_t delta = 0;


    now = Encoder_ReadNow(encoder);


    if (encoder == ENCODER_LEFT)
    {
        delta = (int16_t)(now - left_last_count);

        left_last_count = now;

        left_last_delta = delta;
    }
    else
    {
        delta = (int16_t)(now - right_last_count);

        right_last_count = now;

        right_last_delta = delta;
    }


    return delta;
}


void Encoder_Reset(Encoder_TypeDef encoder)
{
    uint32_t now = HAL_GetTick();


    if (encoder == ENCODER_LEFT)
    {
        __HAL_TIM_SET_COUNTER(&htim2, 0);

        left_last_count = 0;
        left_last_delta = 0;

        left_last_tick = now;

        return;
    }


    __HAL_TIM_SET_COUNTER(&htim3, 0);

    right_last_count = 0;
    right_last_delta = 0;

    right_last_tick = now;
}

// 重置所有编码器计数值和计时
void Encoder_ResetAll(void)
{
    Encoder_Reset(ENCODER_LEFT);

    Encoder_Reset(ENCODER_RIGHT);
}


Encoder_Direction_TypeDef Encoder_GetDirection(Encoder_TypeDef encoder)
{
    int16_t delta;


    delta = (encoder == ENCODER_LEFT)
            ? left_last_delta
            : right_last_delta;


    if (delta > 0)
    {
        return ENCODER_DIR_FORWARD;
    }

    if (delta < 0)
    {
        return ENCODER_DIR_BACKWARD;
    }

    return ENCODER_DIR_STOP;
}

//转每秒rps,Raw → Filtered → Output
static float Encoder_GetRawRPS(Encoder_TypeDef encoder)
{
    int16_t delta;

    uint32_t now_tick;
    uint32_t dt_ms;

    float dt_s;


    delta = Encoder_GetDeltaCount(encoder);


    now_tick = HAL_GetTick();


    if (encoder == ENCODER_LEFT)
    {
        dt_ms = now_tick - left_last_tick;

        left_last_tick = now_tick;
    }
    else
    {
        dt_ms = now_tick - right_last_tick;

        right_last_tick = now_tick;
    }


    if (dt_ms == 0)
    {
        return 0.0f;
    }


    dt_s = (float)dt_ms / 1000.0f;


    return ((float)delta)
            / ((float)ENCODER_PPR * dt_s);
}

// 转每秒rps(等于示波器上频率）
float Encoder_GetRPS(Encoder_TypeDef encoder)
{
	  //TODO:滤波返回
    return Encoder_GetRawRPS(encoder); //直接返回
}


// 返回每分钟转数rpm
float Encoder_GetRPM(Encoder_TypeDef encoder)
{
	  //TODO:滤波返回
    return Encoder_GetRawRPS(encoder) * 60.0f;//直接返回
}


