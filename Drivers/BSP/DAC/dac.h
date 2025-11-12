//
// Created by fcant on 2025/11/12 星期三.
//

#ifndef STM32F103_CLION_DAC_H
#define STM32F103_CLION_DAC_H

#include "stm32f1xx_hal.h"

void dac_init(void);
// void dac_triangular_wave(uint16_t maxval, uint16_t dt, uint16_t samples, uint16_t n);
void dac_triangular_wave_by_key(void);

#endif //STM32F103_CLION_DAC_H