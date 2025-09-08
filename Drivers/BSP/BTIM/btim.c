//
// Created by fcant on 2025/9/8 星期一.
//

#include "btim.h"

TIM_HandleTypeDef htim6;

/**
  * @brief TIM6 Initialization Function
  * @param None
  * @retval None
  */
void TIM6_Init(void) {

    /* USER CODE BEGIN TIM6_Init 0 */

    /* USER CODE END TIM6_Init 0 */

    // TIM_MasterConfigTypeDef sMasterConfig = {0};

    /* USER CODE BEGIN TIM6_Init 1 */

    /* USER CODE END TIM6_Init 1 */
    htim6.Instance = TIM6;
    htim6.Init.Prescaler = 7200-1;
    // htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim6.Init.Period = 5000-1;
    // htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
    {
    }
    HAL_TIM_Base_Start_IT(&htim6);
    // sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    // sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    // if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
    // {
    // }
    /* USER CODE BEGIN TIM6_Init 2 */

    /* USER CODE END TIM6_Init 2 */

}

/**
  * @brief TIM_Base MSP Initialization
  * This function configures the hardware resources used in this example
  * @param htim_base: TIM_Base handle pointer
  * @retval None
  */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
{
    if(htim_base->Instance==TIM6)
    {
        /* USER CODE BEGIN TIM6_MspInit 0 */

        /* USER CODE END TIM6_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_TIM6_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM6_IRQn, 1, 3);
        HAL_NVIC_EnableIRQ(TIM6_IRQn);

    }

}

/**
  * @brief TIM_Base MSP De-Initialization
  * This function freeze the hardware resources used in this example
  * @param htim_base: TIM_Base handle pointer
  * @retval None
  */
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base)
{
    if(htim_base->Instance==TIM6)
    {
        /* USER CODE BEGIN TIM6_MspDeInit 0 */

        /* USER CODE END TIM6_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_TIM6_CLK_DISABLE();
        /* USER CODE BEGIN TIM6_MspDeInit 1 */

        /* USER CODE END TIM6_MspDeInit 1 */
    }
}

/*
 * 定时器6中断服务函数
 */
void TIM6_IRQHandler(void) {
    HAL_TIM_IRQHandler(&htim6);
}

/*
 * 定时器溢出中断回调函数
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance==TIM6) {
        LED_TogglePin(GPIOB, GPIO_PIN_5);
    }
}













