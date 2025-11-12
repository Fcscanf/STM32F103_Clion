//
// Created by fcant on 2025/11/12 星期三.
//

#include "dac.h"

#include <math.h>

#include "../KEY/key.h"

DAC_HandleTypeDef g_dac_dma_handle = {0};

uint16_t g_dac_sin_buf[4096]; /* 发送数据缓冲区 */

/* DAC DMA输出波形初始化函数 */
void dac_dma_wave_init(void) {
    __HAL_RCC_DMA2_CLK_ENABLE();

    DMA_HandleTypeDef g_dma_dac_handle = {0};
    g_dma_dac_handle.Instance = DMA2_Channel3;
    g_dma_dac_handle.Init.Direction = DMA_MEMORY_TO_PERIPH;
    g_dma_dac_handle.Init.PeriphInc = DMA_PINC_DISABLE;
    g_dma_dac_handle.Init.MemInc = DMA_MINC_ENABLE;
    g_dma_dac_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    g_dma_dac_handle.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    g_dma_dac_handle.Init.Mode = DMA_CIRCULAR;
    g_dma_dac_handle.Init.Priority = DMA_PRIORITY_MEDIUM;
    HAL_DMA_Init(&g_dma_dac_handle);

    __HAL_LINKDMA(&g_dac_dma_handle, DMA_Handle1, g_dma_dac_handle);

    g_dac_dma_handle.Instance = DAC;
    HAL_DAC_Init(&g_dac_dma_handle);

    DAC_ChannelConfTypeDef dac_ch_conf = {0};
    dac_ch_conf.DAC_Trigger = DAC_TRIGGER_T7_TRGO;
    dac_ch_conf.DAC_OutputBuffer = DAC_OUTPUTBUFFER_DISABLE;
    HAL_DAC_ConfigChannel(&g_dac_dma_handle, &dac_ch_conf, DAC_CHANNEL_1);

    HAL_DMA_Start(&g_dma_dac_handle, (uint32_t) g_dac_sin_buf, (uint32_t) &DAC1->DHR12R1, 0);
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
 * @brief       DAC DMA使能波形输出
 *   @note      TIM7的输入时钟频率(f)来自APB1, f = 36M * 2 = 72Mhz.
 *              DAC触发频率 ftrgo = f / ((psc + 1) * (arr + 1))
 *              波形频率 = ftrgo / ndtr;
 *
 * @param       ndtr        : DMA通道单次传输数据量
 * @param       arr         : TIM7的自动重装载值
 * @param       psc         : TIM7的分频系数
 * @retval      无
 */
void dac_dma_wave_enable(uint16_t cndtr, uint16_t arr, uint16_t psc) {
    __HAL_RCC_TIM7_CLK_ENABLE();

    TIM_HandleTypeDef tim7_handle = {0};
    tim7_handle.Instance = TIM7;
    tim7_handle.Init.Prescaler = psc;
    tim7_handle.Init.Period = arr;
    HAL_TIM_Base_Init(&tim7_handle);

    TIM_MasterConfigTypeDef tim_mater_config = {0};
    tim_mater_config.MasterOutputTrigger = TIM_TRGO_UPDATE;
    tim_mater_config.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    HAL_TIMEx_MasterConfigSynchronization(&tim7_handle, &tim_mater_config);

    HAL_TIM_Base_Start(&tim7_handle);

    HAL_DAC_Stop_DMA(&g_dac_dma_handle, DAC_CHANNEL_1);
    HAL_DAC_Start_DMA(&g_dac_dma_handle, DAC_CHANNEL_1, (uint32_t *) g_dac_sin_buf, cndtr, DAC_ALIGN_12B_R);
}

/**
 * @brief       产生正弦波序列函数
 *   @note      需保证: maxval > samples/2
 * @param       maxval : 最大值(0 < maxval < 2048)
 * @param       samples: 采样点的个数
 * @retval      无
 */
void dac_creat_sin_buf(uint16_t maxval, uint16_t samples) {
    uint8_t i;
    float outdata = 0; /* 存放计算后的数字量 */
    float inc = (2 * 3.1415962) / samples; /* 计算相邻两个点的x轴间隔 */

    if (maxval <= (samples / 2))return; /* 数据不合法 */

    for (i = 0; i < samples; i++) {
        /*
         * 正弦波函数解析式：y = Asin(ωx + φ）+ b
         * 计算每个点的y值，将峰值放大maxval倍，并将曲线向上偏移maxval到正数区域
         * 注意：DAC无法输出负电压，所以需要将曲线向上偏移一个峰值的量，让整个曲线都落在正数区域
         */
        outdata = maxval * sin(inc * i) + maxval;
        if (outdata > 4095)
            outdata = 4095; /* 上限限定 */
        //printf("%f\r\n",outdata);
        g_dac_sin_buf[i] = outdata;
    }
}

void dac_sin_wave_by_key(void) {
    uint8_t key = KEY_SCAN(0); /* 按键扫描 */

    if (key == KEY0_PRES) /* 高采样率 */
    {
        dac_creat_sin_buf(2048, 100);
        dac_dma_wave_enable(100, 10 - 1, 24 - 1); /* 300Khz触发频率, 100个点, 得到最高3KHz的正弦波. */
    } else if (key == KEY1_PRES) /* 低采样率 */
    {
        dac_creat_sin_buf(2048, 10);
        dac_dma_wave_enable(10, 10 - 1, 24 - 1); /* 300Khz触发频率, 10个点, 可以得到最高30KHz的正弦波. */
    }
}
