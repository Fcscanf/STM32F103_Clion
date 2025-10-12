//
// Created by fcant on 2025/9/8 星期一.
//

#include "gtim.h"

#include <stdio.h>

TIM_HandleTypeDef htim5_ic_cap_chy;

/**
  * @brief 通用定时器PWM输出初始化函数
  * @param None
  * @retval None
  */
void GTIM_TIM5_CAP_CHY_INIT(void) {

    uint16_t arr = 0XFFFF;
    uint16_t psc = 72 - 1;

    htim5_ic_cap_chy.Instance = TIM5;
    htim5_ic_cap_chy.Init.Prescaler = psc; /* 定时器分频 */
    htim5_ic_cap_chy.Init.CounterMode = TIM_COUNTERMODE_UP; /* 递增计数模式 */
    htim5_ic_cap_chy.Init.Period = arr; /* 自动重装载值 */
    // htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_IC_Init(&htim5_ic_cap_chy) != HAL_OK) {
    }

    TIM_IC_InitTypeDef TIM_IC_InitStruct;
    TIM_IC_InitStruct.ICPolarity = TIM_ICPOLARITY_RISING; /* 上升沿捕获 */
    TIM_IC_InitStruct.ICSelection = TIM_ICSELECTION_DIRECTTI; /* 映射到TI1上 */
    TIM_IC_InitStruct.ICPrescaler = TIM_ICPSC_DIV1; /* 配置输入分频 */
    TIM_IC_InitStruct.ICFilter = 0; /* 配置输入滤波器 */
    HAL_TIM_IC_ConfigChannel(&htim5_ic_cap_chy, &TIM_IC_InitStruct, TIM_CHANNEL_1);

    __HAL_TIM_ENABLE_IT(&htim5_ic_cap_chy, TIM_IT_UPDATE); /* 使能更新中断 */
    HAL_TIM_IC_Start(&htim5_ic_cap_chy, TIM_CHANNEL_1); /* 开始捕获TIM5的 */

}

/**
  * @brief 定时器 输入捕获 MSP初始化函数
  * @param htim_base: TIM_Base handle pointer
  * @retval None
  */
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef* htim_base)
{
    if(htim_base->Instance==TIM5) {
        GPIO_InitTypeDef GPIO_InitStruct = {0};
        __HAL_RCC_GPIOA_CLK_ENABLE(); /* 开启捕获IO的时钟 */
        __HAL_RCC_TIM5_CLK_ENABLE(); /* 使能TIM5时钟 */
        /*Configure GPIO pin : GPIO_PIN_5 */
        GPIO_InitStruct.Pin = GPIO_PIN_0;
        /* 推挽复用 */
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP; /* 复用推挽输出 */
        GPIO_InitStruct.Pull = GPIO_PULLDOWN; /* 下拉 */
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; /* 高数 */
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        HAL_NVIC_SetPriority(TIM5_IRQn, 1, 3); /* 抢占1，子优先级3 */
        HAL_NVIC_EnableIRQ(TIM5_IRQn); /* 开启ITMx中断 */
    }
}

/*
 * 输入捕获状态 tim5_chy_cap_sta
 * [7]  :0,没有成功的捕获;1,成功捕获到一次
 * [6]  :0,还没捕获到高电平;1,已经捕获到高电平
 * [5:0]:捕获高电平后溢出的次数，最多溢出63次，所以最大捕获值 = 63 * 65536 + 65535 = 4194303
 *       注意：为了通用，默认ARR和CCRy都是16位寄存器，对于32位的定时器（如:TIM5），也只按16位使用
 *       按1us的计数频率，最长溢出时间为4194303us，约4.19秒
 *
 *      （说明：正常32位定时器1us计数器溢出时间4294秒）
 */
uint8_t tim5_chy_cap_sta = 0; /* 输入捕获状态 */
uint16_t tim5_chy_cap_val = 0; /* 输入捕获值 */

