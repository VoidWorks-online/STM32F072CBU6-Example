# 03SPIBasic

本项目用于演示 STM32F072CBU6 的 SPI 全双工数据收发。

主要使用：SPI1、USART1。

## 引脚定义

| 功能 | 外设信号 | MCU 引脚 | 说明 |
|---|---|---|---|
| SPI 时钟 | SPI1_SCK | PA5 | SPI 时钟输出 |
| SPI 接收 | SPI1_MISO | PA6 | 回环测试输入 |
| SPI 发送 | SPI1_MOSI | PA7 | 回环测试输出 |
| 日志输出 | USART1_TX | PA9 | 输出测试结果 |

