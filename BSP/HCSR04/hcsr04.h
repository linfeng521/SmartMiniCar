#ifndef __HCSR04_H__
#define __HCSR04_H__

#include "main.h"
#include "tim.h"

#define HCSR04_TRIG_PORT GPIOB
#define HCSR04_TRIG_PIN  GPIO_PIN_7

#define HCSR04_ECHO_TIMER           TIM4
#define HCSR04_ECHO_TIMER_HANDLE    htim4
#define HCSR04_ECHO_TIMER_CHANNEL   TIM_CHANNEL_1

void HCSR04_Init(void);
void HCSR04_Trigger(void);
float HCSR04_GetDistanceCm(void);
uint8_t HCSR04_IsDataReady(void);
void HCSR04_ClearDataReady(void);

#endif
