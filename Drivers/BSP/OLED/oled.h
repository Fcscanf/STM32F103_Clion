//
// Created by fcant on 2025/10/28 星期二.
//

#ifndef STM32F103_CLION_OLED_H
#define STM32F103_CLION_OLED_H

/******************************************************************************************/
/* OLED 8080 模式引脚 定义 */

/* 片选脚 */
#define OLED_CS_PORT                GPIOD
#define OLED_CS_PIN                 GPIO_PIN_6
#define OLED_CS_CLK_ENABLE()        do{ __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)   /* PD口时钟使能 */

/* 数据类型脚 命令/数据*/
#define OLED_RS_PORT                GPIOD
#define OLED_RS_PIN                 GPIO_PIN_3
#define OLED_RS_CLK_ENABLE()        do{ __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)   /* PD口时钟使能 */

/* 向OLED读取数据脚 */
#define OLED_RD_PORT                GPIOG
#define OLED_RD_PIN                 GPIO_PIN_13
#define OLED_RD_CLK_ENABLE()        do{ __HAL_RCC_GPIOG_CLK_ENABLE(); }while(0)   /* PG口时钟使能 */

/* 向OLED写入数据脚 */
#define OLED_WR_PORT                GPIOG
#define OLED_WR_PIN                 GPIO_PIN_14
#define OLED_WR_CLK_ENABLE()        do{ __HAL_RCC_GPIOG_CLK_ENABLE(); }while(0)   /* PG口时钟使能 */

/* 复位脚 */
#define OLED_RST_PORT               GPIOG
#define OLED_RST_PIN                GPIO_PIN_15
#define OLED_RST_CLK_ENABLE()       do{ __HAL_RCC_GPIOG_CLK_ENABLE(); }while(0)   /* PG口时钟使能 */

/* 数据脚 */
#define OLED_DATA_PORT               GPIOC
#define OLED_DATA_PIN                GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7
#define OLED_DATA_CLK_ENABLE()       do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)   /* PC口时钟使能 */
/******************************************************************************************/

/* OLED 8080模式相关端口控制函数 定义 */
#define OLED_RST(x)     do{ x ? \
                                  HAL_GPIO_WritePin(OLED_RST_PORT, OLED_RST_PIN, GPIO_PIN_SET) : \
                                  HAL_GPIO_WritePin(OLED_RST_PORT, OLED_RST_PIN, GPIO_PIN_RESET); \
                        }while(0)       /* 设置RST引脚 */

#define OLED_CS(x)      do{ x ? \
                                  HAL_GPIO_WritePin(OLED_CS_PORT, OLED_CS_PIN, GPIO_PIN_SET) : \
                                  HAL_GPIO_WritePin(OLED_CS_PORT, OLED_CS_PIN, GPIO_PIN_RESET); \
                        }while(0)       /* 设置CS引脚 */
#define OLED_RS(x)      do{ x ? \
                                  HAL_GPIO_WritePin(OLED_RS_PORT, OLED_RS_PIN, GPIO_PIN_SET) : \
                                  HAL_GPIO_WritePin(OLED_RS_PORT, OLED_RS_PIN, GPIO_PIN_RESET); \
                        }while(0)       /* 设置RS引脚 */

#define OLED_WR(x)      do{ x ? \
                                  HAL_GPIO_WritePin(OLED_WR_PORT, OLED_WR_PIN, GPIO_PIN_SET) :  \
                                  HAL_GPIO_WritePin(OLED_WR_PORT, OLED_WR_PIN, GPIO_PIN_RESET); \
                        } while (0)     /* 设置WR引脚 */

#define OLED_RD(x)      do{ x ? \
                                  HAL_GPIO_WritePin(OLED_RD_PORT, OLED_RD_PIN, GPIO_PIN_SET) : \
                                  HAL_GPIO_WritePin(OLED_RD_PORT, OLED_RD_PIN, GPIO_PIN_RESET); \
                        }while(0)       /* 设置RD引脚 */

/* 命令/数据 定义 */
#define OLED_CMD        0       /* 写命令 */
#define OLED_DATA       1       /* 写数据 */

/******************************************************************************************/
#include <stdint.h>

void oled_wr_byte(uint8_t data, uint8_t cmd);    /* 写一个字节到OLED */
void oled_init(void);           /* OLED初始化 */
void oled_clear(void);
void oled_refresh_gram(void);
void oled_draw_point_test(uint8_t x, uint8_t y);
void oled_show_char_test(uint8_t x, uint8_t y, uint8_t mode);


#endif //STM32F103_CLION_OLED_H