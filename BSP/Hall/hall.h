#ifndef __HALL_H
#define __HALL_H

#include "main.h"


void Hall_Init(void);

void Hall_EXTI_Callback(uint16_t GPIO_Pin);


uint32_t Hall_GetPulse(void);

uint32_t Hall_GetRPM(void);

float Hall_GetRPS(void);
#endif
