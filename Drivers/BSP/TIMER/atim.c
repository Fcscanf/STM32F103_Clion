//
// Created by fcant on 2025/9/8 星期一.
//

#include "atim.h"

TIM_HandleTypeDef htim8_npwm_chy = {0};

static uint8_t npwm_remain = 0;

/**
  * @brief 高级定时器TIM8 通道y 输出指定个数PWM 初始化函数
  * @param None
  * @retval None
  */
void ATIM_TIM8_NPWM_INIT(uint16_t arr, uint16_t psc) {

    htim8_npwm_chy.Instance = TIM8;
    htim8_npwm_chy.Init.Prescaler = psc; /* 定时器分频 */
    htim8_npwm_chy.Init.CounterMode = TIM_COUNTERMODE_UP; /* 递增计数模式 */
    htim8_npwm_chy.Init.Period = arr; /* 自动重装载值 */
    htim8_npwm_chy.Init.RepetitionCounter = 0; /* 重复计数器初始值 */
    if (HAL_TIM_PWM_Init(&htim8_npwm_chy) != HAL_OK) {
    }

    // 配置定时器的从模式
    TIM_OC_InitTypeDef TIM_OC_NPWM_CHY_InitStruct = {0};
    TIM_OC_NPWM_CHY_InitStruct.OCMode = TIM_OCMODE_PWM1; /* 模式选择 PMW 1*/
    TIM_OC_NPWM_CHY_InitStruct.Pulse = arr/2; /* 设置比较值，此值用来确定占空比 */
                                              /* 这里默认设置比较值为自动重装载值的一半,即占空比为50% */
    TIM_OC_NPWM_CHY_InitStruct.OCPolarity = TIM_OCPOLARITY_HIGH; /* 输出比较极性为高 */
    HAL_TIM_PWM_ConfigChannel(&htim8_npwm_chy, &TIM_OC_NPWM_CHY_InitStruct, TIM_CHANNEL_1);

    __HAL_TIM_ENABLE_IT(&htim8_npwm_chy, TIM_IT_UPDATE);
    HAL_TIM_PWM_Start(&htim8_npwm_chy, TIM_CHANNEL_1);

}

/**
  * @brief 定时器 PWM输出 MSP初始化函数
  * @param htim_base: TIM_Base handle pointer
  * @retval None
  */
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef* htim_base)
{
    if(htim_base->Instance==TIM8) {
        GPIO_InitTypeDef GPIO_InitStruct = {0};
        __HAL_RCC_GPIOC_CLK_ENABLE(); /* 开启捕获IO的时钟 */
        __HAL_RCC_TIM8_CLK_ENABLE(); /* 使能TIM2时钟 */
        /*Configure GPIO pin : GPIO_PIN_6 */

        GPIO_InitStruct.Pin = GPIO_PIN_6;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP; /* 推挽式复用功能 */
        GPIO_InitStruct.Pull = GPIO_PULLUP; /* 上拉 */
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; /* 高速 */
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        HAL_NVIC_SetPriority(TIM8_UP_IRQn, 1, 3);
        HAL_NVIC_EnableIRQ(TIM8_UP_IRQn);
    }
}

/* 高级定时器TIM8 NPWM设置PWM个数函数 */
void ATIM_TIM8_NPWM_CHY_SET(uint8_t npwm) {
    if (npwm == 0) return;
    npwm_remain = npwm;
    HAL_TIM_GenerateEvent(&htim8_npwm_chy, TIM_EVENTSOURCE_UPDATE);
    __HAL_TIM_ENABLE(&htim8_npwm_chy);
}

/* 定时器8中断服务函数 */
void TIM8_UP_IRQHandler(void) {
    HAL_TIM_IRQHandler(&htim8_npwm_chy);
}

/* 定时器更新中断回调函数 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM8) {
        if (npwm_remain) {
            TIM8->RCR = npwm_remain - 1;
            HAL_TIM_GenerateEvent(&htim8_npwm_chy, TIM_EVENTSOURCE_UPDATE);
            __HAL_TIM_ENABLE(&htim8_npwm_chy);
            npwm_remain = 0;
        } else {
            /* 寄存器的关闭操作方式 */
            TIM8->CR1 &= ~(1 << 0);
            /* HAL库的关闭方式 */
            // __HAL_TIM_DISABLE(&htim8_npwm_chy);
        }
    }
}
