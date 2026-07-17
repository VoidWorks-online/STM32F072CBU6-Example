/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usbd_cdc_if.c
  * @version        : v2.0_Cube
  * @brief          : Usb device for Virtual Com Port.
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
#include "usbd_cdc_if.h"

/* USER CODE BEGIN INCLUDE */

/* USER CODE END INCLUDE */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @brief Usb device library.
  * @{
  */

/** @addtogroup USBD_CDC_IF
  * @{
  */

/** @defgroup USBD_CDC_IF_Private_TypesDefinitions USBD_CDC_IF_Private_TypesDefinitions
  * @brief Private types.
  * @{
  */

/* USER CODE BEGIN PRIVATE_TYPES */

/* USER CODE END PRIVATE_TYPES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_Defines USBD_CDC_IF_Private_Defines
  * @brief Private defines.
  * @{
  */

/* USER CODE BEGIN PRIVATE_DEFINES */
/*
 * USB 全速 CDC 端点一次最多收 64 字节。这里准备 8 个独立的数据槽：
 * 接收中断把数据复制进队列，主循环再逐包发送，避免接收缓冲区被覆盖。
 * 队列占用 8 * 64 = 512 字节 RAM；如业务繁忙，可按 RAM 容量调整槽数。
 */
#define CDC_ECHO_QUEUE_DEPTH       8U
#define CDC_ECHO_PACKET_SIZE       CDC_DATA_FS_MAX_PACKET_SIZE
/* USER CODE END PRIVATE_DEFINES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_Macros USBD_CDC_IF_Private_Macros
  * @brief Private macros.
  * @{
  */

/* USER CODE BEGIN PRIVATE_MACRO */

/* USER CODE END PRIVATE_MACRO */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_Variables USBD_CDC_IF_Private_Variables
  * @brief Private variables.
  * @{
  */
/* Create buffer for reception and transmission           */
/* It's up to user to redefine and/or remove those define */
/** Received data over USB are stored in this buffer      */
uint8_t UserRxBufferFS[APP_RX_DATA_SIZE];

/** Data to send over USB CDC are stored in this buffer   */
uint8_t UserTxBufferFS[APP_TX_DATA_SIZE];

/* USER CODE BEGIN PRIVATE_VARIABLES */

/* 单生产者（USB 接收回调）、单消费者（主循环）的环形数据包队列。 */
static uint8_t cdcEchoData[CDC_ECHO_QUEUE_DEPTH][CDC_ECHO_PACKET_SIZE];
static uint16_t cdcEchoLength[CDC_ECHO_QUEUE_DEPTH];

/* volatile 告诉编译器：这些索引可能在中断和主循环两个执行环境中改变。 */
static volatile uint8_t cdcEchoWriteIndex = 0U;
static volatile uint8_t cdcEchoReadIndex = 0U;
static volatile uint8_t cdcEchoSending = 0U;
static volatile uint32_t cdcEchoDroppedPackets = 0U;

/* USER CODE END PRIVATE_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Exported_Variables USBD_CDC_IF_Exported_Variables
  * @brief Public variables.
  * @{
  */

extern USBD_HandleTypeDef hUsbDeviceFS;

/* USER CODE BEGIN EXPORTED_VARIABLES */

/* USER CODE END EXPORTED_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_FunctionPrototypes USBD_CDC_IF_Private_FunctionPrototypes
  * @brief Private functions declaration.
  * @{
  */

static int8_t CDC_Init_FS(void);
static int8_t CDC_DeInit_FS(void);
static int8_t CDC_Control_FS(uint8_t cmd, uint8_t* pbuf, uint16_t length);
static int8_t CDC_Receive_FS(uint8_t* pbuf, uint32_t *Len);

/* USER CODE BEGIN PRIVATE_FUNCTIONS_DECLARATION */

/* USER CODE END PRIVATE_FUNCTIONS_DECLARATION */

/**
  * @}
  */

USBD_CDC_ItfTypeDef USBD_Interface_fops_FS =
{
  CDC_Init_FS,
  CDC_DeInit_FS,
  CDC_Control_FS,
  CDC_Receive_FS
};

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Initializes the CDC media low layer over the FS USB IP
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_Init_FS(void)
{
  /* USER CODE BEGIN 3 */
  /* Set Application Buffers */
  USBD_CDC_SetTxBuffer(&hUsbDeviceFS, UserTxBufferFS, 0);
  USBD_CDC_SetRxBuffer(&hUsbDeviceFS, UserRxBufferFS);
  return (USBD_OK);
  /* USER CODE END 3 */
}

/**
  * @brief  DeInitializes the CDC media low layer
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_DeInit_FS(void)
{
  /* USER CODE BEGIN 4 */
  return (USBD_OK);
  /* USER CODE END 4 */
}

/**
  * @brief  Manage the CDC class requests
  * @param  cmd: Command code
  * @param  pbuf: Buffer containing command data (request parameters)
  * @param  length: Number of data to be sent (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_Control_FS(uint8_t cmd, uint8_t* pbuf, uint16_t length)
{
  /* USER CODE BEGIN 5 */
  switch(cmd)
  {
    case CDC_SEND_ENCAPSULATED_COMMAND:

    break;

    case CDC_GET_ENCAPSULATED_RESPONSE:

    break;

    case CDC_SET_COMM_FEATURE:

    break;

    case CDC_GET_COMM_FEATURE:

    break;

    case CDC_CLEAR_COMM_FEATURE:

    break;

  /*******************************************************************************/
  /* Line Coding Structure                                                       */
  /*-----------------------------------------------------------------------------*/
  /* Offset | Field       | Size | Value  | Description                          */
  /* 0      | dwDTERate   |   4  | Number |Data terminal rate, in bits per second*/
  /* 4      | bCharFormat |   1  | Number | Stop bits                            */
  /*                                        0 - 1 Stop bit                       */
  /*                                        1 - 1.5 Stop bits                    */
  /*                                        2 - 2 Stop bits                      */
  /* 5      | bParityType |  1   | Number | Parity                               */
  /*                                        0 - None                             */
  /*                                        1 - Odd                              */
  /*                                        2 - Even                             */
  /*                                        3 - Mark                             */
  /*                                        4 - Space                            */
  /* 6      | bDataBits  |   1   | Number Data bits (5, 6, 7, 8 or 16).          */
  /*******************************************************************************/
    case CDC_SET_LINE_CODING:

    break;

    case CDC_GET_LINE_CODING:

    break;

    case CDC_SET_CONTROL_LINE_STATE:

    break;

    case CDC_SEND_BREAK:

    break;

  default:
    break;
  }

  return (USBD_OK);
  /* USER CODE END 5 */
}

