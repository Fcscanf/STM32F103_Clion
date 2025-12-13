//
// Created by fcant on 2025/8/28 星期四.
//

#include "led.h"

void LED_INIT(void){
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /*Configure GPIO pin Output Level */

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOE_CLK_ENABLE();

    /*Configure GPIO pin : GPIO_PIN_5 */
    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    // HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    // HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_SET);
}

void LED_TogglePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
    HAL_GPIO_TogglePin(GPIOx, GPIO_Pin);
}