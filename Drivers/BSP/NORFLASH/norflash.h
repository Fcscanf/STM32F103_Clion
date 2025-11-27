//
// Created by fcant on 2025/11/27 星期四.
//

#ifndef STM32F103_CLION_NORFLASH_H
#define STM32F103_CLION_NORFLASH_H

#include "stm32f1xx_hal.h"

/******************************************************************************************/
/* NORFLASH 片选 引脚 定义 */
#define NORFLASH_CS_GPIO_PORT           GPIOB
#define NORFLASH_CS_GPIO_PIN            GPIO_PIN_12
#define NORFLASH_CS_GPIO_CLK_ENABLE()   do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)   /* PB口时钟使能 */

/******************************************************************************************/
/* NORFLASH 片选信号 */
#define NORFLASH_CS(x)      do{ x ? \
HAL_GPIO_WritePin(NORFLASH_CS_GPIO_PORT, NORFLASH_CS_GPIO_PIN, GPIO_PIN_SET) : \
HAL_GPIO_WritePin(NORFLASH_CS_GPIO_PORT, NORFLASH_CS_GPIO_PIN, GPIO_PIN_RESET); \
}while(0)


void norflash_init(void);
uint8_t norflash_rd_sr1(void);
uint8_t norflash_read_data(uint32_t addr);
void norflash_erase_sector(uint32_t addr);
void norflash_write_page(uint8_t data, uint32_t addr);

#endif //STM32F103_CLION_NORFLASH_H