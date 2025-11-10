//
// Created by fcant on 2025/11/10 ÐÇÆÚÒ».
//

#ifndef STM32F103_CLION_DMA_H
#define STM32F103_CLION_DMA_H

#include "stm32f1xx_hal.h"

void dma_init(DMA_Channel_TypeDef *DMAx_CHx);
void data_init(void);
void dma_transmit(void);

#endif //STM32F103_CLION_DMA_H