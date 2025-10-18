//
// Created by fcant on 2025/9/8 星期一.
//

#ifndef STM32F103_CLION_BTIM_H
#define STM32F103_CLION_BTIM_H

#include "stm32f1xx_hal.h"
#include "../LED/led.h"
#include "../KEY/key.h"

void ATIM_TIM8_NPWM_INIT(uint16_t arr, uint16_t psc);

void ATIM_TIM8_NPWM_CHY_SET(uint8_t npwm);

void OUTPUT_NPWM(void);

#endif //STM32F103_CLION_BTIM_H