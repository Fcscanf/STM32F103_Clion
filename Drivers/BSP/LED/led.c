//
// Created by fcant on 2025/8/27 ÐÇÆÚÈý.
//
#include "led.h"

void LED_INIT(void){
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);

    /*Configure GPIO pin : GPIO_PIN_5 */
    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void LED_TogglePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
    HAL_GPIO_TogglePin(GPIOx, GPIO_Pin);
}
