//
// Created by fcant on 2025/9/8 星期一.
//

#ifndef STM32F103_CLION_ATIM_H
#define STM32F103_CLION_ATIM_H

#include "stm32f1xx_hal.h"
#include "../LED/led.h"
#include "../KEY/key.h"

void ATIM_TIM8_PWMIN_CHY_INIT(void);
void ATIM_TIM8_PWMIN_CHY_RESTART(void);
void WORKING(void);

#endif //STM32F103_CLION_ATIM_H