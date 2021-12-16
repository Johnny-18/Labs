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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
#include "stdio.h"
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
UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  uint32_t SIZE_ONE_PAGE = 0x400; // розмір сторінки 1024 байта
  uint32_t ADDR_FLASH_PAGE = 0x08000000 + 31 * SIZE_ONE_PAGE; // адреса 64 сторінки 0x0800FC00

  size_t SIZE_uint32 = sizeof (uint32_t); //4 байти
  //в одну сторінку розміром 1Кб можна записати 1024/4=256 змінних розміром 4 байти
  uint32_t val_000 = 0x01234567;
  uint32_t val_004 = 0x89ABCDEF;
  uint32_t val_00C = 0x01020304;
  uint32_t val_3FC = 0x0000FFFF; //1024 - 4 = 1020

  char word[40] = "Test text by Aradam (Denys Mykhailenko)\0";

  HAL_UART_Transmit (&huart1, (uint8_t*)"\n\n\r!!!Erase page 64!!!\n\r", sizeof ("\n\n\r!!!Erase page 64!!!\n\r"), 100);
  Flash_Erase_One_Page (ADDR_FLASH_PAGE); //очищення 64 сторінки

  HAL_UART_Transmit (&huart1, (uint8_t*)"\n!!!Write!!!\n\r", sizeof ("\n!!!Write!!!\n\r"), 100);
  //запис значень на початку і в кінці 127 сторінки
  Flash_Write_uint32 (ADDR_FLASH_PAGE + 0 * SIZE_uint32, val_000);
  Flash_Write_uint32 (ADDR_FLASH_PAGE + 1 * SIZE_uint32, val_004);
  Flash_Write_uint32 (ADDR_FLASH_PAGE + 3 * SIZE_uint32, val_00C);
  Flash_Write_uint32 (ADDR_FLASH_PAGE + 255 * SIZE_uint32, val_3FC);

  HAL_UART_Transmit (&huart1, (uint8_t*)"\n!!!Erase page 63!!!\n\r", sizeof ("\n!!!Erase page 63!!!\n\r"), 100);
  Flash_Erase_One_Page (ADDR_FLASH_PAGE - SIZE_ONE_PAGE); // очищення 63 сторінки

  HAL_UART_Transmit (&huart1, (uint8_t*)"\n!!!Write!!!\n\r", sizeof ("\n!!!Write!!!\n\r"), 100);
  //запис значень на початку і в кінці 126 сторінки

  Flash_Write_uint32 (ADDR_FLASH_PAGE - SIZE_ONE_PAGE + 0x004, val_004);
  Flash_Write_uint32 (ADDR_FLASH_PAGE - SIZE_ONE_PAGE + 0x00C, val_00C);
  Flash_Write_uint32 (ADDR_FLASH_PAGE - SIZE_ONE_PAGE + 0x3FC, val_3FC);

  HAL_UART_Transmit (&huart1, (uint8_t*)"\n!!!Read!!!\n\r", sizeof ("\n!!!Read!!!\n\r"), 100);

  //читання значень з 64 сторінки Flash-пам'яті
  Flash_Read_uint32 (ADDR_FLASH_PAGE + 0 * SIZE_uint32);
  Flash_Read_uint32 (ADDR_FLASH_PAGE + 1 * SIZE_uint32);
  Flash_Read_uint32 (ADDR_FLASH_PAGE + 3 * SIZE_uint32);
  Flash_Read_uint32 (ADDR_FLASH_PAGE + 255 * SIZE_uint32);

 HAL_UART_Transmit (&huart1, (uint8_t*)"\n!!!Write64!!!\n\r", sizeof ("\n!!!Write64!!!\n\r"), 100);
  Flash_Write_uint64 (ADDR_FLASH_PAGE - SIZE_ONE_PAGE + 0x000, 1844674407370955161);
  Flash_Write_uint8 (0x08007800, word);
  Flash_Read_uint8 (0x08007800, word);
  HAL_Delay (5000);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */
/////////////Функції запису та читання для 1 байтних масивів/////////////
void Flash_Write_uint8 (uint32_t MYADDR, char val[])
{
	char str[64] = { 0, };
	for (int i = 0; i <= strlen (val); i++)
	{
		///////////// ЗАП�?С у Flash-пам'ять ///////////////
		HAL_FLASH_Unlock(); //розблокування Flash-пам'яті

		if (HAL_FLASH_Program (0x0U, MYADDR, val[i]) != HAL_OK)
		{
			uint32_t er = HAL_FLASH_GetError ();
			snprintf (str, 64, "Error Flash_Write(): %lu\n\r", er);
			HAL_UART_Transmit (&huart1, (uint8_t*) str, strlen (str), 100);
			while (1) {}
		}
		MYADDR+=0x001;

		HAL_FLASH_Lock(); //заблокування Flash-пам'яті
	}

	snprintf (str, 64, "Write phrase bits %s to address 0x%08lX OK\n\r", val, MYADDR);
	HAL_UART_Transmit (&huart1, (uint8_t*) str, strlen (str), 100);
}

void Flash_Read_uint8 (uint32_t MYADDR, char val[])
{
	char str[64] = { 0, };

	for (int i = 0; i <= strlen (val); i++)
	{
		/////////////// Ч�?ТАЄМО Flash-пам'ять ///////////////////
		uint8_t dig8 = *(uint8_t*) MYADDR; //читання числа за його адресою
		str[i] = dig8;
		MYADDR += 0x001;
	}

	HAL_UART_Transmit (&huart1, (uint8_t*)"\n!!!Read phrase!!!\n\r", sizeof ("\n!!!Read phrase!!!\n\r"), 100);
	HAL_UART_Transmit (&huart1, (uint8_t*) str, strlen (str), 100);
}

