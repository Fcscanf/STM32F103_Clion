//
// Created by fcant on 2025/11/12 星期三.
//

#ifndef STM32F103_CLION_DAC_H
#define STM32F103_CLION_DAC_H

#include "stm32f1xx_hal.h"

void pwmdac_init(uint16_t arr, uint16_t psc);
void pwmdac_set_voltage(uint16_t vol);

#endif //STM32F103_CLION_DAC_H