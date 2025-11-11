//
// Created by fcant on 2025/11/10 星期一.
//

#include "adc.h"

#include <stdio.h>

DMA_HandleTypeDef g_dma_nch_adc_handle = {0};
ADC_HandleTypeDef g_adc_nch_dma_handle = {0};

#define ADC_DMA_BUF_SIZE        50 * 6      /* ADC DMA采集 BUF大小, 应等于ADC通道数的整数倍 */
uint16_t g_adc_dma_buf[ADC_DMA_BUF_SIZE]; /* ADC DMA BUF */
uint8_t g_adc_dma_sta; /* DMA传输状态标志, 0,未完成; 1, 已完成 */

/* ADC N通道(6通道) DMA读取 初始化函数 */
void adc_nch_dma_init(void) {
    uint32_t mar = (uint32_t) &g_adc_dma_buf;

    ADC_ChannelConfTypeDef adc_ch_conf;

    __HAL_RCC_DMA1_CLK_ENABLE();

    g_dma_nch_adc_handle.Instance = DMA1_Channel1;
    g_dma_nch_adc_handle.Init.Direction = DMA_PERIPH_TO_MEMORY;
    g_dma_nch_adc_handle.Init.PeriphInc = DMA_PINC_DISABLE;
    g_dma_nch_adc_handle.Init.MemInc = DMA_MINC_ENABLE;
    g_dma_nch_adc_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    g_dma_nch_adc_handle.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    g_dma_nch_adc_handle.Init.Mode = DMA_NORMAL;
    g_dma_nch_adc_handle.Init.Priority = DMA_PRIORITY_MEDIUM;
    HAL_DMA_Init(&g_dma_nch_adc_handle);

    __HAL_LINKDMA(&g_adc_nch_dma_handle, DMA_Handle, g_dma_nch_adc_handle);

    g_adc_nch_dma_handle.Instance = ADC1;
    g_adc_nch_dma_handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    g_adc_nch_dma_handle.Init.ScanConvMode = ADC_SCAN_ENABLE;
    g_adc_nch_dma_handle.Init.ContinuousConvMode = ENABLE;
    g_adc_nch_dma_handle.Init.NbrOfConversion = 6;
    g_adc_nch_dma_handle.Init.DiscontinuousConvMode = DISABLE;
    g_adc_nch_dma_handle.Init.NbrOfDiscConversion = 0;
    g_adc_nch_dma_handle.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    HAL_ADC_Init(&g_adc_nch_dma_handle);

    HAL_ADCEx_Calibration_Start(&g_adc_nch_dma_handle);

    adc_ch_conf.Channel = ADC_CHANNEL_0;
    adc_ch_conf.Rank = ADC_REGULAR_RANK_1;
    adc_ch_conf.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
    HAL_ADC_ConfigChannel(&g_adc_nch_dma_handle, &adc_ch_conf);

    adc_ch_conf.Channel = ADC_CHANNEL_1;
    adc_ch_conf.Rank = ADC_REGULAR_RANK_2;
    HAL_ADC_ConfigChannel(&g_adc_nch_dma_handle, &adc_ch_conf);

    adc_ch_conf.Channel = ADC_CHANNEL_2;
    adc_ch_conf.Rank = ADC_REGULAR_RANK_3;
    HAL_ADC_ConfigChannel(&g_adc_nch_dma_handle, &adc_ch_conf);

    adc_ch_conf.Channel = ADC_CHANNEL_3;
    adc_ch_conf.Rank = ADC_REGULAR_RANK_4;
    HAL_ADC_ConfigChannel(&g_adc_nch_dma_handle, &adc_ch_conf);

    adc_ch_conf.Channel = ADC_CHANNEL_4;
    adc_ch_conf.Rank = ADC_REGULAR_RANK_5;
    HAL_ADC_ConfigChannel(&g_adc_nch_dma_handle, &adc_ch_conf);

    adc_ch_conf.Channel = ADC_CHANNEL_5;
    adc_ch_conf.Rank = ADC_REGULAR_RANK_6;
    HAL_ADC_ConfigChannel(&g_adc_nch_dma_handle, &adc_ch_conf);

    HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 3, 3);
    HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

    HAL_DMA_Start_IT(&g_dma_nch_adc_handle, (uint32_t) &ADC1->DR, mar, 0);
    HAL_ADC_Start_DMA(&g_adc_nch_dma_handle, &mar, 0);
}

