/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include <string.h>
/*
 * bxCAN 回环测试：CAN_RX=PA11、CAN_TX=PA12，USART1 TX=PA9。
 * 默认内部回环不需要外部收发器；正常总线必须使用 CAN 收发器，
 * MCU 引脚不能直连 CANH/CANL，总线两端通常各使用 120 欧姆终端电阻。
 */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* 1：保持内部回环；0：启动时重新初始化为正常总线模式。 */
#define CAN_USE_LOOPBACK 1U
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
CAN_HandleTypeDef hcan;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_CAN_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */
static void App_Init(void);
static void App_Loop(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/** @brief 通过 USART1 输出 CAN 测试日志，单次阻塞最长 100 ms。 */
int _write(int file,char *ptr,int len){(void)file;if(ptr==NULL||len<=0)return 0;return HAL_UART_Transmit(&huart1,(uint8_t*)ptr,(uint16_t)len,100U)==HAL_OK?len:-1;}
#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
int fputc(int ch, FILE *stream){uint8_t byte=(uint8_t)ch;(void)stream;return HAL_UART_Transmit(&huart1,&byte,1U,100U)==HAL_OK?ch:EOF;}
#endif
/*
 * 配置放行测试帧进入 FIFO0 的 32 位掩码过滤器并启动 CAN。
 * CAN 时钟 8 MHz，Prescaler=1，BS1=13 TQ，BS2=2 TQ：
 * 波特率=8 MHz/[1×(1+13+2)]=500 kbit/s，采样点=(1+13)/16=87.5%。
 * SJW=1 TQ，限制每次重同步的最大校正量。任一关键 HAL 操作失败后
 * 继续收发没有意义，因此进入 Error_Handler()，调试可在其中设置断点。
 */
static void App_Init(void){CAN_FilterTypeDef f={0};if(CAN_USE_LOOPBACK==0U){if(HAL_CAN_DeInit(&hcan)!=HAL_OK)Error_Handler();hcan.Init.Mode=CAN_MODE_NORMAL;if(HAL_CAN_Init(&hcan)!=HAL_OK)Error_Handler();}f.FilterBank=0;f.FilterMode=CAN_FILTERMODE_IDMASK;f.FilterScale=CAN_FILTERSCALE_32BIT;f.FilterFIFOAssignment=CAN_FILTER_FIFO0;f.FilterActivation=ENABLE;if(HAL_CAN_ConfigFilter(&hcan,&f)!=HAL_OK||HAL_CAN_Start(&hcan)!=HAL_OK)Error_Handler();}
/*
 * 发送标准 ID 0x123、DLC=8 的数据帧，再从 FIFO0 取回并逐项比较。
 * 轮询上限 100 ms，避免无响应时永久阻塞；超时或任一字段不同均打印 FAIL。
 */
static void App_Loop(void){static const uint8_t tx[8]={0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88};uint8_t rx[8]={0};CAN_TxHeaderTypeDef th={0};CAN_RxHeaderTypeDef rh={0};uint32_t mb;th.StdId=0x123;th.IDE=CAN_ID_STD;th.RTR=CAN_RTR_DATA;th.DLC=8;printf("\r\nCAN loopback test\r\nTX ID: 0x123, Data: 11 22 33 44 55 66 77 88\r\n");HAL_StatusTypeDef s=HAL_CAN_AddTxMessage(&hcan,&th,(uint8_t*)tx,&mb);uint32_t start=HAL_GetTick();while(s==HAL_OK&&HAL_CAN_GetRxFifoFillLevel(&hcan,CAN_RX_FIFO0)==0U&&(HAL_GetTick()-start)<100U){}if(s==HAL_OK&&HAL_CAN_GetRxFifoFillLevel(&hcan,CAN_RX_FIFO0)>0U)s=HAL_CAN_GetRxMessage(&hcan,CAN_RX_FIFO0,&rh,rx);else s=HAL_ERROR;if(s==HAL_OK)printf("RX ID: 0x%03lX, Data: %02X %02X %02X %02X %02X %02X %02X %02X\r\n",(unsigned long)rh.StdId,rx[0],rx[1],rx[2],rx[3],rx[4],rx[5],rx[6],rx[7]);printf("Result: %s\r\n",(s==HAL_OK&&rh.StdId==0x123&&rh.DLC==8&&memcmp(tx,rx,8)==0)?"PASS":"FAIL");HAL_Delay(1000U);}
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
  MX_CAN_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  App_Init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    App_Loop();
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI48;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief CAN Initialization Function
  * @param None
  * @retval None
  */
static void MX_CAN_Init(void)
{

  /* USER CODE BEGIN CAN_Init 0 */

  /* USER CODE END CAN_Init 0 */

  /* USER CODE BEGIN CAN_Init 1 */

  /* USER CODE END CAN_Init 1 */
  hcan.Instance = CAN;
  hcan.Init.Prescaler = 6;
  hcan.Init.Mode = CAN_MODE_LOOPBACK;
  hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan.Init.TimeSeg1 = CAN_BS1_13TQ;
  hcan.Init.TimeSeg2 = CAN_BS2_2TQ;
  hcan.Init.TimeTriggeredMode = DISABLE;
  hcan.Init.AutoBusOff = DISABLE;
  hcan.Init.AutoWakeUp = DISABLE;
  hcan.Init.AutoRetransmission = DISABLE;
  hcan.Init.ReceiveFifoLocked = DISABLE;
  hcan.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN_Init 2 */

  /* USER CODE END CAN_Init 2 */

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
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
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
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

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
#ifdef USE_FULL_ASSERT
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
