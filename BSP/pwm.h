//filename: pwm.h
#ifndef _PWM_H
#define _PWM_H

#include "stm32f10x.h"

void RCC_Configuration(void);
void GPIO_Configuration(void);
void TIM2_Configuration(u16 arr, u16 psc);

#endif
