//
// Created by fcant on 2025/9/5 星期五.
//

#include "wdg.h"

WWDG_HandleTypeDef hwwdg;

/**
  * 窗口看门狗初始化函数
  * @brief WWDG Initialization Function
  * @param None
  * @retval None
  */
void WWDG_Init(void){

    /* USER CODE BEGIN WWDG_Init 0 */

    /* USER CODE END WWDG_Init 0 */

    /* USER CODE BEGIN WWDG_Init 1 */

    /* USER CODE END WWDG_Init 1 */
    hwwdg.Instance = WWDG;
    hwwdg.Init.Prescaler = WWDG_PRESCALER_8;
    hwwdg.Init.Window = 0x5f;
    hwwdg.Init.Counter = 0x7f;
    hwwdg.Init.EWIMode = WWDG_EWI_ENABLE;
    if (HAL_WWDG_Init(&hwwdg) != HAL_OK)
    {
    }
    /* USER CODE BEGIN WWDG_Init 2 */

    /* USER CODE END WWDG_Init 2 */

}

/**
  * WWDG MSP回调函数
  * @brief WWDG MSP Initialization
  * This function configures the hardware resources used in this example
  * @param hwwdg: WWDG handle pointer
  * @retval None
  */
void HAL_WWDG_MspInit(WWDG_HandleTypeDef* hwwdg)
{
    if(hwwdg->Instance==WWDG)
    {
        /* USER CODE BEGIN WWDG_MspInit 0 */

        /* USER CODE END WWDG_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_WWDG_CLK_ENABLE();
        /* USER CODE BEGIN WWDG_MspInit 1 */
        HAL_NVIC_SetPriority(WWDG_IRQn, 2, 3);
        HAL_NVIC_EnableIRQ(WWDG_IRQn);
        /* USER CODE END WWDG_MspInit 1 */

    }

}

/* WWDG中断服务函数 */
void WWDG_IRQHandler(void) {
    HAL_WWDG_IRQHandler(&hwwdg);
}

/* WWDG提前唤醒回调函数 */
void HAL_WWDG_EarlyWakeupCallback(WWDG_HandleTypeDef *hwwdg) {
    // 中断里及时喂狗不会产生复位
    // HAL_WWDG_Refresh(hwwdg);
    // printf("HAL_WWDG_EarlyWakeupCallback WWDG_Refresh\r\n");
}

void WWDG_Refresh(void) {
    HAL_WWDG_Refresh(&hwwdg);
}
