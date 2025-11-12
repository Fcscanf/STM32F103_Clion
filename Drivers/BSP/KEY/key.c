//
// Created by fcant on 2025/8/27 星期三.
//
#include "key.h"

/**
* @brief 按键初始化函数
* @param 无
* @retval 无
*/
void KEY_INIT(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0}; /* GPIO 配置参数存储变量 */
    KEY0_GPIO_CLK_ENABLE(); /* KEY0 时钟使能 */
    KEY1_GPIO_CLK_ENABLE(); /* KEY1 时钟使能 */
    WKUP_GPIO_CLK_ENABLE(); /* WKUP 时钟使能 */
    GPIO_InitStruct.Pin = KEY0_GPIO_PIN; /* KEY0 引脚 */
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT; /* 输入 */
    GPIO_InitStruct.Pull = GPIO_PULLUP; /* 上拉 */
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; /* 高速 */
    HAL_GPIO_Init(KEY0_GPIO_PORT, &GPIO_InitStruct); /* KEY0 引脚模式设置 */
    GPIO_InitStruct.Pin = KEY1_GPIO_PIN; /* KEY1 引脚 */
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT; /* 输入 */
    GPIO_InitStruct.Pull = GPIO_PULLUP; /* 上拉 */
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; /* 高速 */
    HAL_GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStruct); /* KEY1 引脚模式设置 */
    GPIO_InitStruct.Pin = WKUP_GPIO_PIN; /* WKUP 引脚 */
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT; /* 输入 */
    GPIO_InitStruct.Pull = GPIO_PULLDOWN; /* 下拉 */
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; /* 高速 */
    HAL_GPIO_Init(WKUP_GPIO_PORT, &GPIO_InitStruct); /* WKUP 引脚模式设置 */
}

/**
* @brief 按键扫描函数
* @note 该函数有响应优先级(同时按下多个按键): WK_UP > KEY1 > KEY0!!
* @param mode:0 / 1, 具体含义如下:
* @arg 0, 不支持连续按(当按键按下不放时, 只有第一次调用会返回键值,
* 必须松开以后, 再次按下才会返回其他键值)
* @arg 1, 支持连续按(当按键按下不放时, 每次调用该函数都会返回键值)
* @retval 键值, 定义如下:
* KEY0_PRES, 1, KEY0 按下
* KEY1_PRES, 2, KEY1 按下
* WKUP_PRES, 3, WKUP 按下
*/
uint8_t KEY_SCAN(uint8_t mode) {
    static uint8_t key_up = 1; /* 按键按松开标志 */
    uint8_t keyval = 0;
    if (mode) key_up = 1; /* 支持连按 */
    if (key_up && (KEY0 == 0 || KEY1 == 0 || WK_UP == 1)) {
        /* 按键松开标志为 1, 且有任意一个按键按下了 */
        HAL_Delay(10); /* 去抖动 */
        key_up = 0;
        if (KEY0 == 0) keyval = KEY0_PRES;
        if (KEY1 == 0) keyval = KEY1_PRES;
        if (WK_UP == 1) keyval = WKUP_PRES;
    } else if (KEY0 == 1 && KEY1 == 1 && WK_UP == 0) {
        /* 没有任何按键按下, 标记按键松开 */
        key_up = 1;
    }
    return keyval; /* 返回键值 */
}
