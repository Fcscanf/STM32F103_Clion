//
// Created by fcant on 2025/9/8 星期一.
//

#ifndef STM32F103_CLION_TPAD_H
#define STM32F103_CLION_TPAD_H

#include "stm32f1xx_hal.h"
#include "../LED/led.h"

/*
 * 触摸的门限值，也就是必须大于 g_tpad_default_val+TPAD_GATE_VAL
 * 才认为是有效触摸，改大 TPAD_GATE_VAL 可以降低灵敏度，反之，则可以提高灵敏度
 * 根据实际需求，选择合适的 TPAD_GATE_VAL 即可
 */
#define TPAD_GATE_VAL 40 /* 触摸的门限值，也就是必须大于 g_tpad_default_val+TPAD_GATE_VAL */
#define TPAD_ARR_MAX_VAL 0XFFFF /* 最大的ARR值，一般设置为定时器ARR的最大值 */

uint8_t tpad_init(uint16_t psc);
void working(void);

#endif //STM32F103_CLION_TPAD_H