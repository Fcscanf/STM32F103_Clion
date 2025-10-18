//
// Created by fcant on 2025/9/8 星期一.
//

#include "atim.h"

#include <stdio.h>

TIM_HandleTypeDef htim8_comp_pwm_chy = {0};

/**
  * @brief 高级定时器TIM8 输出比较模式 初始化函数
  * @param None
  * @retval None
  */
void ATIM_TIM8_NPWM_INIT(uint16_t arr, uint16_t psc) {

    htim8_comp_pwm_chy.Instance = TIM8;
    htim8_comp_pwm_chy.Init.Prescaler = psc; /* 定时器分频 */
    htim8_comp_pwm_chy.Init.CounterMode = TIM_COUNTERMODE_UP; /* 递增计数模式 */
    htim8_comp_pwm_chy.Init.Period = arr; /* 自动重装载值 */
    /* 输出比较模式初始化 */
    if (HAL_TIM_OC_Init(&htim8_comp_pwm_chy) != HAL_OK) {
    }

    // 配置定时器的从模式
    TIM_OC_InitTypeDef TIM_OC_COMP_PWM_InitStruct = {0};
    TIM_OC_COMP_PWM_InitStruct.OCMode = TIM_OCMODE_TOGGLE; /* 模式选择 */
    TIM_OC_COMP_PWM_InitStruct.OCPolarity = TIM_OCPOLARITY_HIGH; /* 输出比较极性为高 */
    HAL_TIM_PWM_ConfigChannel(&htim8_comp_pwm_chy, &TIM_OC_COMP_PWM_InitStruct, TIM_CHANNEL_1);
    HAL_TIM_PWM_ConfigChannel(&htim8_comp_pwm_chy, &TIM_OC_COMP_PWM_InitStruct, TIM_CHANNEL_2);
    HAL_TIM_PWM_ConfigChannel(&htim8_comp_pwm_chy, &TIM_OC_COMP_PWM_InitStruct, TIM_CHANNEL_3);
    HAL_TIM_PWM_ConfigChannel(&htim8_comp_pwm_chy, &TIM_OC_COMP_PWM_InitStruct, TIM_CHANNEL_4);

    __HAL_TIM_ENABLE_OCxPRELOAD(&htim8_comp_pwm_chy, TIM_CHANNEL_1);
    __HAL_TIM_ENABLE_OCxPRELOAD(&htim8_comp_pwm_chy, TIM_CHANNEL_2);
    __HAL_TIM_ENABLE_OCxPRELOAD(&htim8_comp_pwm_chy, TIM_CHANNEL_3);
    __HAL_TIM_ENABLE_OCxPRELOAD(&htim8_comp_pwm_chy, TIM_CHANNEL_4);

    HAL_TIM_OC_Start(&htim8_comp_pwm_chy, TIM_CHANNEL_1);
    HAL_TIM_OC_Start(&htim8_comp_pwm_chy, TIM_CHANNEL_2);
    HAL_TIM_OC_Start(&htim8_comp_pwm_chy, TIM_CHANNEL_3);
    HAL_TIM_OC_Start(&htim8_comp_pwm_chy, TIM_CHANNEL_4);
}

/**
  * @brief 定时器 输出比较 MSP初始化函数
  * @param htim_base: TIM_Base handle pointer
  * @retval None
  */
void HAL_TIM_OC_MspInit(TIM_HandleTypeDef* htim_base)
{
    if(htim_base->Instance==TIM8) {
        GPIO_InitTypeDef GPIO_InitStruct = {0};
        __HAL_RCC_GPIOC_CLK_ENABLE(); /* 开启捕获IO的时钟 */
        __HAL_RCC_TIM8_CLK_ENABLE(); /* 使能TIM2时钟 */
        /*Configure GPIO pin : GPIO_PIN_6 */
        GPIO_InitStruct.Pin = GPIO_PIN_6;
        /* 推挽复用 */
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP; /* 推挽式复用功能 */
        GPIO_InitStruct.Pull = GPIO_NOPULL; /* 上拉 */
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; /* 高速 */
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_6;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_7;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_8;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    }
}

void ATIM_TIM8_SET_COMPARE() {
    __HAL_TIM_SET_COMPARE(&htim8_comp_pwm_chy, TIM_CHANNEL_1, 250-1);
    __HAL_TIM_SET_COMPARE(&htim8_comp_pwm_chy, TIM_CHANNEL_2, 500-1);
    __HAL_TIM_SET_COMPARE(&htim8_comp_pwm_chy, TIM_CHANNEL_3, 750-1);
    __HAL_TIM_SET_COMPARE(&htim8_comp_pwm_chy, TIM_CHANNEL_4, 1000-1);
}

uint8_t t = 0;
void WORKING(void) {
    t++;
    if (t > 20) {
        t = 0;
        LED_TogglePin(GPIOB, GPIO_PIN_5);
    }
}