/////////////Функції запису для 16 біт/////////////
void Flash_Write_uint16 (uint32_t MYADDR, uint16_t val)
{
	char str[64] = { 0, };

	///////////// ЗАП�?С у Flash-пам'ять ///////////////
	HAL_FLASH_Unlock (); //розблокування Flash-пам'яті

	if (HAL_FLASH_Program (FLASH_TYPEPROGRAM_HALFWORD, MYADDR, val) != HAL_OK)
	{
		uint32_t er = HAL_FLASH_GetError ();
		snprintf (str, 64, "Error Flash_Write(): %lu\n\r", er);
		HAL_UART_Transmit (&huart1, (uint8_t*) str, strlen (str), 100);
		while (1) {}
	}

	snprintf (str, 64, "Write 16 bits (Hex: 0x%08X) to address 0x%08lX OK\n\r", val, MYADDR);
	HAL_UART_Transmit (&huart1, (uint8_t*) str, strlen (str), 100);

	HAL_FLASH_Lock (); //заблокування Flash-пам'яті
}

/////////////Функції запису читання та стирання сторінки для 32 біт/////////////
void Flash_Erase_One_Page (uint32_t MYADDR)
{
	FLASH_EraseInitTypeDef EraseInitStruct; //структура для очищення флеша

	EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES; //очищення сторінками, FLASH_TYPEERASE_MASSERASE - очищення всього флеша
	EraseInitStruct.PageAddress = MYADDR; //адреса сторінки;
	EraseInitStruct.NbPages = 1;          //кількість сторінок для очищення
	//EraseInitStruct.Banks = FLASH_BANK_1; //для більш потужник МК ніж STM32F103

	uint32_t page_error = 0; //змінна в яку запишеться адреса сторінки, при невдалому очищенні

	char str[64] = { 0, };

	//////// ОЧ�?ЩЕННЯ Flash-пам'яті ////////
	HAL_FLASH_Unlock (); //розблокування Flash-пам'яті

	if (HAL_FLASHEx_Erase (&EraseInitStruct, &page_error) != HAL_OK)
	{
		//сталась помилка при очищенні сторінки
		uint32_t er = HAL_FLASH_GetError ();
		snprintf (str, 64, "Error Flash_Erase(): %lu\n\r", er);
		HAL_UART_Transmit (&huart1, (uint8_t*) str, strlen (str), 100);
		while (1) {}
	}

	//очищення сторінки виконане успішно
	snprintf (str, 64, "Erase Flash from address 0x%08lX to address 0x%08lX OK\n\r", MYADDR, MYADDR + 0x400 - 1);
	HAL_UART_Transmit (&huart1, (uint8_t*) str, strlen (str), 100);

	HAL_FLASH_Lock (); //заблокування Flash-пам'яті
}

void Flash_Write_uint32 (uint32_t MYADDR, uint32_t val)
{
	char str[64] = { 0, };

	/////////////ЗАП�?С у Flash-пам'ять///////////////
	HAL_FLASH_Unlock (); //розблокування Flash-пам'яті

	if (HAL_FLASH_Program (FLASH_TYPEPROGRAM_WORD, MYADDR, val) != HAL_OK)
	{
		uint32_t er = HAL_FLASH_GetError ();
		snprintf (str, 64, "Error Flash_Write(): %lu\n\r", er);
		HAL_UART_Transmit (&huart1, (uint8_t*) str, strlen (str), 100);
		while (1) {}
	}

	snprintf (str, 64, "Write 32 bits (Hex: 0x%08lX) to address 0x%08lX OK\n\r", val, MYADDR);
	HAL_UART_Transmit (&huart1, (uint8_t*) str, strlen (str), 100);

	HAL_FLASH_Lock (); // заблокування Flash-пам'яті
}

uint32_t Flash_Read_uint32(uint32_t MYADDR)
{
	char str[64] = { 0, };

	/////////////// Ч�?ТАЄМО Flash-пам'ять ///////////////////
	uint32_t dig32 = *(uint32_t*) MYADDR; // читання числа за його адресою

	snprintf (str, 64, "Read 32 bits (Dec: %lu \tHex: 0x%08lX)", dig32, dig32);
	HAL_UART_Transmit (&huart1, (uint8_t*) str, strlen (str), 100);
	snprintf (str, 64, " from address 0x%08lX \n\r", MYADDR);
	HAL_UART_Transmit (&huart1, (uint8_t*) str, strlen (str), 100);

	return dig32;
}

/////////////Функції запису для 64біт/////////////

void Flash_Write_uint64(uint32_t MYADDR, uint64_t val)
{
	char str[64] = { 0, };

	/////////////ЗАП�?С у Flash-пам'ять ///////////////
	HAL_FLASH_Unlock (); //розблокування Flash-пам'яті

	if (HAL_FLASH_Program (FLASH_TYPEPROGRAM_DOUBLEWORD, MYADDR, val) != HAL_OK)
	{
		uint32_t er = HAL_FLASH_GetError ();
		snprintf (str, 64, "Error Flash_Write(): %lu\n\r", er);
		HAL_UART_Transmit (&huart1, (uint8_t*) str, strlen (str), 100);
		while (1) {}
	}

	snprintf (str, 64, "Write 64 bits (Hex: 0x%08llX) to address 0x%08lX OK\n\r", val, MYADDR);
	HAL_UART_Transmit (&huart1, (uint8_t*) str, strlen (str), 100);

	HAL_FLASH_Lock (); //заблокування Flash-пам'яті
}
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

