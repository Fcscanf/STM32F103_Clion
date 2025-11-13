//
// Created by fcant on 2025/8/27 星期三.
//

#ifndef STM32F103_CLION_KEY_H
#define STM32F103_CLION_KEY_H

#include "stm32f1xx_hal.h"

/* 引脚 定义 */
#define KEY0_GPIO_PORT GPIOE
#define KEY0_GPIO_PIN GPIO_PIN_4
/* PE 口时钟使能 */
#define KEY0_GPIO_CLK_ENABLE() do{ __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)
#define KEY1_GPIO_PORT GPIOE
#define KEY1_GPIO_PIN GPIO_PIN_3
/* PE 口时钟使能 */
#define KEY1_GPIO_CLK_ENABLE() do{ __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)
#define WKUP_GPIO_PORT GPIOA
#define WKUP_GPIO_PIN GPIO_PIN_0
/* PA 口时钟使能 */
#define WKUP_GPIO_CLK_ENABLE() do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)

#define KEY0 HAL_GPIO_ReadPin(KEY0_GPIO_PORT, KEY0_GPIO_PIN) /* 读取 KEY0 引脚 */
#define KEY1 HAL_GPIO_ReadPin(KEY1_GPIO_PORT, KEY1_GPIO_PIN) /* 读取 KEY1 引脚 */
#define WK_UP HAL_GPIO_ReadPin(WKUP_GPIO_PORT, WKUP_GPIO_PIN) /* 读取 WKUP 引脚 */
#define KEY0_PRES 1 /* KEY0 按下 */
#define KEY1_PRES 2 /* KEY1 按下 */
#define WKUP_PRES 3 /* KEY_UP 按下(即 WK_UP) */

void KEY_INIT(void);
uint8_t KEY_SCAN(uint8_t mode);

#endif //STM32F103_CLION_KEY_H