/**
  * @brief  Data received over USB OUT endpoint are sent over CDC interface
  *         through this function.
  *
  *         @note
  *         This function will issue a NAK packet on any OUT packet received on
  *         USB endpoint until exiting this function. If you exit this function
  *         before transfer is complete on CDC interface (ie. using DMA controller)
  *         it will result in receiving more data while previous ones are still
  *         not sent.
  *
  * @param  Buf: Buffer of data to be received
  * @param  Len: Number of data received (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_Receive_FS(uint8_t* Buf, uint32_t *Len)
{
  /* USER CODE BEGIN 6 */
  uint8_t nextWriteIndex;
  uint32_t copyLength = *Len;

  /*
   * 此函数由 USB 接收流程调用，执行时间应尽量短，不能在这里 while 等待发送。
   * CDC_Transmit_FS() 是异步发送，直接传入 Buf 后 Buf 可能被下一包改写，
   * 所以先把本包复制到自有队列，再由主循环 CDC_EchoProcess_FS() 发出。
   */
  if (copyLength > CDC_ECHO_PACKET_SIZE)
  {
    /* 理论上全速端点不会超过 64 字节；这里防止异常长度导致数组越界。 */
    copyLength = CDC_ECHO_PACKET_SIZE;
  }

  nextWriteIndex = (uint8_t)((cdcEchoWriteIndex + 1U) % CDC_ECHO_QUEUE_DEPTH);
  if (nextWriteIndex != cdcEchoReadIndex)
  {
    memcpy(cdcEchoData[cdcEchoWriteIndex], Buf, copyLength);
    cdcEchoLength[cdcEchoWriteIndex] = (uint16_t)copyLength;

    /* 先写数据和长度，最后发布写索引，主循环才会看到完整的数据包。 */
    __DMB();
    cdcEchoWriteIndex = nextWriteIndex;
  }
  else
  {
    /* 主循环长期不运行或主机连续突发发送时，队列可能装满。 */
    cdcEchoDroppedPackets++;
  }

  /* 必须重新挂接接收缓冲区并打开 OUT 端点，否则主机后续数据无法进入。 */
  USBD_CDC_SetRxBuffer(&hUsbDeviceFS, &Buf[0]);
  USBD_CDC_ReceivePacket(&hUsbDeviceFS);
  return (USBD_OK);
  /* USER CODE END 6 */
}

