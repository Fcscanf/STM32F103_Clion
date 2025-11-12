//
// Created by fcant on 2025/11/12 星期三.
//

#include "dac.h"

#include <stdio.h>

#include "../KEY/key.h"

DAC_HandleTypeDef g_dac_handle;

/* DAC初始化函数 */
void dac_init(void) {
    DAC_ChannelConfTypeDef dac_ch_conf;

    g_dac_handle.Instance = DAC;
    HAL_DAC_Init(&g_dac_handle); /* 初始化DAC */

    dac_ch_conf.DAC_Trigger = DAC_TRIGGER_NONE; /* 不使用触发功能 */
    dac_ch_conf.DAC_OutputBuffer = DAC_OUTPUTBUFFER_DISABLE; /* DAC输出缓冲关闭 */

    HAL_DAC_ConfigChannel(&g_dac_handle, &dac_ch_conf, DAC_CHANNEL_1); /* 配置DAC通道1 */
    HAL_DAC_Start(&g_dac_handle, DAC_CHANNEL_1); /* 开启DAC通道1 */
}

/* DAC MSP初始化函数 */
void HAL_DAC_MspInit(DAC_HandleTypeDef *hdac) {
    if (hdac->Instance == DAC) {
        GPIO_InitTypeDef gpio_init_struct;

        __HAL_RCC_DAC_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();

        gpio_init_struct.Pin = GPIO_PIN_4;
        gpio_init_struct.Mode = GPIO_MODE_ANALOG;
        HAL_GPIO_Init(GPIOA, &gpio_init_struct);
    }
}

/**
 * @brief       设置DAC_OUT1输出三角波
 *   @note      输出频率 ≈ 1000 / (dt * samples) Khz, 不过在dt较小的时候,比如小于5us时, 由于delay_us
 *              本身就不准了(调用函数,计算等都需要时间,延时很小的时候,这些时间会影响到延时), 频率会偏小.
 *
 * @param       maxval : 最大值(0 < maxval < 4096), (maxval + 1)必须大于等于samples/2
 * @param       dt     : 每个采样点的延时时间(单位: us)
 * @param       samples: 采样点的个数, samples必须小于等于(maxval + 1) * 2 , 且maxval不能等于0
 * @param       n      : 输出波形个数,0~65535
 *
 * @retval      无
 */
void dac_triangular_wave(uint16_t maxval, uint16_t dt, uint16_t samples, uint16_t n) {
    uint16_t i, j;
    float incval; /* 递增量 */
    float Curval; /* 当前值 */

    if (samples > ((maxval + 1) * 2))return; /* 数据不合法 */

    incval = (maxval + 1) / (samples / 2); /* 计算递增量 */

    for (j = 0; j < n; j++) {
        Curval = 0;
        HAL_DAC_SetValue(&g_dac_handle, DAC_CHANNEL_1, DAC_ALIGN_12B_R, Curval); /* 先输出0 */
        for (i = 0; i < (samples / 2); i++) /* 输出上升沿 */
        {
            Curval += incval; /* 新的输出值 */
            HAL_DAC_SetValue(&g_dac_handle, DAC_CHANNEL_1, DAC_ALIGN_12B_R, Curval);
            HAL_Delay(dt);
        }
        for (i = 0; i < (samples / 2); i++) /* 输出下降沿 */
        {
            Curval -= incval; /* 新的输出值 */
            HAL_DAC_SetValue(&g_dac_handle, DAC_CHANNEL_1, DAC_ALIGN_12B_R, Curval);
            HAL_Delay(dt);
        }
    }
}

void dac_triangular_wave_by_key(void) {
    uint8_t key = KEY_SCAN(0); /* 按键扫描 */

    if (key == KEY0_PRES) /* 高采样率 , 100hz波形 ， 实际只有65.5hz */
    {
        printf("DAC Wave1");
        dac_triangular_wave(4095, 5, 2000, 100); /* 幅值4095, 采样点间隔5us, 2000个采样点, 100个波形 */
        printf("DAC None");
    } else if (key == KEY1_PRES) /* 低采样率 , 100hz波形 ， 实际99.5hz */
    {
        printf("DAC Wave2");
        dac_triangular_wave(4095, 500, 20, 100); /* 幅值4095, 采样点间隔500us, 20个采样点, 100个波形 */
        printf("DAC None");
    }
}
