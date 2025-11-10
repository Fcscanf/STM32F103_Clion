//
// Created by fcant on 2025/11/10 星期一.
//

#ifndef STM32F103_CLION_ADC_H
#define STM32F103_CLION_ADC_H

#include "stm32f1xx_hal.h"

void adc_init(void);
uint32_t adc_get_result(void);
void show(void);

#endif //STM32F103_CLION_ADC_H