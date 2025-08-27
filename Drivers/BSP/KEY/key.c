//
// Created by fcant on 2025/8/27 ������.
//
#include "key.h"

void KEY_INIT(void){
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
}

uint8_t KEY_SCAN(void) {
    if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4) == GPIO_PIN_RESET) {
        // ����
        HAL_Delay(10);
        if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4) == GPIO_PIN_RESET) {
            // �ȴ������ɿ�
            while (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4) == GPIO_PIN_RESET)
            // ����������
            return 0;
        }
    }
    // ����δ����
    return 1;
}