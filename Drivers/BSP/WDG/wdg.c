//
// Created by fcant on 2025/9/2 星期二.
//

#include "wdg.h"

IWDG_HandleTypeDef hiwdg;

/*IWDG初始化函数*/
void IWDG_Init(void) {

    hiwdg.Instance = IWDG;
    hiwdg.Init.Prescaler = IWDG_PRESCALER_32;
    hiwdg.Init.Reload = 1250;
    /*溢出时间约为1s*/
    if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
    {

    }
}

/*喂狗函数*/
void IWDG_Feed(void) {
    HAL_IWDG_Refresh(&hiwdg);
}