/**
  * @brief  CDC_Transmit_FS
  *         Data to send over USB IN endpoint are sent over CDC interface
  *         through this function.
  *         @note
  *
  *
  * @param  Buf: Buffer of data to be sent
  * @param  Len: Number of data to be sent (in bytes)
  * @retval USBD_OK if all operations are OK else USBD_FAIL or USBD_BUSY
  */
uint8_t CDC_Transmit_FS(uint8_t* Buf, uint16_t Len)
{
  uint8_t result = USBD_OK;
  /* USER CODE BEGIN 7 */
  USBD_CDC_HandleTypeDef *hcdc = (USBD_CDC_HandleTypeDef*)hUsbDeviceFS.pClassData;
  /* 枚举尚未完成时 pClassData 可能为空，先判断可避免空指针访问。 */
  if ((hcdc == NULL) || (hcdc->TxState != 0U)){
    return USBD_BUSY;
  }
  USBD_CDC_SetTxBuffer(&hUsbDeviceFS, Buf, Len);
  result = USBD_CDC_TransmitPacket(&hUsbDeviceFS);
  /* USER CODE END 7 */
  return result;
}

/* USER CODE BEGIN PRIVATE_FUNCTIONS_IMPLEMENTATION */

void CDC_EchoProcess_FS(void)
{
  USBD_CDC_HandleTypeDef *hcdc;

  /* USB 尚未完成枚举/配置时不处理，主循环下次会再次尝试。 */
  hcdc = (USBD_CDC_HandleTypeDef *)hUsbDeviceFS.pClassData;
  if (hcdc == NULL)
  {
    return;
  }

  if (cdcEchoSending != 0U)
  {
    /*
     * TxState 清零表示上一包已经真正发送完成。只有此时才能释放队列槽；
     * 提前移动读索引会允许接收回调覆盖 USB 外设仍在使用的发送数据。
     */
    if (hcdc->TxState != 0U)
    {
      return;
    }

    cdcEchoReadIndex =
        (uint8_t)((cdcEchoReadIndex + 1U) % CDC_ECHO_QUEUE_DEPTH);
    cdcEchoSending = 0U;
  }

  if (cdcEchoReadIndex == cdcEchoWriteIndex)
  {
    return;                         /* 队列为空，没有数据需要回显。 */
  }

  __DMB();
  if (CDC_Transmit_FS(cdcEchoData[cdcEchoReadIndex],
                      cdcEchoLength[cdcEchoReadIndex]) == USBD_OK)
  {
    cdcEchoSending = 1U;            /* 保留当前槽，直到异步发送完成。 */
  }
}

uint32_t CDC_EchoGetDroppedPackets_FS(void)
{
  return cdcEchoDroppedPackets;
}

/* USER CODE END PRIVATE_FUNCTIONS_IMPLEMENTATION */

/**
  * @}
  */

/**
  * @}
  */
