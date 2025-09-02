//
// Created by fcant on 2025/9/2 ���ڶ�.
//

#include "wdg.h"

IWDG_HandleTypeDef hiwdg;

/*IWDG��ʼ������*/
void IWDG_Init(void) {

    hiwdg.Instance = IWDG;
    hiwdg.Init.Prescaler = IWDG_PRESCALER_32;
    hiwdg.Init.Reload = 1250;
    /*���ʱ��ԼΪ1s*/
    if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
    {

    }
}

/*ι������*/
void IWDG_Feed(void) {
    HAL_IWDG_Refresh(&hiwdg);
}