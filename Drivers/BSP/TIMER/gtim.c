//
// Created by fcant on 2025/9/8 星期一.
//

#include "gtim.h"

#include <stdio.h>

TIM_HandleTypeDef htim2_cnt_cap_chy;

/**
  * @brief 通用定时器通道y 脉冲计数 初始化函数
  * @param None
  * @retval None
  */
void GTIM_TIM2_CAP_CHY_INIT(void) {

    uint16_t arr = 65535;
    uint16_t psc = 0;

    htim2_cnt_cap_chy.Instance = TIM2;
    htim2_cnt_cap_chy.Init.Prescaler = psc; /* 定时器分频 */
    htim2_cnt_cap_chy.Init.CounterMode = TIM_COUNTERMODE_UP; /* 递增计数模式 */
    htim2_cnt_cap_chy.Init.Period = arr; /* 自动重装载值 */
    // htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_IC_Init(&htim2_cnt_cap_chy) != HAL_OK) {
    }

    // 配置定时器的从模式
    TIM_SlaveConfigTypeDef TIM_SlaveConfig_InitStruct = {0};
    TIM_SlaveConfig_InitStruct.SlaveMode = TIM_SLAVEMODE_EXTERNAL1; /* 从模式选择 */
    TIM_SlaveConfig_InitStruct.InputTrigger = TIM_TS_TI1FP1; /* 输入触发源 */
    TIM_SlaveConfig_InitStruct.TriggerPolarity = TIM_TRIGGERPOLARITY_RISING; /* 输入触发极性 */
    TIM_SlaveConfig_InitStruct.TriggerFilter = 0; /* 输入滤波器配置 */
    HAL_TIM_SlaveConfigSynchro(&htim2_cnt_cap_chy, &TIM_SlaveConfig_InitStruct);

    // 使能输入捕获并启动计数器
    HAL_TIM_IC_Start(&htim2_cnt_cap_chy, TIM_CHANNEL_1);

}

/**
  * @brief 定时器 输入捕获 MSP初始化函数
  * @param htim_base: TIM_Base handle pointer
  * @retval None
  */
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef* htim_base)
{
    if(htim_base->Instance==TIM2) {
        GPIO_InitTypeDef GPIO_InitStruct = {0};
        __HAL_RCC_GPIOA_CLK_ENABLE(); /* 开启捕获IO的时钟 */
        __HAL_RCC_TIM2_CLK_ENABLE(); /* 使能TIM2时钟 */
        /*Configure GPIO pin : GPIO_PIN_5 */
        GPIO_InitStruct.Pin = GPIO_PIN_0;
        /* 推挽复用 */
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP; /* 复用推挽输出 */
        GPIO_InitStruct.Pull = GPIO_PULLDOWN; /* 下拉 */
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; /* 高速 */
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    }
}

uint16_t curcnt;
uint16_t oldcnt;
uint8_t key;
uint8_t t = 0;
void PULSE_COUNT(void) {
    key = KEY_SCAN(0);
    if (key == KEY0_PRES) {
        __HAL_TIM_SET_COUNTER(&htim2_cnt_cap_chy, 0);
    }
    curcnt = __HAL_TIM_GET_COUNTER(&htim2_cnt_cap_chy);
    if (oldcnt != curcnt) {
        oldcnt = curcnt;
        printf("CNT:%d\r\n", oldcnt);
    }
    t++;
    if (t > 20) {
        t = 0;
        LED_TogglePin(GPIOB, GPIO_PIN_5);
    }
}
