/**
  ******************************************************************************
  * @file    test.h
  * @brief   模块化测试接口
  ******************************************************************************
  */

#ifndef __TEST_H__
#define __TEST_H__

#include "main.h"

typedef enum {
    TEST_MOTOR_LEFT = 0,
    TEST_MOTOR_RIGHT,
    TEST_MOTOR_FORWARD,
    TEST_MOTOR_BACKWARD,
    TEST_MOTOR_TURN_LEFT,
    TEST_MOTOR_TURN_RIGHT,
    TEST_MOTOR_ROTATE_LEFT,
    TEST_MOTOR_ROTATE_RIGHT,
    TEST_ENCODER_LEFT,
    TEST_ENCODER_RIGHT,
    TEST_ENCODER_BOTH,
    TEST_HCSR04,
    TEST_MOTOR_ENCODER_SYNC,
    TEST_ALL
} Test_Module_TypeDef;

void Test_Init(void);
void Test_Motor_Left(void);
void Test_Motor_Right(void);
void Test_Motor_Forward(void);
void Test_Motor_Backward(void);
void Test_Motor_TurnLeft(void);
void Test_Motor_TurnRight(void);
void Test_Motor_RotateLeft(void);
void Test_Motor_RotateRight(void);
void Test_Encoder_Left(void);
void Test_Encoder_Right(void);
void Test_Encoder_Both(void);
void Test_HCSR04(void);
void Test_Motor_Encoder_Sync(void);
void Test_RunModule(Test_Module_TypeDef module);
void Test_PrintMenu(void);

#endif
