//
// Created by fcant on 2025/9/8 星期一.
//

#include "atim.h"

#include <stdio.h>

TIM_HandleTypeDef htim1_cplm_pwm = {0};
/* 死区时间设置 */
TIM_BreakDeadTimeConfigTypeDef sbreak_dead_time_config = {0};

/**
  * @brief 高级定时器TIM1 输出比较模式 初始化函数
  * @param None
  * @retval None
  */
void ATIM_TIM1_CPLM_NPWM_INIT(uint16_t arr, uint16_t psc) {

    htim1_cplm_pwm.Instance = TIM1;
    htim1_cplm_pwm.Init.Prescaler = psc; /* 定时器分频 */
    htim1_cplm_pwm.Init.CounterMode = TIM_COUNTERMODE_UP; /* 递增计数模式 */
    htim1_cplm_pwm.Init.Period = arr; /* 自动重装载值 */
    htim1_cplm_pwm.Init.ClockDivision = TIM_CLOCKDIVISION_DIV4; /* CKD[1:0] = 10, tDTS = 4 * tCK_INT = Ft / 4 = 18Mhz */
    /* 输出比较模式初始化 */
    if (HAL_TIM_PWM_Init(&htim1_cplm_pwm) != HAL_OK) {
    }

    // 配置定时器的从模式
    TIM_OC_InitTypeDef tim_oc_cplm_pwm = {0};
    tim_oc_cplm_pwm.OCMode = TIM_OCMODE_PWM1; /* 模式选择 */
    tim_oc_cplm_pwm.OCPolarity = TIM_OCPOLARITY_HIGH; /* OCy 高电平有效 */
    tim_oc_cplm_pwm.OCNPolarity = TIM_OCNPOLARITY_HIGH; /* OCyN 高电平有效 */
    tim_oc_cplm_pwm.OCIdleState = TIM_OCIDLESTATE_RESET; /* 当MOE=0，OCx=0 */
    tim_oc_cplm_pwm.OCNIdleState = TIM_OCNIDLESTATE_RESET; /* 当MOE=0，OCxN=0 */
    HAL_TIM_PWM_ConfigChannel(&htim1_cplm_pwm, &tim_oc_cplm_pwm, TIM_CHANNEL_1);

    /* 设置死区参数，开启死区中断 */
    sbreak_dead_time_config.OffStateRunMode = TIM_OSSR_DISABLE; /* 运行模式的关闭输出状态 */
    sbreak_dead_time_config.OffStateIDLEMode = TIM_OSSI_DISABLE; /* 空闲模式的关闭输出状态 */
    sbreak_dead_time_config.LockLevel = TIM_LOCKLEVEL_OFF; /* 不用寄存器锁功能 */
    sbreak_dead_time_config.BreakState = TIM_BREAK_ENABLE; /* 使能刹车输入 */
    sbreak_dead_time_config.BreakPolarity = TIM_BREAKPOLARITY_HIGH; /* 刹车输入有效信号极性为高 */
    sbreak_dead_time_config.AutomaticOutput = TIM_AUTOMATICOUTPUT_ENABLE; /* 使能AOE位，允许刹车结束后自动恢复输出 */

    HAL_TIM_PWM_Start(&htim1_cplm_pwm, TIM_CHANNEL_1); /* OCy 输出使能 */
    HAL_TIMEx_PWMN_Start(&htim1_cplm_pwm, TIM_CHANNEL_1); /* OCyN 输出使能 */
}

/**
  * @brief 定时器 PWM输出 MSP初始化函数
  * @param htim_base: TIM_Base handle pointer
  * @retval None
  */
void HAL_TIM_OC_MspInit(TIM_HandleTypeDef* htim_base)
{
    if(htim_base->Instance==TIM1) {
        GPIO_InitTypeDef GPIO_InitStruct = {0};
        __HAL_RCC_GPIOE_CLK_ENABLE(); /* 开启捕获IO的时钟 */
        __HAL_RCC_TIM1_CLK_ENABLE(); /* 使能TIM2时钟 */
        /*Configure GPIO pin : GPIO_PIN_6 */
        GPIO_InitStruct.Pin = GPIO_PIN_9;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP; /* 推挽式复用功能 */
        GPIO_InitStruct.Pull = GPIO_PULLDOWN; /* 上拉 */
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; /* 高速 */
        HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_8;
        HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_15;
        HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

        /* 重映射 */
        __HAL_RCC_AFIO_CLK_ENABLE();
        __HAL_AFIO_REMAP_TIM1_ENABLE();
    }
}

/**
* @brief 定时器 TIM1 设置输出比较值 & 死区时间
* @param ccr: 输出比较值
* @param dtg: 死区时间
* @arg dtg[7:5]=0xx 时, 死区时间 = dtg[7:0] * tDTS
* @arg dtg[7:5]=10x 时, 死区时间 = (64 + dtg[6:0]) * 2 * tDTS
* @arg dtg[7:5]=110 时, 死区时间 = (32 + dtg[5:0]) * 8 * tDTS
* @arg dtg[7:5]=111 时, 死区时间 = (32 + dtg[5:0]) * 16 * tDTS
* @note tDTS = 1 / (Ft / CKD[1:0]) = 1 / 18M = 55.56ns
* @retval 无
*/
void ATIM_TIM1_CPLM_SET(uint16_t ccr, uint8_t dtg) {
    __HAL_TIM_SET_COMPARE(&htim1_cplm_pwm, TIM_CHANNEL_1, ccr);
    sbreak_dead_time_config.DeadTime = dtg;
    HAL_TIMEx_ConfigBreakDeadTime(&htim1_cplm_pwm, &sbreak_dead_time_config);
}

uint8_t t = 0;
void WORKING(void) {
    t++;
    if (t > 20) {
        t = 0;
        LED_TogglePin(GPIOB, GPIO_PIN_5);
    }
}
