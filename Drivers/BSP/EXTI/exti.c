//
// Created by fcant on 2025/8/28 ������.
//

#include "exti.h"

void EXTI_INIT(void){
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /*Configure GPIO pin Output Level */

    /*Configure GPIO pin : GPIO_PIN_5 */
    GPIO_InitStruct.Pin = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    HAL_NVIC_SetPriority(EXTI4_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(EXTI4_IRQn);
}

// ע�ṫ��������-��.s�ļ���������
void EXTI4_IRQHandler(void) {
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);
    // ����жϱ�ʶ����ֹ�������ֶ��󴥷�
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_4);
}

// �ڻص������ж����û������߼�
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    // ��Ӣ��������������
    // HAL_Delay(20);
    if (GPIO_Pin == GPIO_PIN_4) {
        if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4) == GPIO_PIN_RESET) {
            LED_TogglePin(GPIOB, GPIO_PIN_5);
        }
    }
}
