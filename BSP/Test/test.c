/**
  ******************************************************************************
  * @file    test.c
  * @brief   模块化测试实现
  ******************************************************************************
  */

#include "test.h"
#include "motor.h"
#include "encoder.h"
#include "hcsr04.h"
#include <stdio.h>
#include <string.h>

#define TEST_SPEED 50
#define TEST_DURATION 2000

void Test_Init(void)
{
    printf("\r\n========================================\r\n");
    printf("  SmartMiniCar 模块测试系统\r\n");
    printf("========================================\r\n");
}

void Test_Motor_Left(void)
{
    printf("\r\n[测试] 左电机测试\r\n");
    printf("左电机将以 %d%% 速度运行 %d ms\r\n", TEST_SPEED, TEST_DURATION);
    printf("请观察：应该是【左侧】电机转动\r\n");

    Motor_TestLeftMotor(TEST_SPEED, TEST_DURATION);

    printf("左电机测试完成\r\n");
}

void Test_Motor_Right(void)
{
    printf("\r\n[测试] 右电机测试\r\n");
    printf("右电机将以 %d%% 速度运行 %d ms\r\n", TEST_SPEED, TEST_DURATION);
    printf("请观察：应该是【右侧】电机转动\r\n");

    Motor_TestRightMotor(TEST_SPEED, TEST_DURATION);

    printf("右电机测试完成\r\n");
}

void Test_Motor_Forward(void)
{
    printf("\r\n[测试] 前进测试\r\n");
    printf("小车将以 %d%% 速度前进 %d ms\r\n", TEST_SPEED, TEST_DURATION);
    printf("请观察：小车应该【前进】\r\n");

    Motor_Forward(TEST_SPEED);
    HAL_Delay(TEST_DURATION);
    Motor_StopAll();

    printf("前进测试完成\r\n");
}

void Test_Motor_Backward(void)
{
    printf("\r\n[测试] 后退测试\r\n");
    printf("小车将以 %d%% 速度后退 %d ms\r\n", TEST_SPEED, TEST_DURATION);
    printf("请观察：小车应该【后退】\r\n");

    Motor_Backward(TEST_SPEED);
    HAL_Delay(TEST_DURATION);
    Motor_StopAll();

    printf("后退测试完成\r\n");
}

void Test_Motor_TurnLeft(void)
{
    printf("\r\n[测试] 左转测试\r\n");
    printf("小车将以 %d%% 速度左转 %d ms\r\n", TEST_SPEED, TEST_DURATION);
    printf("请观察：小车应该【向左转弯】\r\n");

    Motor_TurnLeft(TEST_SPEED);
    HAL_Delay(TEST_DURATION);
    Motor_StopAll();

    printf("左转测试完成\r\n");
}

void Test_Motor_TurnRight(void)
{
    printf("\r\n[测试] 右转测试\r\n");
    printf("小车将以 %d%% 速度右转 %d ms\r\n", TEST_SPEED, TEST_DURATION);
    printf("请观察：小车应该【向右转弯】\r\n");

    Motor_TurnRight(TEST_SPEED);
    HAL_Delay(TEST_DURATION);
    Motor_StopAll();

    printf("右转测试完成\r\n");
}

void Test_Motor_RotateLeft(void)
{
    printf("\r\n[测试] 原地左旋测试\r\n");
    printf("小车将以 %d%% 速度原地左旋 %d ms\r\n", TEST_SPEED, TEST_DURATION);
    printf("请观察：小车应该【原地逆时针旋转】\r\n");

    Motor_RotateLeft(TEST_SPEED);
    HAL_Delay(TEST_DURATION);
    Motor_StopAll();

    printf("原地左旋测试完成\r\n");
}

void Test_Motor_RotateRight(void)
{
    printf("\r\n[测试] 原地右旋测试\r\n");
    printf("小车将以 %d%% 速度原地右旋 %d ms\r\n", TEST_SPEED, TEST_DURATION);
    printf("请观察：小车应该【原地顺时针旋转】\r\n");

    Motor_RotateRight(TEST_SPEED);
    HAL_Delay(TEST_DURATION);
    Motor_StopAll();

    printf("原地右旋测试完成\r\n");
}

