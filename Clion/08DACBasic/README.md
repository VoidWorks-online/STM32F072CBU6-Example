# 08DACBasic

本项目用于演示 STM32F072CBU6 通过 DAC 输出模拟锯齿波。

主要使用：DAC、USART1。

系统时钟：内部 HSI48，48 MHz。

## 引脚定义

| 功能 | 外设信号 | MCU 引脚 | 说明 |
|---|---|---|---|
| 模拟输出 | DAC_OUT1 | PA4 | DAC 通道 1 输出 |
| 日志输出 | USART1_TX | PA9 | 输出周期日志 |

