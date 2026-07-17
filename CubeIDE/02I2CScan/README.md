# 02I2CScan

本项目用于演示 STM32F072CBU6 扫描 I2C 总线设备地址。

主要使用：I2C1、USART1。

系统时钟：内部 HSI48，48 MHz。

## 引脚定义

| 功能 | 外设信号 | MCU 引脚 | 说明 |
|---|---|---|---|
| I2C 时钟 | I2C1_SCL | PB6 | 连接目标设备 SCL |
| I2C 数据 | I2C1_SDA | PB7 | 连接目标设备 SDA |
| 日志输出 | USART1_TX | PA9 | 输出扫描结果 |

