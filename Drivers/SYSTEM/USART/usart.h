//
// Created by fcant on 2025/8/30 星期六.
//

#ifndef STM32F103_CLION_UART_H
#define STM32F103_CLION_UART_H
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_uart.h"

#define USART_REC_LEN 200

void USART1_UART_Init(uint32_t baudrate);
void USART1_FeedBack(void);

#endif //STM32F103_CLION_UART_H