//
// Created by fcant on 2025/11/12 星期三.
//

#ifndef STM32F103_CLION_DAC_H
#define STM32F103_CLION_DAC_H

#include "stm32f1xx_hal.h"

void dac_dma_wave_init(void);
// void dac_dma_wave_enable(uint16_t cndtr, uint16_t arr, uint16_t psc);
void dac_sin_wave_by_key(void);

#endif //STM32F103_CLION_DAC_H