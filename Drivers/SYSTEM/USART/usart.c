//
// Created by fcant on 2025/8/30 星期六.
//

#include "usart.h"

#include <stdio.h>

// 重定向 printf 到串口

// MDK/ARMCC/ARMCLANG
// 在 Keil 的库里，printf 会调用 fputc()。
// 所以只要重写 int fputc(int ch, FILE *f)，就能把 printf 的输出重定向到串口。
// MDK 的 printf 调用 fputc → 每个字符顺序可靠
//
// GCC (arm-none-eabi-gcc)
// 在 newlib（GCC 默认的 C 库）里，printf 最终不会直接调用 fputc，而是通过 系统调用接口 _write() 来输出。
// 如果没有实现 _write()，printf 的输出会丢失或者跑到半主机（semihosting）。
// 所以在 CLion 里写的 fputc() 并不会被 printf 调用，自然无效。
// GCC 的 printf 调用 _write → 整块发送，和后续 HAL_UART_Transmit 调用存在竞态。输出顺序会乱

// V1:HAL库版
// int _write(int file, char *ptr, int len)
// {
//     HAL_UART_Transmit(&huart1, (uint8_t *)ptr, len, HAL_MAX_DELAY);
//     return len;
// }

// V2:寄存器版
int _write(int file, char *ptr, int len){
    (void)file; // 明确表示不使用file参数，避免警告
    for (int i = 0; i < len; i++) {
        while ((USART1->SR & 0X40) == 0);   // 等待上一个字符发送完成
        USART1->DR = (uint8_t)ptr[i];       // 发送字符
    }
    return len;
}

UART_HandleTypeDef huart1;
/* 数据接收缓冲区 */
uint8_t rx_buffer[1];
/* 串口接收到数据标志 */
uint8_t usart_rx_flag = 0;

/* 串口1初始化函数
  * @brief UART MSP Initialization
  * This function configures the hardware resources used in this example
  * @param huart: UART handle pointer
  * @retval None
  */
void USART1_UART_Init(uint32_t baudrate) {
    huart1.Instance = USART1;
    huart1.Init.BaudRate = baudrate;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    HAL_UART_Init(&huart1);

    HAL_UART_Receive_IT(&huart1, (uint8_t*)rx_buffer, 1);

}

/* 串口MSP回调函数 */
void HAL_UART_MspInit(UART_HandleTypeDef *huart) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* 如果是串口1，进行串口1 MSP 初始化 */
    if (huart->Instance == USART1) {
        /* 1、使能USART1和对应IO时钟 */
        __HAL_RCC_USART1_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();

        /* 2、初始化IO */
        /**USART1 GPIO Configuration
        PA9     ------> USART1_TX
        PA10     ------> USART1_RX
        */
        GPIO_InitStruct.Pin = GPIO_PIN_9;
        /* 推挽式复用输出 */
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        /* 高速 */
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_10;
        /* 输入 */
        GPIO_InitStruct.Mode = GPIO_MODE_AF_INPUT;
        /* 上拉 */
        GPIO_InitStruct.Pull = GPIO_PULLUP;

        // MX生成的属性选项
        // GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        // GPIO_InitStruct.Pull = GPIO_NOPULL;

        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
        /* 3、使能USART1中断，设置优先级 */
        HAL_NVIC_SetPriority(USART1_IRQn, 3, 3);
        HAL_NVIC_EnableIRQ(USART1_IRQn);
    }
}

/* 串口1中断服务函数 */
void USART1_IRQHandler(void) {
    HAL_UART_IRQHandler(&huart1);
}

/* 串口数据接收完成回调函数 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART1) {
        usart_rx_flag = 1;
        /* 由于HAL_UART_Receive_IT函数操作会失能，所以下面需要再次调用使能函数使其后续接收数据发生中断 */
        HAL_UART_Receive_IT(&huart1, (uint8_t*)rx_buffer, 1);
    }
}

/* 判断接收到数据后进行响应 */
void USART1_FeedBack(void) {
    if (usart_rx_flag == 1) {
        /* printf 和 HAL_UART_Transmit 走的不是同一个通道：
        在 _write 里已经把 printf 重定向到了串口（内部调用 HAL_UART_Transmit）。
        在 printf("您输入的字符为："); 的时候，底层也会发串口数据，但这一步并不是立刻“全部发完”，而是逐字节进入硬件发送移位寄存器。
        紧接着又调用了 HAL_UART_Transmit(&huart1, rx_buffer, 1, …) 去发送接收的那个字符。因为 UART 里 FIFO/移位器的调度，结果是这个字符 可能先发出去。
        所以终端上看起来顺序反了：先出来了 2，再出来提示语。 */
        printf("您输入的字符为：");
        HAL_UART_Transmit(&huart1, (uint8_t*)rx_buffer, 1, 1000);
        while (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TC) != SET);
        printf("\r\n");
        usart_rx_flag = 0;
    } else {
        HAL_Delay(10);
    }
}