/* ADC MSP初始化函数 */
void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc) {
    if (hadc->Instance == ADC1) {
        GPIO_InitTypeDef gpio_init_struct;
        RCC_PeriphCLKInitTypeDef adc_clk_init = {0};

        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_ADC1_CLK_ENABLE();

        gpio_init_struct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5;
        gpio_init_struct.Mode = GPIO_MODE_ANALOG;
        HAL_GPIO_Init(GPIOA, &gpio_init_struct);

        adc_clk_init.PeriphClockSelection = RCC_PERIPHCLK_ADC;
        adc_clk_init.AdcClockSelection = RCC_ADCPCLK2_DIV6;
        HAL_RCCEx_PeriphCLKConfig(&adc_clk_init);
    }
}

/* 使能一次ADC DMA传输函数 */
void adc_dma_enable(void) {
    ADC1->CR2 &= ~(1 << 0);

    DMA1_Channel1->CCR &= ~(1 << 0);
    while (DMA1_Channel1->CCR & (1 << 0));
    DMA1_Channel1->CNDTR = ADC_DMA_BUF_SIZE;
    DMA1_Channel1->CCR |= 1 << 0;

    ADC1->CR2 |= 1 << 0;
    ADC1->CR2 |= 1 << 22;

    //    __HAL_ADC_DISABLE(&g_adc_dma_handle);
    //
    //    __HAL_DMA_DISABLE(&g_dma_adc_handle);
    //    while (__HAL_DMA_GET_FLAG(&g_dma_adc_handle, __HAL_DMA_GET_TC_FLAG_INDEX(&g_dma_adc_handle)));
    //    DMA1_Channel1->CNDTR = cndtr;
    //    __HAL_DMA_ENABLE(&g_dma_adc_handle);
    //
    //    __HAL_ADC_ENABLE(&g_adc_dma_handle);
    //    HAL_ADC_Start(&g_adc_dma_handle);
}

/* ADC DMA采集中断服务函数 */
void DMA1_Channel1_IRQHandler(void) {
    if (DMA1->ISR & (1 << 1)) {
        g_adc_dma_sta = 1;
        DMA1->IFCR |= 1 << 1;
    }
}

uint16_t i, j;
uint16_t adcx;
uint32_t sum;
float temp;

void show(void) {
    if (g_adc_dma_sta == 1) {
        /* 循环显示通道0~通道5的结果 */
        for (j = 0; j < 6; j++) /* 遍历6个通道 */
        {
            sum = 0; /* 清零 */
            for (i = 0; i < ADC_DMA_BUF_SIZE / 6; i++) /* 每个通道采集了50次数据,进行50次累加 */
            {
                sum += g_adc_dma_buf[(6 * i) + j]; /* 相同通道的转换数据累加 */
            }
            adcx = sum / (ADC_DMA_BUF_SIZE / 6); /* 取平均值 */

            /* 显示结果 */
            // lcd_show_xnum(108, 110 + (j * 30), adcx, 4, 12, 0, BLUE);   /* 显示ADC采样后的原始值 */
            printf("S CH%d: %d\r\n", j, adcx);

            temp = (float) adcx * (3.3 / 4096); /* 获取计算后的带小数的实际电压值，比如3.1111 */
            adcx = temp; /* 赋值整数部分给adcx变量，因为adcx为u16整形 */
            // lcd_show_xnum(108, 122 + (j * 30), adcx, 1, 12, 0, BLUE);   /* 显示电压值的整数部分，3.1111的话，这里就是显示3 */
            printf("V CH%d: %d", j, adcx);

            temp -= adcx; /* 把已经显示的整数部分去掉，留下小数部分，比如3.1111-3=0.1111 */
            temp *= 1000; /* 小数部分乘以1000，例如：0.1111就转换为111.1，相当于保留三位小数。 */
            // lcd_show_xnum(120, 122 + (j * 30), temp, 3, 12, 0X80, BLUE);/* 显示小数部分（前面转换为了整形显示），这里显示的就是111. */
            printf(".%d\r\n", (uint16_t) temp);
        }

        g_adc_dma_sta = 0; /* 清除DMA采集完成状态标志 */
        adc_dma_enable(); /* 启动下一次ADC DMA采集 */
    }
}