void Test_Encoder_Left(void)
{
    printf("\r\n[测试] 左编码器测试\r\n");
    printf("左电机将前进 %d ms，测量编码器计数\r\n", TEST_DURATION);

    Encoder_ResetAll();
    HAL_Delay(100);

    Motor_SetSpeed(MOTOR_LEFT, TEST_SPEED);
    HAL_Delay(TEST_DURATION);
    Motor_Stop(MOTOR_LEFT);

    int16_t count = Encoder_GetDeltaCount(ENCODER_LEFT);
    Encoder_Direction_TypeDef dir = Encoder_GetDirection(ENCODER_LEFT);

    printf("左编码器计数: %d\r\n", count);
    printf("方向: %s\r\n",
           dir == ENCODER_DIR_FORWARD ? "前进" :
           dir == ENCODER_DIR_BACKWARD ? "后退" : "停止");

    if (count > 0) {
        printf("✓ 编码器方向正确（前进为正）\r\n");
    } else if (count < 0) {
        printf("✗ 编码器方向相反（前进为负，需要调整）\r\n");
    } else {
        printf("✗ 编码器无计数（检查接线）\r\n");
    }
}

void Test_Encoder_Right(void)
{
    printf("\r\n[测试] 右编码器测试\r\n");
    printf("右电机将前进 %d ms，测量编码器计数\r\n", TEST_DURATION);

    Encoder_ResetAll();
    HAL_Delay(100);

    Motor_SetSpeed(MOTOR_RIGHT, TEST_SPEED);
    HAL_Delay(TEST_DURATION);
    Motor_Stop(MOTOR_RIGHT);

    int16_t count = Encoder_GetDeltaCount(ENCODER_RIGHT);
    Encoder_Direction_TypeDef dir = Encoder_GetDirection(ENCODER_RIGHT);

    printf("右编码器计数: %d\r\n", count);
    printf("方向: %s\r\n",
           dir == ENCODER_DIR_FORWARD ? "前进" :
           dir == ENCODER_DIR_BACKWARD ? "后退" : "停止");

    if (count > 0) {
        printf("✓ 编码器方向正确（前进为正）\r\n");
    } else if (count < 0) {
        printf("✗ 编码器方向相反（前进为负，需要调整）\r\n");
    } else {
        printf("✗ 编码器无计数（检查接线）\r\n");
    }
}

void Test_Encoder_Both(void)
{
    printf("\r\n[测试] 双编码器同步测试\r\n");
    printf("小车将前进 %d ms，测量两个编码器\r\n", TEST_DURATION);

    Encoder_ResetAll();
    HAL_Delay(100);

    Motor_Forward(TEST_SPEED);
    HAL_Delay(TEST_DURATION);
    Motor_StopAll();

    int16_t left_count = Encoder_GetDeltaCount(ENCODER_LEFT);
    int16_t right_count = Encoder_GetDeltaCount(ENCODER_RIGHT);

    printf("左编码器: %d\r\n", left_count);
    printf("右编码器: %d\r\n", right_count);

    int16_t diff = left_count - right_count;
    float diff_percent = (float)diff / (float)((left_count + right_count) / 2) * 100.0f;

    printf("差异: %d (%.1f%%)\r\n", diff, diff_percent);

    if (left_count > 0 && right_count > 0) {
        printf("✓ 两个编码器方向都正确\r\n");
        if (diff_percent < 10.0f) {
            printf("✓ 两轮速度基本一致\r\n");
        } else {
            printf("⚠ 两轮速度差异较大，可能需要校准\r\n");
        }
    } else {
        printf("✗ 编码器方向有问题\r\n");
    }
}

void Test_HCSR04(void)
{
    printf("\r\n[测试] HC-SR04 超声波测试\r\n");
    printf("将连续测量 10 次距离\r\n");

    for (int i = 0; i < 10; i++) {
        HCSR04_Trigger();
        HAL_Delay(100);

        uint32_t timeout = 0;
        while (!HCSR04_IsDataReady() && timeout < 1000) {
            HAL_Delay(1);
            timeout++;
        }

        if (HCSR04_IsDataReady()) {
            float distance = HCSR04_GetDistanceCm();
            printf("[%d] 距离: %.1f cm\r\n", i + 1, distance);
            HCSR04_ClearDataReady();
        } else {
            printf("[%d] 超时，未收到回波\r\n", i + 1);
        }

        HAL_Delay(100);
    }

    printf("超声波测试完成\r\n");
}

