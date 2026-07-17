# 05CANBasic

本项目用于演示 STM32F072CBU6 的 CAN 标准帧通信，默认使用内部回环模式。

主要使用：CAN、USART1。

系统时钟：内部 HSI48，48 MHz。

## 引脚定义

| 功能 | 外设信号 | MCU 引脚 | 说明 |
|---|---|---|---|
| CAN 接收 | CAN_RX | PA11 | 连接 CAN 收发器 RXD |
| CAN 发送 | CAN_TX | PA12 | 连接 CAN 收发器 TXD |
| 日志输出 | USART1_TX | PA9 | 输出通信结果 |

