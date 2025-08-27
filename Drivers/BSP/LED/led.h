//
// Created by fcant on 2025/8/27 ÐÇÆÚÈý.
//

#ifndef STM32F103_CLION_LED_H
#define STM32F103_CLION_LED_H

#include "stm32f1xx_hal.h"

void LED_INIT(void);
void LED_TogglePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

#endif //STM32F103_CLION_LED_H