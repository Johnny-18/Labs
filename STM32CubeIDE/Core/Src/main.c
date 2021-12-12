/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
struct Led
{
	GPIO_TypeDef *GPIOx;
	uint16_t pin;
};
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

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
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  const int LED_COUNT = 4;
  struct Led LEDS[LED_COUNT];

  LEDS[0].GPIOx = LED1_GPIO_Port;
  LEDS[0].pin = LED1_Pin;
  LEDS[1].GPIOx = LED2_GPIO_Port;
  LEDS[1].pin = LED2_Pin;
  LEDS[2].GPIOx = LED3_GPIO_Port;
  LEDS[2].pin = LED3_Pin;
  LEDS[3].GPIOx = LED4_GPIO_Port;
  LEDS[3].pin = LED4_Pin;

  int mode = 0;
  int onPress = -1;
  while (1)
  {
	  int newMode = 0;
	  if (HAL_GPIO_ReadPin (BUTTON1_GPIO_Port, BUTTON1_Pin) == GPIO_PIN_SET)
	  {
		  newMode = 1;
		  if (onPress < 0)
		  {
			  onPress = 1;
		  }
	  } else if (HAL_GPIO_ReadPin (BUTTON2_GPIO_Port, BUTTON2_Pin) == GPIO_PIN_SET)
	  {
		  newMode = 2;
		  if (onPress < 0)
		  {
			  onPress = 1;
		  }
	  }

	  if (!newMode)
	  {
		  onPress = -1;
	  }

	  if ((newMode) && (mode != newMode))
	  {
		  for (int i = 0; i < LED_COUNT; i++)
		  {
			  struct Led led = LEDS[i];
			  HAL_GPIO_WritePin (led.GPIOx, led.pin, GPIO_PIN_RESET);
		  }

		  mode = newMode;
	  }

	  if (onPress == 1)
	  {
		  if (mode == 1)
		  {
			  for (int i = 0, found = 0; ; i++)
			  {
				  struct Led led = LEDS[i % LED_COUNT];

				  if (found)
				  {
					  HAL_GPIO_WritePin (led.GPIOx, led.pin, GPIO_PIN_SET);
					  break;
				  }

				  if ((HAL_GPIO_ReadPin (led.GPIOx, led.pin) == GPIO_PIN_SET) || (i == LED_COUNT - 1))
				  {
					  HAL_GPIO_WritePin (led.GPIOx, led.pin, GPIO_PIN_RESET);
					  found = 1;
				  }
			  }
		  } else if (mode == 2)
		  {
			  int changed = 0;
			  for (int i = 0; i < LED_COUNT; i++)
			  {
				  struct Led led = LEDS[i % LED_COUNT];

				  if (HAL_GPIO_ReadPin (led.GPIOx, led.pin) == GPIO_PIN_RESET)
				  {
					  HAL_GPIO_WritePin (led.GPIOx, led.pin, GPIO_PIN_SET);
					  changed = 1;
					  break;
				  }
			  }

			  if (!changed)
			  {
				  for (int i = 0; i < LED_COUNT; i++)
				  {
					  struct Led led = LEDS[i];
					  HAL_GPIO_WritePin (led.GPIOx, led.pin, GPIO_PIN_RESET);
				  }
			  }
		  }

		  onPress = 0;
	  }
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

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
