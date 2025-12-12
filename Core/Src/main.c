/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>

#include "../../SYSTEM/USART/usart.h"
#include "../../BSP/KEY/key.h"
#include "../../BSP/NORFLASH/norflash.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* 要写入到FLASH的字符串数组 */
const uint8_t g_text_buf[] = {"STM32 SPI TEST"};

#define TEXT_SIZE sizeof(g_text_buf) /* TEXT字符串长度 */
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
  uint16_t i = 0;
  uint8_t datatemp[TEXT_SIZE];
  uint32_t flashsize;
  uint16_t id = 0;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  GPIO_Init();
  LED_INIT();
  USART1_UART_Init(115200);
  KEY_INIT();
  norflash_init();
  /* USER CODE BEGIN 2 */
  printf("STM32 SPI TEST\r\n");

  id = norflash_read_id(); /* 读取FLASH ID */

  while ((id == 0) || (id == 0XFFFF)) /* 检测不到FLASH芯片 */
  {
    // lcd_show_string(30, 130, 200, 16, 16, "FLASH Check Failed!", RED);
    printf("FLASH Check Failed!\r\n");
    HAL_Delay(500);
    // lcd_show_string(30, 130, 200, 16, 16, "Please Check!      ", RED);
    printf("Please Check!\r\n");
    HAL_Delay(500);
    LED_TogglePin(GPIOB, GPIO_PIN_5); /* LED0闪烁 */
  }

  // lcd_show_string(30, 130, 200, 16, 16, "SPI FLASH Ready!", BLUE);
  printf("SPI FLASH Ready!\r\n");
  flashsize = 16 * 1024 * 1024; /* FLASH 大小为16M字节 */
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    uint8_t key = KEY_SCAN(0);

    if (key == KEY1_PRES) /* KEY1按下,写入 */
    {
      // lcd_fill(0, 150, 239, 319, WHITE); /* 清除半屏 */
      // lcd_show_string(30, 150, 200, 16, 16, "Start Write FLASH....", BLUE);
      printf("Start Write FLASH...\r\n");
      sprintf((char *)datatemp, "%s %d", (char *)g_text_buf, i);
      norflash_write((uint8_t *)datatemp, flashsize - 100, TEXT_SIZE);      /* 从倒数第100个地址处开始,写入SIZE长度的数据 */
      // lcd_show_string(30, 150, 200, 16, 16, "FLASH Write Finished!", BLUE); /* 提示传送完成 */
      printf("FLASH Write Finished!\r\n");
    }

    if (key == KEY0_PRES) /* KEY0按下,读取字符串并显示 */
    {
      // lcd_show_string(30, 150, 200, 16, 16, "Start Read FLASH... . ", BLUE);
      printf("Start Read FLASH...\r\n");
      norflash_read(datatemp, flashsize - 100, TEXT_SIZE);                   /* 从倒数第100个地址处开始,读出SIZE个字节 */
      // lcd_show_string(30, 150, 200, 16, 16, "The Data Readed Is:   ", BLUE); /* 提示传送完成 */
      // lcd_show_string(30, 170, 200, 16, 16, (char *)datatemp, BLUE);         /* 显示读到的字符串 */
      printf("The Data Readed Is: %s\r\n", (char *)datatemp);
    }

    i++;
    if (i == 20) {
      LED_TogglePin(GPIOB, GPIO_PIN_5); /* LED0闪烁 */
      i = 0;
    }
    HAL_Delay(10);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void GPIO_Init(void)
{
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /*Configure GPIO pin Output Level */

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
