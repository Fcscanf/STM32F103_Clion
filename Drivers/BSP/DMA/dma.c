//
// Created by fcant on 2025/11/10 星期一.
//
#include "dma.h"
#include <stdio.h>

#include "../../BSP/KEY/key.h"

DMA_HandleTypeDef g_dma_handle = {0}; /* DMA句柄 */
extern UART_HandleTypeDef huart1; /* UART句柄 */

/**
 * @brief       串口TX DMA初始化函数
 *   @note      这里的传输形式是固定的, 这点要根据不同的情况来修改
 *              从存储器 -> 外设模式/8位数据宽度/存储器增量模式
 *
 * @param       dmax_chy    : DMA的通道, DMA1_Channel1 ~ DMA1_Channel7, DMA2_Channel1 ~ DMA2_Channel5
 *                            某个外设对应哪个DMA, 哪个通道, 请参考<<STM32中文参考手册 V10>> 10.3.7节
 *                            必须设置正确的DMA及通道, 才能正常使用!
 * @retval      无
 */
void dma_init(DMA_Channel_TypeDef *DMAx_CHx) {
    if ((uint32_t) DMAx_CHx > (uint32_t) DMA1_Channel7) /* 大于DMA1_Channel7, 则为DMA2的通道了 */
    {
        __HAL_RCC_DMA2_CLK_ENABLE(); /* DMA2时钟使能 */
    } else {
        __HAL_RCC_DMA1_CLK_ENABLE(); /* DMA1时钟使能 */
    }

    __HAL_LINKDMA(&huart1, hdmatx, g_dma_handle); /* 将DMA与USART1联系起来(发送DMA) */

    /* Tx DMA配置 */
    g_dma_handle.Instance = DMAx_CHx; /* USART1_TX使用的DMA通道为: DMA1_Channel4 */
    g_dma_handle.Init.Direction = DMA_MEMORY_TO_PERIPH; /* DIR = 1 , 存储器到外设模式 */
    g_dma_handle.Init.PeriphInc = DMA_PINC_DISABLE; /* 外设非增量模式 */
    g_dma_handle.Init.MemInc = DMA_MINC_ENABLE; /* 存储器增量模式 */
    g_dma_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE; /* 外设数据长度:8位 */
    g_dma_handle.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE; /* 存储器数据长度:8位 */
    g_dma_handle.Init.Mode = DMA_NORMAL; /* 外设流控模式 */
    g_dma_handle.Init.Priority = DMA_PRIORITY_MEDIUM; /* 中等优先级 */

    HAL_DMA_Init(&g_dma_handle);
}

const uint8_t TEXT_TO_SEND[] = {"STM32 DMA UART Transmit Test"}; /* 要循环发送的字符串 */
#define SEND_BUF_SIZE       (sizeof(TEXT_TO_SEND) + 2) * 200    /* 发送数据长度, 等于sizeof(TEXT_TO_SEND) + 2的200倍. */

uint8_t g_sendbuf[SEND_BUF_SIZE]; /* 发送数据缓冲区 */
uint16_t i, k;
uint16_t len;
uint8_t mask = 0;

void data_init(void) {
    len = sizeof(TEXT_TO_SEND);
    k = 0;

    for (i = 0; i < SEND_BUF_SIZE; i++) /* 填充ASCII字符集数据 */
    {
        if (k >= len) /* 入换行符 */
        {
            if (mask) {
                g_sendbuf[i] = 0x0a;
                k = 0;
            } else {
                g_sendbuf[i] = 0x0d;
                mask++;
            }
        } else /* 复制TEXT_TO_SEND语句 */
        {
            mask = 0;
            g_sendbuf[i] = TEXT_TO_SEND[k];
            k++;
        }
    }
}

uint8_t key = 0;
float pro = 0; /* 进度 */
void dma_transmit(void) {
    key = KEY_SCAN(0);
    if (key == KEY0_PRES) /* KEY0按下 */
    {
        printf("DMA DATA:\r\n");
        printf("Start Transmit ....\r\n");

        HAL_UART_Transmit_DMA(&huart1, g_sendbuf, SEND_BUF_SIZE);
        /* 等待DMA传输完成，此时来做另外一些事情，比如点灯
         * 实际应用中，传输数据期间，可以执行另外的任务
         */
        while (1) {
            if (__HAL_DMA_GET_FLAG(&g_dma_handle, DMA_FLAG_TC4)) /* 等待 DMA1_Channel4 传输完成 */
            {
                __HAL_DMA_CLEAR_FLAG(&g_dma_handle, DMA_FLAG_TC4);
                HAL_UART_DMAStop(&huart1); /* 传输完成以后关闭串口DMA */
                break;
            }

            pro = DMA1_Channel4->CNDTR; /* 得到当前还剩余多少个数据 */
            len = SEND_BUF_SIZE; /* 总长度 */
            pro = 1 - (pro / len); /* 得到百分比 */
            pro *= 100; /* 扩大100倍 */
            // printf("transmit %.2f%%\r\n", pro);
        }
        printf("transmit 100%%\r\n"); /* 显示100% */
        printf("Transmit Finished!\r\n"); /* 提示传送完成 */
    }
}
