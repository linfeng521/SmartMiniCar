/**
  ******************************************************************************
  * @file    motor.c
  * @brief   TB6612 电机驱动实现
  * @note    PWM 频率: 72MHz / 72 / 100 = 10kHz
  ******************************************************************************
  */

#include "motor.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 限制速度在有效 PWM 范围内。
static int Motor_ClampPercent(int value)
{
    
    if (value > MOTOR_PWM_MAX) {
        return MOTOR_PWM_MAX;
    }

    if (value < -MOTOR_PWM_MAX) {
        return -MOTOR_PWM_MAX;
    }

    return value;
}

static void Motor_ToUpperToken(char *token)
{
    // 统一转成大写，方便命令匹配。
    while (*token != '\0') {
        *token = (char)toupper((unsigned char)*token);
        token++;
    }
}

// 初始化电机：先启动 PWM，再让电机停住。
void Motor_Init(void)
{
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
    Motor_StopAll();
}

// 设置某个电机的方向。
static void Motor_SetDirection(Motor_TypeDef motor, Motor_Direction_TypeDef dir)
{
    // 通过 TB6612 的方向引脚控制前进、后退和停止。
    switch (motor) {
    case MOTOR_LEFT:
        switch (dir) {
        case MOTOR_DIR_FORWARD:
					// 1 0正转
            HAL_GPIO_WritePin(MOTOR_AIN1_PORT, MOTOR_AIN1_PIN, GPIO_PIN_SET);
            HAL_GPIO_WritePin(MOTOR_AIN2_PORT, MOTOR_AIN2_PIN, GPIO_PIN_RESET);
            break;

        case MOTOR_DIR_BACKWARD:
					// 0 1反转
            HAL_GPIO_WritePin(MOTOR_AIN1_PORT, MOTOR_AIN1_PIN, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(MOTOR_AIN2_PORT, MOTOR_AIN2_PIN, GPIO_PIN_SET);
            break;

        case MOTOR_DIR_STOP:
					// 0 0 自由停止(惯性滑行）
        default:
            HAL_GPIO_WritePin(MOTOR_AIN1_PORT, MOTOR_AIN1_PIN, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(MOTOR_AIN2_PORT, MOTOR_AIN2_PIN, GPIO_PIN_RESET);
            break;
        }
        break;

    case MOTOR_RIGHT:
        switch (dir) {
        case MOTOR_DIR_FORWARD:
            HAL_GPIO_WritePin(MOTOR_BIN1_PORT, MOTOR_BIN1_PIN, GPIO_PIN_SET);
            HAL_GPIO_WritePin(MOTOR_BIN2_PORT, MOTOR_BIN2_PIN, GPIO_PIN_RESET);
            break;

        case MOTOR_DIR_BACKWARD:
            HAL_GPIO_WritePin(MOTOR_BIN1_PORT, MOTOR_BIN1_PIN, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(MOTOR_BIN2_PORT, MOTOR_BIN2_PIN, GPIO_PIN_SET);
            break;

        case MOTOR_DIR_STOP:
        default:
            HAL_GPIO_WritePin(MOTOR_BIN1_PORT, MOTOR_BIN1_PIN, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(MOTOR_BIN2_PORT, MOTOR_BIN2_PIN, GPIO_PIN_RESET);
            break;
        }
        break;

    default:
        break;
    }
}
// 设置 PWM 占空比。
void Motor_SetPWM(Motor_TypeDef motor, uint16_t pwm)
{
    // 更新对应 PWM 通道的比较值。
    if (pwm > MOTOR_PWM_MAX) {
        pwm = MOTOR_PWM_MAX;
    }

    switch (motor) {
    case MOTOR_LEFT:
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, pwm);
        break;

    case MOTOR_RIGHT:
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, pwm);
        break;

    default:
        break;
    }
}

// 设置电机速度：正数前进，负数后退，0 停止。
void Motor_SetSpeed(Motor_TypeDef motor, int16_t speed)
{
    // 正数表示前进，负数表示后退，0 表示停止。
    speed = (int16_t)Motor_ClampPercent(speed);

    if (speed > 0) {
        Motor_SetDirection(motor, MOTOR_DIR_FORWARD);
        Motor_SetPWM(motor, (uint16_t)speed);
    } else if (speed < 0) {
        Motor_SetDirection(motor, MOTOR_DIR_BACKWARD);
        Motor_SetPWM(motor, (uint16_t)(-speed));
    } else {
        Motor_Stop(motor);
    }
}

// 停止单个电机。

void Motor_Stop(Motor_TypeDef motor)
{
    Motor_SetDirection(motor, MOTOR_DIR_STOP);
    Motor_SetPWM(motor, 0);
}
// 停止两个电机。
void Motor_StopAll(void)
{
    Motor_Stop(MOTOR_LEFT);
    Motor_Stop(MOTOR_RIGHT);
}

// 刹车单个电机。
void Motor_Brake(Motor_TypeDef motor)
{
    // 刹车模式：两个方向引脚都拉高。
    switch (motor) {
    case MOTOR_LEFT:
        HAL_GPIO_WritePin(MOTOR_AIN1_PORT, MOTOR_AIN1_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(MOTOR_AIN2_PORT, MOTOR_AIN2_PIN, GPIO_PIN_SET);
        break;

    case MOTOR_RIGHT:
        HAL_GPIO_WritePin(MOTOR_BIN1_PORT, MOTOR_BIN1_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(MOTOR_BIN2_PORT, MOTOR_BIN2_PIN, GPIO_PIN_SET);
        break;

    default:
        break;
    }

    Motor_SetPWM(motor, 0);
}

void Motor_BrakeAll(void)
{
    Motor_Brake(MOTOR_LEFT);
    Motor_Brake(MOTOR_RIGHT);
}

//前进 speed:0~100
void Motor_Forward(uint16_t speed)
{
    Motor_SetSpeed(MOTOR_LEFT, (int16_t)speed);
    Motor_SetSpeed(MOTOR_RIGHT, (int16_t)speed);
}

////后退 speed:0~-100
void Motor_Backward(uint16_t speed)
{
    Motor_SetSpeed(MOTOR_LEFT, -(int16_t)speed);
    Motor_SetSpeed(MOTOR_RIGHT, -(int16_t)speed);
}

//左转 左轮慢一点，右轮快一点。左轮半速 右轮全速
void Motor_TurnLeft(uint16_t speed)
{
    Motor_SetSpeed(MOTOR_LEFT, (int16_t)(speed / 2U));
    Motor_SetSpeed(MOTOR_RIGHT, (int16_t)speed);
}

// 右转：右轮慢一点，左轮快一点。
void Motor_TurnRight(uint16_t speed)
{
    Motor_SetSpeed(MOTOR_LEFT, (int16_t)speed);
    Motor_SetSpeed(MOTOR_RIGHT, (int16_t)(speed / 2U));
}

// 原地左旋：左轮后退，右轮前进。
void Motor_RotateLeft(uint16_t speed)
{
    Motor_SetSpeed(MOTOR_LEFT, -(int16_t)speed);
    Motor_SetSpeed(MOTOR_RIGHT, (int16_t)speed);
}

// 原地右旋：左轮前进，右轮后退。
void Motor_RotateRight(uint16_t speed)
{
    Motor_SetSpeed(MOTOR_LEFT, (int16_t)speed);
    Motor_SetSpeed(MOTOR_RIGHT, -(int16_t)speed);
}

/*
 * 串口支持的命令格式：
 *   STOP
 *   BRAKE
 *   F 60
 *   B 60
 *   L 60
 *   R 60
 *   RL 60
 *   RR 60
 *   M -30 50
 */
void Motor_ParseCommand(char *cmd)
{
    // 解析一行串口命令并分发到对应电机动作。
    char action[8] = {0};
    int value1 = 0;
    int value2 = 0;
    int parsed = 0;

    if (cmd == NULL) {
        return;
    }

    while ((*cmd == ' ') || (*cmd == '\t')) {
        cmd++;
    }

    if (*cmd == '\0') {
        return;
    }

    parsed = sscanf(cmd, "%7s %d %d", action, &value1, &value2);
    if (parsed <= 0) {
        return;
    }

    Motor_ToUpperToken(action);

    if ((strcmp(action, "STOP") == 0) || (strcmp(action, "S") == 0) || (strcmp(action, "0") == 0)) {
        Motor_StopAll();
        return;
    }

    if ((strcmp(action, "BRAKE") == 0) || (strcmp(action, "BK") == 0) ||
        ((strcmp(action, "B") == 0) && (parsed == 1))) {
        Motor_BrakeAll();
        return;
    }

    if ((strcmp(action, "F") == 0) && (parsed >= 2)) {
        Motor_Forward((uint16_t)abs(value1));
        return;
    }

    if ((strcmp(action, "B") == 0) && (parsed >= 2)) {
        Motor_Backward((uint16_t)abs(value1));
        return;
    }

    if ((strcmp(action, "L") == 0) && (parsed >= 2)) {
        Motor_TurnLeft((uint16_t)abs(value1));
        return;
    }

    if ((strcmp(action, "R") == 0) && (parsed >= 2)) {
        Motor_TurnRight((uint16_t)abs(value1));
        return;
    }

    if ((strcmp(action, "RL") == 0) && (parsed >= 2)) {
        Motor_RotateLeft((uint16_t)abs(value1));
        return;
    }

    if ((strcmp(action, "RR") == 0) && (parsed >= 2)) {
        Motor_RotateRight((uint16_t)abs(value1));
        return;
    }

    if ((strcmp(action, "M") == 0) && (parsed >= 3)) {
        Motor_SetSpeed(MOTOR_LEFT, (int16_t)value1);
        Motor_SetSpeed(MOTOR_RIGHT, (int16_t)value2);
        return;
    }
}

void Motor_TestLeftMotor(uint16_t speed, uint16_t duration_ms)
{
    Motor_SetSpeed(MOTOR_LEFT, (int16_t)speed);
    HAL_Delay(duration_ms);
    Motor_Stop(MOTOR_LEFT);
}

void Motor_TestRightMotor(uint16_t speed, uint16_t duration_ms)
{
    Motor_SetSpeed(MOTOR_RIGHT, (int16_t)speed);
    HAL_Delay(duration_ms);
    Motor_Stop(MOTOR_RIGHT);
}