/* 定时器5中断服务函数 */
void TIM5_IRQHandler(void) {
    HAL_TIM_IRQHandler(&htim5_ic_cap_chy); /* 定时器HAL库处理函数 */
}

/* 定时器输入捕获中断处理回调函数 */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM5) {
        if ((tim5_chy_cap_sta & 0X80) == 0) {
            /* 还没有成功捕获 */
            if (tim5_chy_cap_sta & 0X40) {
                /* 捕获到一个下降沿 */
                tim5_chy_cap_sta |= 0X80; /* 标记成功捕获到一次高电平脉宽 */
                tim5_chy_cap_val = HAL_TIM_ReadCapturedValue(&htim5_ic_cap_chy, TIM_CHANNEL_1); /* 获取当前的捕获值 */
                TIM_RESET_CAPTUREPOLARITY(&htim5_ic_cap_chy, TIM_CHANNEL_1); /* 一定要先清除原来的设置 */
                TIM_SET_CAPTUREPOLARITY(&htim5_ic_cap_chy, TIM_CHANNEL_1, TIM_ICPOLARITY_RISING); /* 配置TIM5通道1上升沿捕获 */
            } else {
                /* 还未开始，第一次捕获上升沿 */
                tim5_chy_cap_sta = 0; /* 清空 */
                tim5_chy_cap_val = 0;
                tim5_chy_cap_sta |= 0X40; /* 标记捕获到了上升沿 */
                __HAL_TIM_DISABLE(&htim5_ic_cap_chy); /* 关闭定时器5 */
                __HAL_TIM_SET_COUNTER(&htim5_ic_cap_chy, 0); /* 定时器5计数器清零 */
                TIM_RESET_CAPTUREPOLARITY(&htim5_ic_cap_chy, TIM_CHANNEL_1); /* 一定要先清除原来的设置 */
                TIM_SET_CAPTUREPOLARITY(&htim5_ic_cap_chy, TIM_CHANNEL_1, TIM_ICPOLARITY_RISING);
                __HAL_TIM_ENABLE(&htim5_ic_cap_chy); /* 使能定时器5 */
            }
        }
    }
}

/* 定时器更新中断回调函数 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM5) {
        if ((tim5_chy_cap_sta & 0X80) == 0) { /* 还未成功捕获 */
            if (tim5_chy_cap_sta & 0X40) { /* 已经捕获到高电平了 */
                if ((tim5_chy_cap_sta & 0X3F) == 0X3F) { /* 高电平太长了 */
                    TIM_RESET_CAPTUREPOLARITY(&htim5_ic_cap_chy, TIM_CHANNEL_1); /* 一定要先清除原来的设置 */
                    TIM_SET_CAPTUREPOLARITY(&htim5_ic_cap_chy, TIM_CHANNEL_1, TIM_ICPOLARITY_RISING); /* 配置TIM5通道1上升沿捕获 */
                    tim5_chy_cap_sta |= 0X80; /* 标记成功捕获了一次 */
                    tim5_chy_cap_val = 0XFFFF;
                } else { /* 累计定时器溢出次数 */
                    tim5_chy_cap_sta++;
                }
            }
        }
    }
}

uint32_t temp = 0;
uint8_t t = 0;
void CAP_IC(void) {
    if (tim5_chy_cap_sta & 0X80) {  /* 成功捕获到一次高电平 */
        temp = tim5_chy_cap_sta & 0X3F;
        temp *= 65535;  /* 溢出时间总和 */
        temp += tim5_chy_cap_val;  /* 得到总的高电平时间 */
        printf("HIGH: %ld us\r\n", temp);  /* 打印总的高电平时间 */
        tim5_chy_cap_sta = 0;  /* 开启下一次捕获 */
    }
    t++;
    if (t > 20) {  /* 200ms进入一次 */
        t = 0;
        LED_TogglePin(GPIOB, GPIO_PIN_5);  /* LED0闪烁，提示程序运行 */
    }
}
