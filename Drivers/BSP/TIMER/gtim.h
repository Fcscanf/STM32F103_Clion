//
// Created by fcant on 2025/9/8 星期一.
//

#ifndef STM32F103_CLION_GTIM_H
#define STM32F103_CLION_GTIM_H

#include "stm32f1xx_hal.h"
#include "../LED/led.h"

void GTIM_TIM3_PWM_CHY_INIT(uint16_t arr, uint16_t psc);
void TIM3_Compare(void);

#endif //STM32F103_CLION_GTIM_H