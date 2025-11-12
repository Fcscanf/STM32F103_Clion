//
// Created by fcant on 2025/11/12 星期三.
//

#ifndef STM32F103_CLION_DAC_H
#define STM32F103_CLION_DAC_H

#include "stm32f1xx_hal.h"

void dac_init(void);
void dac_set_voltage(uint16_t vol);

#endif //STM32F103_CLION_DAC_H