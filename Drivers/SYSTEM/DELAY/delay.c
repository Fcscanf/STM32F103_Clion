//
// Created by fcant on 2025/12/8 星期一.
//

#include "delay.h"

static uint16_t g_fac_us = 0; /* us延时倍乘数 */

void delay_us(uint32_t nus) {
    uint32_t temp;
    SysTick->LOAD = nus * g_fac_us; /* 时间加载 */
    SysTick->VAL = 0x00; /* 清空计数器 */
    SysTick->CTRL |= 1 << 0; /* 开始倒数 */

    do {
        temp = SysTick->CTRL;
    } while ((temp & 0x01) && !(temp & (1 << 16))); /* CTRL.ENABLE位必须为1, 并等待时间到达 */

    SysTick->CTRL &= ~(1 << 0); /* 关闭SYSTICK */
    SysTick->VAL = 0X00; /* 清空计数器 */
}
