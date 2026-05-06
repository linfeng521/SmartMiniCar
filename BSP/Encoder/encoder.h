/**
 ******************************************************************************
 * @file    encoder.h
 * @brief   编码器驱动头文件
 ******************************************************************************
 */

#ifndef __ENCODER_H__
#define __ENCODER_H__

#include "main.h"
#include "tim.h"

// 13（电机尾部霍尔传感器线数）*2*34（减速比）
#define ENCODER_PPR 884U

typedef enum
{
    ENCODER_LEFT = 0,
    ENCODER_RIGHT

} Encoder_TypeDef;

typedef enum
{
    ENCODER_DIR_FORWARD = 0,
    ENCODER_DIR_BACKWARD,
    ENCODER_DIR_STOP

} Encoder_Direction_TypeDef;

void Encoder_Init(void);

uint16_t Encoder_GetCount(Encoder_TypeDef encoder);

int16_t Encoder_GetDeltaCount(Encoder_TypeDef encoder);

void Encoder_Reset(Encoder_TypeDef encoder);

void Encoder_ResetAll(void);

Encoder_Direction_TypeDef Encoder_GetDirection(Encoder_TypeDef encoder);

static float Encoder_GetRawRPS(Encoder_TypeDef encoder);

float Encoder_GetRPS(Encoder_TypeDef encoder);
float Encoder_GetRPM(Encoder_TypeDef encoder);

#endif
