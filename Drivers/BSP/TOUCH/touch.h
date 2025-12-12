//
// Created by fcant on 2025/12/8 星期一.
//

/*
 * 触摸屏 驱动代码
 * 支持电阻/电容式触摸屏
 * 触摸屏驱动（支持ADS7843/7846/UH7843/7846/XPT2046/TSC2046/GT9147/GT9271/FT5206等）代码
 */

#ifndef STM32F103_CLION_TOUCH_H
#define STM32F103_CLION_TOUCH_H

#include "stm32f1xx_hal.h"

/******************************************************************************************/
/* 电阻触摸屏驱动IC T_PEN/T_CS/T_MISO/T_MOSI/T_SCK 引脚 定义 */

#define T_PEN_GPIO_PORT                 GPIOF
#define T_PEN_GPIO_PIN                  GPIO_PIN_10
#define T_PEN_GPIO_CLK_ENABLE()         do{ __HAL_RCC_GPIOF_CLK_ENABLE(); }while(0)   /* 所在IO口时钟使能 */

#define T_CS_GPIO_PORT                  GPIOF
#define T_CS_GPIO_PIN                   GPIO_PIN_11
#define T_CS_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOF_CLK_ENABLE(); }while(0)   /* 所在IO口时钟使能 */

#define T_MISO_GPIO_PORT                GPIOB
#define T_MISO_GPIO_PIN                 GPIO_PIN_2
#define T_MISO_GPIO_CLK_ENABLE()        do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)   /* 所在IO口时钟使能 */

#define T_MOSI_GPIO_PORT                GPIOF
#define T_MOSI_GPIO_PIN                 GPIO_PIN_9
#define T_MOSI_GPIO_CLK_ENABLE()        do{ __HAL_RCC_GPIOF_CLK_ENABLE(); }while(0)   /* 所在IO口时钟使能 */

#define T_CLK_GPIO_PORT                 GPIOB
#define T_CLK_GPIO_PIN                  GPIO_PIN_1
#define T_CLK_GPIO_CLK_ENABLE()         do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)   /* 所在IO口时钟使能 */

/******************************************************************************************/

/* 电阻触摸屏控制引脚 */
#define T_PEN           HAL_GPIO_ReadPin(T_PEN_GPIO_PORT, T_PEN_GPIO_PIN)           /* T_PEN */
#define T_MISO          HAL_GPIO_ReadPin(T_MISO_GPIO_PORT, T_MISO_GPIO_PIN)         /* T_MISO */

#define T_MOSI(x)     do{ x ? \
                          HAL_GPIO_WritePin(T_MOSI_GPIO_PORT, T_MOSI_GPIO_PIN, GPIO_PIN_SET) : \
                          HAL_GPIO_WritePin(T_MOSI_GPIO_PORT, T_MOSI_GPIO_PIN, GPIO_PIN_RESET); \
                      }while(0)     /* T_MOSI */

#define T_CLK(x)      do{ x ? \
                          HAL_GPIO_WritePin(T_CLK_GPIO_PORT, T_CLK_GPIO_PIN, GPIO_PIN_SET) : \
                          HAL_GPIO_WritePin(T_CLK_GPIO_PORT, T_CLK_GPIO_PIN, GPIO_PIN_RESET); \
                      }while(0)     /* T_CLK */

#define T_CS(x)       do{ x ? \
                          HAL_GPIO_WritePin(T_CS_GPIO_PORT, T_CS_GPIO_PIN, GPIO_PIN_SET) : \
                          HAL_GPIO_WritePin(T_CS_GPIO_PORT, T_CS_GPIO_PIN, GPIO_PIN_RESET); \
                      }while(0)     /* T_CS */

#endif //STM32F103_CLION_TOUCH_H
