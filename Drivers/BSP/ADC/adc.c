//
// Created by fcant on 2025/11/10 星期一.
//

#include "adc.h"

#include <stdio.h>

ADC_HandleTypeDef g_adc_handle;

/* ADC单通道 */
void adc_init(void) {
    ADC_ChannelConfTypeDef adc_ch_conf;

    g_adc_handle.Instance = ADC1;
    g_adc_handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    g_adc_handle.Init.ScanConvMode = ADC_SCAN_DISABLE;
    g_adc_handle.Init.ContinuousConvMode = DISABLE;
    g_adc_handle.Init.NbrOfConversion = 1;
    g_adc_handle.Init.DiscontinuousConvMode = DISABLE;
    g_adc_handle.Init.NbrOfDiscConversion = 0;
    g_adc_handle.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    HAL_ADC_Init(&g_adc_handle);

    HAL_ADCEx_Calibration_Start(&g_adc_handle);

    adc_ch_conf.Channel = ADC_CHANNEL_1;
    adc_ch_conf.Rank = ADC_REGULAR_RANK_1;
    adc_ch_conf.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
    HAL_ADC_ConfigChannel(&g_adc_handle, &adc_ch_conf);
}

/* ADC MSP初始化函数 */
void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc) {
    if (hadc->Instance == ADC1) {
        GPIO_InitTypeDef gpio_init_struct;
        RCC_PeriphCLKInitTypeDef adc_clk_init = {0};

        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_ADC1_CLK_ENABLE();

        gpio_init_struct.Pin = GPIO_PIN_1;
        gpio_init_struct.Mode = GPIO_MODE_ANALOG;
        HAL_GPIO_Init(GPIOA, &gpio_init_struct);

        adc_clk_init.PeriphClockSelection = RCC_PERIPHCLK_ADC;
        adc_clk_init.AdcClockSelection = RCC_ADCPCLK2_DIV6;
        HAL_RCCEx_PeriphCLKConfig(&adc_clk_init);
    }
}

/* 获得ADC转换后的结果函数 */
uint32_t adc_get_result(void) {
    HAL_ADC_Start(&g_adc_handle);
    HAL_ADC_PollForConversion(&g_adc_handle, 10);
    return (uint16_t) HAL_ADC_GetValue(&g_adc_handle);
}

uint16_t adcx;
float temp;

void show(void) {
    adcx = adc_get_result();
    // lcd_show_xnum(134, 110, adcx, 5, 16, 0, BLUE);  /* 显示ADCC采样后的原始值 */
    printf("S: %d\r\n", adcx);

    temp = (float) adcx * (3.3 / 4096); /* 获取计算后的带小数的实际电压值，比如3.1111 */

    adcx = temp; /* 赋值整数部分给adcx变量，因为adcx为u16整形 */
    // lcd_show_xnum(134, 130, adcx, 1, 16, 0, BLUE);  /* 显示电压值的整数部分，3.1111的话，这里就是显示3 */
    printf("V: %d", adcx);

    temp -= adcx; /* 把已经显示的整数部分去掉，留下小数部分，比如3.1111-3=0.1111 */
    temp *= 1000; /* 小数部分乘以1000，例如：0.1111就转换为111.1，相当于保留三位小数。 */
    // lcd_show_xnum(150, 130, temp, 3, 16, 0X80, BLUE);/* 显示小数部分（前面转换为了整形显示），这里显示的就是111. */
    printf(".%d\r\n", (uint16_t) temp);
}
