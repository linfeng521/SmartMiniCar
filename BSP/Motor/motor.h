/**
  ******************************************************************************
  * @file    motor.h
  * @brief   TB6612 motor driver interface
  ******************************************************************************
  */

#ifndef __MOTOR_H__
#define __MOTOR_H__


#include "main.h"
#include "tim.h"

typedef enum {
    MOTOR_LEFT = 0,
    MOTOR_RIGHT = 1
} Motor_TypeDef;

typedef enum {
    MOTOR_DIR_FORWARD = 0,
    MOTOR_DIR_BACKWARD = 1,
    MOTOR_DIR_STOP = 2
} Motor_Direction_TypeDef;

#define MOTOR_PWM_MAX 100
#define MOTOR_PWM_MIN 0

#define MOTOR_AIN1_PIN GPIO_PIN_5
#define MOTOR_AIN1_PORT GPIOB
#define MOTOR_AIN2_PIN GPIO_PIN_4
#define MOTOR_AIN2_PORT GPIOB

#define MOTOR_BIN1_PIN GPIO_PIN_3
#define MOTOR_BIN1_PORT GPIOB
#define MOTOR_BIN2_PIN GPIO_PIN_15
#define MOTOR_BIN2_PORT GPIOA

void Motor_Init(void);
void Motor_SetSpeed(Motor_TypeDef motor, int16_t speed);
void Motor_SetPWM(Motor_TypeDef motor, uint16_t pwm);
void Motor_Stop(Motor_TypeDef motor);
void Motor_StopAll(void);
void Motor_Brake(Motor_TypeDef motor);
void Motor_BrakeAll(void);
void Motor_Forward(uint16_t speed);
void Motor_Backward(uint16_t speed);
void Motor_TurnLeft(uint16_t speed);
void Motor_TurnRight(uint16_t speed);
void Motor_RotateLeft(uint16_t speed);
void Motor_RotateRight(uint16_t speed);
void Motor_ParseCommand(char *cmd);
void Motor_TestLeftMotor(uint16_t speed, uint16_t duration_ms);
void Motor_TestRightMotor(uint16_t speed, uint16_t duration_ms);

#ifdef __cplusplus
}
#endif

#endif
