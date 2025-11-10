//
// Created by fcant on 2025/11/10 星期一.
//
#include "dma.h"
#include <stdio.h>
#include <string.h>

#include "stm32f1xx_hal.h"
#include "../../BSP/KEY/key.h"

DMA_HandleTypeDef g_dma_handler = {0};

uint8_t src_buf[10] = {0x0a, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};
uint8_t dest_buf[10] = {0};

/* 内存到内存 DMA传输 */
void dma_init(void) {
    /* 使能DMA1时钟 */
    __HAL_RCC_DMA1_CLK_ENABLE();

    g_dma_handler.Instance = DMA1_Channel1;
    g_dma_handler.Init.Direction = DMA_MEMORY_TO_MEMORY;

    /* 与目标地址相关 */
    g_dma_handler.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    g_dma_handler.Init.MemInc = DMA_MINC_ENABLE;

    /* 与源地址相关 */
    g_dma_handler.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    g_dma_handler.Init.PeriphInc = DMA_PINC_ENABLE;

    g_dma_handler.Init.Priority = DMA_PRIORITY_HIGH;
    g_dma_handler.Init.Mode = DMA_NORMAL; /* 内存到内存是不支持循环模式 */

    HAL_DMA_Init(&g_dma_handler);
    HAL_DMA_Start(&g_dma_handler, (uint32_t) src_buf, (uint32_t) dest_buf, 0);
}

void dma_enable_transmit(uint16_t cndtr) {
    __HAL_DMA_DISABLE(&g_dma_handler);

    //    DMA1_Channel1->CNDTR = cndtr;
    g_dma_handler.Instance->CNDTR = cndtr;

    __HAL_DMA_ENABLE(&g_dma_handler);
}

uint8_t key;
void dma_transmit(void) {
    key = KEY_SCAN(0); /* 得到键值 */

    if (key == KEY0_PRES) {
        memset(dest_buf, 0, 10);
        dma_enable_transmit(10);

        while (1) {
            if (__HAL_DMA_GET_FLAG(&g_dma_handler, DMA_FLAG_TC1)) {
                __HAL_DMA_CLEAR_FLAG(&g_dma_handler, DMA_FLAG_TC1);
                printf("传输完成 \r\n");
                break;
            }
        }
    }
}
