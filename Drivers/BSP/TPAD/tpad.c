//
// Created by fcant on 2025/9/8 星期一.
//

#include "tpad.h"

#include <stdio.h>

TIM_HandleTypeDef htim5_cap_chy = {0};

volatile uint16_t tpad_default_val = 0; /* 空载的时候（没有手按下）计数器需要的时间 */

/**
* @brief 触摸按键输入捕获设置
* @param arr ：自动重装值
* @param psc ：时钟预分频数
* @retval 无
*/
void tpad_tim5_cap_init(uint16_t arr, uint16_t psc) {
    GPIO_InitTypeDef gpio_init_struct = {0};
    __HAL_RCC_GPIOA_CLK_ENABLE();; /* TPAD 引脚 时钟使能 */
    __HAL_RCC_TIM5_CLK_ENABLE(); /* 定时器5 时钟使能 */
    gpio_init_struct.Pin = GPIO_PIN_1; /* 输入捕获的 GPIO 口 */
    gpio_init_struct.Mode = GPIO_MODE_INPUT; /* 输入 */
    gpio_init_struct.Pull = GPIO_PULLDOWN; /* 下拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_MEDIUM; /* 中速 */
    HAL_GPIO_Init(GPIOA, &gpio_init_struct); /* TPAD 引脚下拉输入 */

    htim5_cap_chy.Instance = TIM5; /* 定时器 5 */
    htim5_cap_chy.Init.Prescaler = psc; /* 定时器分频 */
    htim5_cap_chy.Init.CounterMode = TIM_COUNTERMODE_UP; /* 向上计数模式 */
    htim5_cap_chy.Init.Period = arr; /* 自动重装载值 */
    htim5_cap_chy.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; /* 不分频*/
    HAL_TIM_IC_Init(&htim5_cap_chy);

    TIM_IC_InitTypeDef timx_ic_cap_chy = {0};
    timx_ic_cap_chy.ICPolarity = TIM_ICPOLARITY_RISING; /* 上升沿捕获 */
    timx_ic_cap_chy.ICSelection = TIM_ICSELECTION_DIRECTTI; /* 映射 TI1 */
    timx_ic_cap_chy.ICPrescaler = TIM_ICPSC_DIV1; /* 配置输入不分频 */
    timx_ic_cap_chy.ICFilter = 0; /* 配置输入滤波器，不滤波 */
    HAL_TIM_IC_ConfigChannel(&htim5_cap_chy,
                             &timx_ic_cap_chy, TIM_CHANNEL_2); /* 配置 TIM5 通道 2 */
    HAL_TIM_IC_Start(&htim5_cap_chy, TIM_CHANNEL_2); /* 使能输入捕获 */
}

/**
* @brief 复位 TPAD
* @note 将 TPAD 按键看做是一个电容, 当手指按下/不按下时容值有变化
* 该函数将 GPIO 设置成推挽输出, 然后输出 0, 进行放电, 然后再设置
* GPIO 为浮空输入, 等待外部大电阻慢慢充电
* @param 无
* @retval 无
*/
static void tpad_reset(void) {
    GPIO_InitTypeDef gpio_init_struct;
    gpio_init_struct.Pin = GPIO_PIN_1; /* 输入捕获的 GPIO 口 */
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP; /* 推挽输出 */
    gpio_init_struct.Pull = GPIO_PULLUP; /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_MEDIUM; /* 中速 */
    HAL_GPIO_Init(GPIOA, &gpio_init_struct);
    /* TPAD 引脚输出 0, 放电 */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
    HAL_Delay(5);
    htim5_cap_chy.Instance->SR = 0; /* 清除标记 */
    htim5_cap_chy.Instance->CNT = 0; /* 归零 */
    gpio_init_struct.Pin = GPIO_PIN_1; /* 输入捕获的 GPIO 口 */
    gpio_init_struct.Mode = GPIO_MODE_INPUT; /* 输入 */
    gpio_init_struct.Pull = GPIO_NOPULL; /* 浮空 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_MEDIUM; /* 中速 */
    HAL_GPIO_Init(GPIOA, &gpio_init_struct); /* TPAD 引脚浮空输入 */
}