void Test_Motor_Encoder_Sync(void)
{
    printf("\r\n[测试] 电机编码器同步测试\r\n");
    printf("测试电机方向与编码器是否匹配\r\n");

    Encoder_ResetAll();
    HAL_Delay(100);

    printf("\n1. 测试前进...\r\n");
    Motor_Forward(TEST_SPEED);
    HAL_Delay(1000);
    Motor_StopAll();

    int16_t left_fwd = Encoder_GetDeltaCount(ENCODER_LEFT);
    int16_t right_fwd = Encoder_GetDeltaCount(ENCODER_RIGHT);
    printf("   左: %d, 右: %d\r\n", left_fwd, right_fwd);

    HAL_Delay(500);
    Encoder_ResetAll();
    HAL_Delay(100);

    printf("2. 测试后退...\r\n");
    Motor_Backward(TEST_SPEED);
    HAL_Delay(1000);
    Motor_StopAll();

    int16_t left_bwd = Encoder_GetDeltaCount(ENCODER_LEFT);
    int16_t right_bwd = Encoder_GetDeltaCount(ENCODER_RIGHT);
    printf("   左: %d, 右: %d\r\n", left_bwd, right_bwd);

    printf("\n结果分析:\r\n");
    if (left_fwd > 0 && right_fwd > 0 && left_bwd < 0 && right_bwd < 0) {
        printf("✓ 电机和编码器方向完全匹配\r\n");
    } else {
        printf("✗ 发现问题:\r\n");
        if (left_fwd < 0 || right_fwd < 0) {
            printf("  - 前进时编码器为负，编码器接线可能反了\r\n");
        }
        if (left_bwd > 0 || right_bwd > 0) {
            printf("  - 后退时编码器为正，编码器接线可能反了\r\n");
        }
    }
}

void Test_RunModule(Test_Module_TypeDef module)
{
    switch (module) {
        case TEST_MOTOR_LEFT:
            Test_Motor_Left();
            break;
        case TEST_MOTOR_RIGHT:
            Test_Motor_Right();
            break;
        case TEST_MOTOR_FORWARD:
            Test_Motor_Forward();
            break;
        case TEST_MOTOR_BACKWARD:
            Test_Motor_Backward();
            break;
        case TEST_MOTOR_TURN_LEFT:
            Test_Motor_TurnLeft();
            break;
        case TEST_MOTOR_TURN_RIGHT:
            Test_Motor_TurnRight();
            break;
        case TEST_MOTOR_ROTATE_LEFT:
            Test_Motor_RotateLeft();
            break;
        case TEST_MOTOR_ROTATE_RIGHT:
            Test_Motor_RotateRight();
            break;
        case TEST_ENCODER_LEFT:
            Test_Encoder_Left();
            break;
        case TEST_ENCODER_RIGHT:
            Test_Encoder_Right();
            break;
        case TEST_ENCODER_BOTH:
            Test_Encoder_Both();
            break;
        case TEST_HCSR04:
            Test_HCSR04();
            break;
        case TEST_MOTOR_ENCODER_SYNC:
            Test_Motor_Encoder_Sync();
            break;
        case TEST_ALL:
            Test_Motor_Left();
            HAL_Delay(1000);
            Test_Motor_Right();
            HAL_Delay(1000);
            Test_Motor_Forward();
            HAL_Delay(1000);
            Test_Encoder_Both();
            HAL_Delay(1000);
            Test_HCSR04();
            break;
        default:
            printf("未知的测试模块\r\n");
            break;
    }
}

void Test_PrintMenu(void)
{
    printf("\r\n========================================\r\n");
    printf("  测试菜单\r\n");
    printf("========================================\r\n");
    printf("电机测试:\r\n");
    printf("  0 - 测试左电机\r\n");
    printf("  1 - 测试右电机\r\n");
    printf("  2 - 测试前进\r\n");
    printf("  3 - 测试后退\r\n");
    printf("  4 - 测试左转\r\n");
    printf("  5 - 测试右转\r\n");
    printf("  6 - 测试原地左旋\r\n");
    printf("  7 - 测试原地右旋\r\n");
    printf("\r\n编码器测试:\r\n");
    printf("  8 - 测试左编码器\r\n");
    printf("  9 - 测试右编码器\r\n");
    printf("  a - 测试双编码器\r\n");
    printf("\r\n传感器测试:\r\n");
    printf("  b - 测试超声波\r\n");
    printf("\r\n综合测试:\r\n");
    printf("  c - 电机编码器同步测试\r\n");
    printf("  d - 运行所有测试\r\n");
    printf("\r\n  m - 显示菜单\r\n");
    printf("========================================\r\n");
}
