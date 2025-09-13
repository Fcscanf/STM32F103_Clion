//
// Created by fcant on 2025/9/8 星期一.
//

#include "gtim.h"

TIM_HandleTypeDef htim3_pwm_chy;

/**
  * @brief 通用定时器PWM输出初始化函数
  * @param None
  * @retval None
  */
void GTIM_TIM3_PWM_CHY_INIT(void) {

    uint16_t arr = 500 - 1;
    uint16_t psc = 72 - 1;

    htim3_pwm_chy.Instance = TIM3;
    htim3_pwm_chy.Init.Prescaler = psc;
    // htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim3_pwm_chy.Init.Period = arr;
    htim3_pwm_chy.Init.CounterMode = TIM_COUNTERMODE_UP;
    // htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_PWM_Init(&htim3_pwm_chy) != HAL_OK) {
    }

    TIM_OC_InitTypeDef TIM_OC_InitStruct;
    TIM_OC_InitStruct.OCMode = TIM_OCMODE_PWM1;
    TIM_OC_InitStruct.Pulse = arr/2;
    TIM_OC_InitStruct.OCPolarity = TIM_OCPOLARITY_LOW;
    HAL_TIM_PWM_ConfigChannel(&htim3_pwm_chy, &TIM_OC_InitStruct, TIM_CHANNEL_2);

    HAL_TIM_PWM_Start(&htim3_pwm_chy, TIM_CHANNEL_2);

}

/**
  * @brief 定时器输出PWM_MSP初始化函数
  * @param htim_base: TIM_Base handle pointer
  * @retval None
  */
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef* htim_base)
{
    if(htim_base->Instance==TIM3)
    {
        GPIO_InitTypeDef GPIO_InitStruct = {0};
        __HAL_RCC_GPIOB_CLK_ENABLE();
        __HAL_RCC_TIM3_CLK_ENABLE();
        /*Configure GPIO pin : GPIO_PIN_5 */
        GPIO_InitStruct.Pin = GPIO_PIN_5;
        /* 推挽复用 */
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        __HAL_RCC_AFIO_CLK_ENABLE();
        __HAL_AFIO_REMAP_TIM3_PARTIAL();
    }

}

uint16_t ledrpwmval = 0;
uint8_t dir = 1;

/*
 * 调整占空比实现呼吸灯的效果
 */
void TIM3_Compare(void) {
    if (dir) ledrpwmval++;
    else ledrpwmval--;
    if (ledrpwmval > 300) dir = 0;
    if (ledrpwmval == 0) dir = 1;
    /* 修改比较值控制占空比 */
    __HAL_TIM_SET_COMPARE(&htim3_pwm_chy, TIM_CHANNEL_2, ledrpwmval);
}