/**
* @brief 得到定时器捕获值
* @note 如果超时, 则直接返回定时器的计数值
* 定义超时时间为: TPAD_ARR_MAX_VAL - 500
* @param 无
* @retval 捕获值/计数值（超时的情况下返回）
*/
static uint16_t tpad_get_val(void) {
    uint32_t flag = (TIM_CHANNEL_2 == TIM_CHANNEL_1)
                        ? TIM_FLAG_CC1
                        : (TIM_CHANNEL_2 == TIM_CHANNEL_2)
                              ? TIM_FLAG_CC2
                              : (TIM_CHANNEL_2 == TIM_CHANNEL_3)
                                    ? TIM_FLAG_CC3
                                    : TIM_FLAG_CC4;

    tpad_reset();
    while (__HAL_TIM_GET_FLAG(&htim5_cap_chy, flag) == RESET) {
        /* 等待通道 CHY 捕获上升沿 */
        if (htim5_cap_chy.Instance->CNT > TPAD_ARR_MAX_VAL - 500) {
            return htim5_cap_chy.Instance->CNT; /* 超时了,直接返回 CNT 的值 */
        }
    }
    return TIM5->CCR2; /* 返回捕获/比较值 */
}

/*
 * @brief 读取n次，取最大值
 * @param n：连续获取到次数
 * @retval n次读数里面读到的最大读数值
 */
uint16_t tpad_get_maxval(uint8_t n) {
    uint16_t temp = 0;
    uint16_t maxval = 0;
    while (n--) {
        temp = tpad_get_val(); /* 得到一次值 */
        if (temp > maxval) {
            maxval = temp;
        }
    }

    return maxval;
}

/**
* @brief 初始化触摸按键
* @param psc : 分频系数(值越小, 越灵敏, 最小值为: 1)
* @retval 0, 初始化成功; 1, 初始化失败;
*/
uint8_t tpad_init(uint16_t psc) {
    uint16_t buf[10];
    uint16_t temp;
    uint8_t j, i;
    tpad_tim5_cap_init(TPAD_ARR_MAX_VAL, psc - 1); /* 以 72/(psc-1)Mhz 的频率计数 */
    for (i = 0; i < 10; i++) /* 连续读取 10 次 */
    {
        buf[i] = tpad_get_val();
        HAL_Delay(10);
    }
    for (i = 0; i < 9; i++) /* 排序 */
    {
        for (j = i + 1; j < 10; j++) {
            if (buf[i] > buf[j]) /* 升序排列 */
            {
                temp = buf[i];
                buf[i] = buf[j];
                buf[j] = temp;
            }
        }
    }
    temp = 0;
    for (i = 2; i < 8; i++) /* 取中间的 6 个数据进行平均 */
    {
        temp += buf[i];
    }

    tpad_default_val = temp / 6;
    printf("tpad_default_val:%d\r\n", tpad_default_val);
    printf("charging time:%d\r\n", tpad_default_val / (psc - 1));
    if (tpad_default_val > TPAD_ARR_MAX_VAL / 2) {
        return 1; /* 初始化遇到超过 TPAD_ARR_MAX_VAL/2 的数值,不正常! */
    }
    return 0;
}

/**
* @brief 扫描触摸按键
* @param mode ：扫描模式
* @arg 0, 不支持连续触发(按下一次必须松开才能按下一次);
* @arg 1, 支持连续触发(可以一直按下)
* @retval 0, 没有按下; 1, 有按下;
*/
uint8_t tpad_scan(uint8_t mode) {
    static uint8_t keyen = 0; /* 0, 可以开始检测; >0, 还不能开始检测; */
    uint8_t res = 0;
    uint8_t sample = 3; /* 默认采样次数为 3 次 */
    uint16_t rval;
    if (mode) {
        sample = 6; /* 支持连按的时候，设置采样次数为 6 次 */
        keyen = 0; /* 支持连按, 每次调用该函数都可以检测 */
    }
    rval = tpad_get_maxval(sample);
    if (rval > (tpad_default_val + TPAD_GATE_VAL)) {
        /* 大于 tpad_default_val+TPAD_GATE_VAL,有效 */
        if (keyen == 0) {
            res = 1; /* keyen==0, 有效 */
        }
        // printf("r:%d\r\n", rval); /* 输出计数值, 调试的时候才用到 */
        keyen = 3; /* 至少要再过 3 次之后才能按键有效 */
    }
    if (keyen) keyen--;
    return res;
}

uint8_t t = 0;
void working(void) {
    if (tpad_scan(0)) {
        LED_TogglePin(GPIOE, GPIO_PIN_5);
    }
    t++;
    if (t > 20) {
        t = 0;
        LED_TogglePin(GPIOB, GPIO_PIN_5);
    }
}