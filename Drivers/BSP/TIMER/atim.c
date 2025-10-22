//
// Created by fcant on 2025/9/8 星期一.
//

#include "atim.h"

#include <stdio.h>

TIM_HandleTypeDef htim8_pwmin_chy = {0};

/**
 * PWM输入状态：
 * 0:没有成功捕获
 * 1:已经成功捕获了
 */
uint8_t tim8chy_pwmin_sta = 0; /* PWM输入状态 */
uint16_t tim8chy_pwmin_psc = 0; /* PWM输入分频系数 */
uint32_t tim8chy_pwmin_hval = 0; /* PWM的高电平脉宽 */
uint32_t tim8chy_pwmin_cval = 0; /* PWM的周期宽度 */

/**
  * @brief PWM输入模式 初始化函数，采样时钟频率为72Mhz，精度约13.8ns
  * @param None
  * @retval None
  */
void ATIM_TIM8_PWMIN_CHY_INIT(void) {

    htim8_pwmin_chy.Instance = TIM8;
    htim8_pwmin_chy.Init.Prescaler = 0; /* 定时器预分频系数 */
    htim8_pwmin_chy.Init.CounterMode = TIM_COUNTERMODE_UP; /* 递增计数模式 */
    htim8_pwmin_chy.Init.Period = 65535; /* 自动重装载值 */
    /* 输入模式初始化 */
    if (HAL_TIM_IC_Init(&htim8_pwmin_chy) != HAL_OK) {
    }

    /* 从模式配置，IT1触发更新 */
    TIM_SlaveConfigTypeDef slave_config = {0};
    slave_config.SlaveMode = TIM_SLAVEMODE_RESET; /* 从模式：复位模式 */
    slave_config.InputTrigger = TIM_TS_TI1FP1; /* 定时器输入触发源：TI1FP1 */
    slave_config.TriggerPolarity = TIM_TRIGGERPOLARITY_RISING; /* 上升沿检测 */
    slave_config.TriggerFilter = 0; /* 不滤波 */
    HAL_TIM_SlaveConfigSynchro(&htim8_pwmin_chy, &slave_config);

    /* IC1捕获，上升沿触发TI1FP1 */
    TIM_IC_InitTypeDef tim_ic_pwmin_chy = {0};
    tim_ic_pwmin_chy.ICPolarity = TIM_ICPOLARITY_RISING; /* 上升沿检测 */
    tim_ic_pwmin_chy.ICSelection = TIM_ICSELECTION_DIRECTTI; /* 选择输入端IC1映射到TI1 */
    tim_ic_pwmin_chy.ICPrescaler = TIM_ICPSC_DIV1; /* 不分频 */
    tim_ic_pwmin_chy.ICFilter = 0; /* 不滤波 */
    HAL_TIM_IC_ConfigChannel(&htim8_pwmin_chy, &tim_ic_pwmin_chy, TIM_CHANNEL_1);

    /* IC2捕获，下降沿触发TI1FP2 */
    tim_ic_pwmin_chy.ICPolarity = TIM_ICPOLARITY_FALLING; /* 下降沿检测 */
    tim_ic_pwmin_chy.ICSelection = TIM_ICSELECTION_INDIRECTTI; /* 选择输入端IC2映射到TI1 */
    HAL_TIM_IC_ConfigChannel(&htim8_pwmin_chy, &tim_ic_pwmin_chy, TIM_CHANNEL_2);

    HAL_TIM_IC_Start_IT(&htim8_pwmin_chy, TIM_CHANNEL_1);
    HAL_TIM_IC_Start(&htim8_pwmin_chy, TIM_CHANNEL_2);
}

/**
  * @brief 定时器 输入捕获 MSP初始化函数
  * @param htim_base: TIM_Base handle pointer
  * @retval None
  */
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef* htim_base) {
    if(htim_base->Instance==TIM8) {
        GPIO_InitTypeDef GPIO_InitStruct = {0};
        __HAL_RCC_GPIOC_CLK_ENABLE(); /* 开启捕获IO的时钟 */
        __HAL_RCC_TIM8_CLK_ENABLE(); /* 使能TIM2时钟 */
        /*Configure GPIO pin : GPIO_PIN_6 */
        GPIO_InitStruct.Pin = GPIO_PIN_6;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP; /* 推挽式复用功能 */
        GPIO_InitStruct.Pull = GPIO_PULLDOWN; /* 下拉 */
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; /* 高速 */
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        /* TIM1/TIM8有独立的输入捕获中断服务函数 */
        HAL_NVIC_SetPriority(TIM8_CC_IRQn, 1, 3);
        HAL_NVIC_EnableIRQ(TIM8_CC_IRQn);
    }
}

/* 定时器8 输入捕获 中断服务函数，仅TIM1/TIM8有这个函数，其他普通定时器没有这个中断服务函数 */
void TIM8_CC_IRQHandler(void) {
    HAL_TIM_IRQHandler(&htim8_pwmin_chy);
}

/* PWM输入模式，重新启动捕获 */
void ATIM_TIM8_PWMIN_CHY_RESTART(void) {
    __ASM volatile("cpsid i"); /* 关闭中断 */
    /* 清零状态，重新开始检测 */
    tim8chy_pwmin_sta = 0;
    tim8chy_pwmin_hval = 0;
    tim8chy_pwmin_cval = 0;
    __ASM volatile("cpsie i"); /* 打开中断 */
}

/* 定时器输入捕获中断处理回调函数 */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM8) {
        if (tim8chy_pwmin_sta == 0) {
            if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1) {
                tim8chy_pwmin_hval = HAL_TIM_ReadCapturedValue(&htim8_pwmin_chy, TIM_CHANNEL_2) + 1 + 1;
                tim8chy_pwmin_cval = HAL_TIM_ReadCapturedValue(&htim8_pwmin_chy, TIM_CHANNEL_1) + 1 + 1;
                tim8chy_pwmin_sta = 1;
            }
        }
    }
}

double ht, ct, f, tpsc;
uint8_t t = 0;
void WORKING(void) {
    t++;
    if (t >= 20) {  /* 每200ms输出一次结果，并闪烁LED0，提示程序运行 */
        if (tim8chy_pwmin_sta) {
            printf("\r\n");
            printf("PWM PSC   : %d\r\n", tim8chy_pwmin_psc); /* 打印分频系数 */
            printf("PWM Hight : %ld\r\n", tim8chy_pwmin_hval); /* 打印高电平脉宽 */
            printf("PWM Cycle : %ld\r\n", tim8chy_pwmin_cval); /* 打印周期 */
            tpsc = ((double)tim8chy_pwmin_psc + 1) / 72; /* 得到PWM采样时钟周期时间 */
            ht = tim8chy_pwmin_hval * tpsc; /* 计算高电平时间 */
            ct = tim8chy_pwmin_cval * tpsc; /* 计算周期长度 */
            f = (1 / ct) * 1000000; /* 计算频率 */
            printf("PWM Hight time : %.3fus\r\n", ht); /* 打印高电平脉宽长度 */
            printf("PWM Cycle time : %.3fus\r\n", ct); /* 打印周期时间长度 */
            printf("PWM Frequency  : %.3fHz\r\n", f); /* 打印频率 */
            ATIM_TIM8_PWMIN_CHY_RESTART(); /* 重启PWM输入检测 */
        }
        t = 0;
        LED_TogglePin(GPIOE, GPIO_PIN_5);
    }
}
