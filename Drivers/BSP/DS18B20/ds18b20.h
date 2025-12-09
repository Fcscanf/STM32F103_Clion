//
// Created by fcant on 2025/12/9 星期二.
//

#ifndef STM32F103_CLION_DS18B20_H
#define STM32F103_CLION_DS18B20_H

#include "stm32f1xx_hal.h"

/******************************************************************************************/
/* DS18B20引脚 定义 */

#define DS18B20_DQ_GPIO_PORT                GPIOG
#define DS18B20_DQ_GPIO_PIN                 GPIO_PIN_11
#define DS18B20_DQ_GPIO_CLK_ENABLE()        do{ __HAL_RCC_GPIOG_CLK_ENABLE(); }while(0)   /* PG口时钟使能 */

/******************************************************************************************/

/* IO操作函数 */
#define DS18B20_DQ_OUT(x)   do{ x ? \
HAL_GPIO_WritePin(DS18B20_DQ_GPIO_PORT, DS18B20_DQ_GPIO_PIN, GPIO_PIN_SET) : \
HAL_GPIO_WritePin(DS18B20_DQ_GPIO_PORT, DS18B20_DQ_GPIO_PIN, GPIO_PIN_RESET); \
}while(0)                                                       /* 数据端口输出 */
#define DS18B20_DQ_IN       HAL_GPIO_ReadPin(DS18B20_DQ_GPIO_PORT, DS18B20_DQ_GPIO_PIN)     /* 数据端口输入 */

void ds18b20_init(void);
float ds18b20_get_temperature(void);

#endif //STM32F103_CLION_DS18B20_H