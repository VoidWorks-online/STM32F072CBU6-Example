# VoidCore F0 STM32F072CBU6 示例库

本仓库收录 **VoidCore F0 – STM32F072CBU6** 开发板的 STM32 HAL 示例工程，用于学习和验证 GPIO、UART、I2C、SPI、ADC、CAN、PWM、USB Device CDC 和 DAC。当前 9 个示例均提供 CLion/CMake、STM32CubeIDE 和 Keil MDK-ARM 工程。

> 开发板的引脚图、原理图、尺寸及完整硬件说明，请查看 [VoidCore F0 – STM32F072CBU6 官方文档](https://docs.voidworks.link/docs/VoidCore/ST/F0/voidcore-f0-stm32f072cbu6)。

## 开发板简介

| 项目 | 参数 |
| --- | --- |
| 主控 | STM32F072CBU6 |
| 内核 / 主频 | Arm Cortex-M0 / 最高 48 MHz |
| 存储 | 128 KB Flash / 16 KB RAM |
| 封装 | UFQFPN-48 |
| 供电 | USB Type-C 5 V 输入，板载 3.3 V LDO |
| 调试 | SWD（+5V、GND、RST、SWCLK、SWDIO） |
| 主要外设 | USB 2.0 FS、CAN、USART、SPI/I2S、I2C、ADC、DAC、定时器 |

板载白色 LED 是电源指示灯，上电后常亮，并非由 MCU 控制的用户 LED。GPIO 点灯示例需要在指定引脚外接 LED 和限流电阻。

## 示例列表

示例按照开发环境分类存放。同一功能在不同 IDE 下的外设配置和运行效果应保持一致，但工程文件、启动文件及链接脚本可能不同。

### CLion / CMake

工程位于 [`Clion`](./Clion) 目录，使用 CMake、Ninja 和 GNU Arm Embedded Toolchain 构建。

| 编号 | 示例 | 主要资源 | 功能说明 |
| --- | --- | --- | --- |
| 00 | [GPIO Blink](./Clion/00GPIOBlink) | PA0 | 每 500 ms 翻转一次 GPIO，完整周期约 1 s（约 1 Hz） |
| 01 | [UART Log](./Clion/01UARTLog) | PA9 / USART1_TX、PA10 / USART1_RX | 通过 USART1 输出启动信息、运行时间和递增计数 |
| 02 | [I2C Scan](./Clion/02I2CScan) | PB6 / I2C1_SCL、PB7 / I2C1_SDA、PA9 / USART1_TX | 扫描 `0x08`～`0x77` 的 7 位 I2C 地址并输出结果 |
| 03 | [SPI Basic](./Clion/03SPIBasic) | PA5 / SPI1_SCK、PA6 / SPI1_MISO、PA7 / SPI1_MOSI、PA9 / USART1_TX | 通过 MISO/MOSI 回环验证 SPI1 全双工收发 |
| 04 | [ADC Basic](./Clion/04ADCBasic) | PA0 / ADC_IN0、PA9 / USART1_TX | 周期采集 12 位 ADC 原始值并换算为毫伏 |
| 05 | [CAN Basic](./Clion/05CANBasic) | PA11 / CAN_RX、PA12 / CAN_TX、PA9 / USART1_TX | 默认使用 bxCAN 内部回环发送和校验标准数据帧 |
| 06 | [PWM Basic](./Clion/06PWMBasic) | PA8 / TIM1_CH1、PA0 / TIM2_CH1 | 两路约 1 kHz、50% 占空比 PWM 输出 |
| 07 | [USB CDC](./Clion/07USBCDC) | PA11 / USB_DM、PA12 / USB_DP | 枚举为 USB 虚拟串口，并以非阻塞队列原样回显收到的数据 |
| 08 | [DAC Basic](./Clion/08DACBasic) | PA4 / DAC_OUT1、PA9 / USART1_TX | 软件更新 DAC 码值，输出约 0.98 Hz 的锯齿波 |

USB CDC 示例的设计、CubeMX 配置和测试方法详见 [`Clion/07USBCDC/USB_CDC_ECHO_使用说明.md`](./Clion/07USBCDC/USB_CDC_ECHO_%E4%BD%BF%E7%94%A8%E8%AF%B4%E6%98%8E.md)。

### STM32CubeIDE

工程位于 [`CubeIDE`](./CubeIDE) 目录，可直接导入 STM32CubeIDE。

| 编号 | 示例 | 主要资源 | 功能说明 |
| --- | --- | --- | --- |
| 00 | [GPIO Blink](./CubeIDE/00GPIOBlink) | PA0 | 每 500 ms 翻转一次 GPIO，功能与 CLion 版本一致 |
| 01 | [UART Log](./CubeIDE/01UARTLog) | PA9 / USART1_TX、PA10 / USART1_RX | USART1 周期日志输出 |
| 02 | [I2C Scan](./CubeIDE/02I2CScan) | PB6 / I2C1_SCL、PB7 / I2C1_SDA、PA9 / USART1_TX | I2C 7 位地址扫描 |
| 03 | [SPI Basic](./CubeIDE/03SPIBasic) | PA5 / SPI1_SCK、PA6 / SPI1_MISO、PA7 / SPI1_MOSI、PA9 / USART1_TX | SPI1 全双工回环测试 |
| 04 | [ADC Basic](./CubeIDE/04ADCBasic) | PA0 / ADC_IN0、PA9 / USART1_TX | 12 位 ADC 采样与电压换算 |
| 05 | [CAN Basic](./CubeIDE/05CANBasic) | PA11 / CAN_RX、PA12 / CAN_TX、PA9 / USART1_TX | bxCAN 标准帧内部回环测试 |
| 06 | [PWM Basic](./CubeIDE/06PWMBasic) | PA8 / TIM1_CH1、PA0 / TIM2_CH1 | 两路约 1 kHz、50% 占空比 PWM 输出 |
| 07 | [USB CDC](./CubeIDE/07USBCDC) | PA11 / USB_DM、PA12 / USB_DP | USB CDC 虚拟串口非阻塞回显，功能与 CLion 版本一致 |
| 08 | [DAC Basic](./CubeIDE/08DACBasic) | PA4 / DAC_OUT1、PA9 / USART1_TX | 软件锯齿波输出 |

STM32CubeIDE 版本的 USB CDC 说明见 [`CubeIDE/07USBCDC/USB_CDC_ECHO_使用说明.md`](./CubeIDE/07USBCDC/USB_CDC_ECHO_%E4%BD%BF%E7%94%A8%E8%AF%B4%E6%98%8E.md)。

### Keil5 / MDK-ARM

工程位于 [`Keil`](./Keil) 目录，使用 Keil MDK-ARM V5 工程格式。9 个示例的编号、引脚和运行效果与上表一致；工程入口位于各示例的 `MDK-ARM/*.uvprojx`，例如 [`Keil/00GPIOBlink/MDK-ARM/00GPIOBlink.uvprojx`](./Keil/00GPIOBlink/MDK-ARM/00GPIOBlink.uvprojx)。

## 快速开始

### 准备工作

- VoidCore F0 STM32F072CBU6 开发板
- USB Type-C 数据线
- 支持 SWD 的调试器（如 ST-Link 或兼容调试器）
- GNU Arm Embedded Toolchain、CMake 和 Ninja（使用当前 `Clion` 工程时）
- STM32CubeIDE（使用 `CubeIDE` 工程时）
- Keil MDK-ARM V5 与对应 STM32F0 Device Family Pack（使用 `Keil` 工程时）
- STM32CubeMX（需要查看或修改 `.ioc` 配置时）

### SWD 接线

| 调试器 | 开发板 | MCU 引脚 |
| --- | --- | --- |
| +5V | +5V | — |
| GND | GND | — |
| RST | RST | NRST |
| SWCLK | SWCLK | PA14 |
| SWDIO | SWDIO | PA13 |

连接前请确认调试器的供电方式和电压设置，避免由 USB 与调试器重复供电造成冲突。如果开发板已通过 USB Type-C 供电，调试器通常只需连接电压参考、GND、RST、SWCLK 和 SWDIO；具体以所用调试器说明为准。

### 使用 CLion

1. 在 CLion 中打开所需示例目录，例如 `Clion/00GPIOBlink`。
2. 选择 `Debug` 或 `Release` CMake Preset。
3. 确认 CLion 能找到 `arm-none-eabi-gcc`、CMake 和 Ninja。
4. 编译工程，并使用已配置的 OpenOCD/ST-Link 下载配置烧录和调试。

也可以在终端中构建：

```bash
cd Clion/00GPIOBlink
cmake --preset Debug
cmake --build --preset Debug
```

生成的 ELF、HEX 和 BIN 文件位于示例的 `build/Debug/` 目录。

### 使用 STM32CubeIDE

1. 启动 STM32CubeIDE，选择 **File > Import**。
2. 选择 **General > Existing Projects into Workspace**。
3. 将根目录指向示例目录，例如 `CubeIDE/02I2CScan`，导入后编译、下载。

每个示例目录直接包含 `.project`、`.cproject`、`.ioc`、链接脚本、`Core` 和驱动代码。需要使用 STM32CubeMX 重新生成代码时，建议先提交或备份修改，并保留 `USER CODE` 区域中的自定义代码。

### 使用 Keil MDK-ARM

1. 安装 Keil MDK-ARM V5 及 STM32F0 Device Family Pack。
2. 打开所需示例的 `MDK-ARM/*.uvprojx`，例如 `Keil/00GPIOBlink/MDK-ARM/00GPIOBlink.uvprojx`。
3. 选择并配置实际使用的 SWD 调试器，然后编译、下载和调试。

不同 IDE 版本的示例应保持相同的外设配置和功能，但工程文件、启动文件及链接脚本不可直接混用。各环境应打开自身目录中的 `.ioc` 修改配置，以免生成器覆盖其他环境的工程文件。

## 示例验证

带日志输出的 `01`～`05`、`08` 示例默认使用 USART1，参数为 **115200 8N1、无硬件流控**。将 PA9（USART1_TX）连接到 3.3 V USB-UART 模块的 RX，并将两者 GND 相连；需要使用接收功能时，再将 PA10（USART1_RX）连接到模块的 TX。

### GPIO Blink

在 PA0 与 GND 之间连接 LED 和限流电阻，烧录后可观察到闪烁。程序每 500 ms 翻转一次 PA0，因此高低电平完整周期约为 1 s（约 1 Hz）。也可直接用逻辑分析仪或示波器观察。

### UART Log

连接 USB-UART 后打开 115200 8N1 串口。程序启动时输出芯片和系统时钟信息，随后每秒输出运行时间与递增计数。

### I2C Scan

将目标设备的 SCL、SDA 分别连接至 PB6、PB7，并共地。I2C 总线必须通过合适的电阻上拉至 3.3 V；程序每约 2 秒扫描一次 `0x08`～`0x77`，结果由 USART1 输出。

### SPI Basic

短接 PA6（MISO）与 PA7（MOSI），通过 USART1 查看回环结果。程序周期发送固定测试数据，接收内容完全一致时输出 `PASS`；PA5 可用于观察 SPI 时钟。

### ADC Basic

向 PA0 输入 `0 V`～`VDDA` 范围内的模拟电压。程序每约 500 ms 输出一次 12 位 ADC 原始值，以及按 `VDDA = 3.3 V` 估算的电压；请勿让 PA0 超出允许的电压范围。

### CAN Basic

默认固件工作于 bxCAN 内部回环模式，无需外接 CAN 收发器，USART1 每秒输出一次发送、接收和 `PASS`/`FAIL` 结果。切换到正常总线模式后必须外接 3.3 V 逻辑兼容的 CAN 收发器，MCU 的 PA11、PA12 不可直接连接 CANH、CANL；默认位速率为 500 kbit/s。

### PWM Basic

烧录后使用示波器或逻辑分析仪测量：

- PA8：TIM1_CH1
- PA0：TIM2_CH1

两路默认均为约 1 kHz、50% 占空比的 PWM 信号。

### USB CDC Echo

1. 烧录 `07USBCDC` 固件。
2. 通过带数据功能的 USB Type-C 线连接开发板与电脑。
3. Linux 通常会出现 `/dev/ttyACM0`，Windows 通常会出现新的 COM 端口。
4. 使用串口工具发送文本或十六进制数据，设备应原样返回。

Linux 可使用以下命令测试：

```bash
python3 -m serial.tools.miniterm /dev/ttyACM0 115200
```

USB CDC 未桥接物理 UART，因此串口工具中选择的波特率通常不影响实际 USB 传输。

### DAC Basic

使用示波器或高阻万用表测量 PA4。程序以 64 为步进更新 12 位 DAC 码值，每步约 16 ms，输出理论周期约 1.024 s（约 0.98 Hz）的锯齿波，并在每个周期结束时通过 USART1 输出日志。PA4 不适合直接驱动电机、扬声器等大负载。

## 重要引脚与注意事项

- PA11、PA12 分别用于原生 USB D-、D+；进行 USB 实验时不要复用。
- PA13、PA14 分别用于 SWDIO、SWCLK；调试阶段建议保留。
- STM32F072 提供 CAN 控制器，但接入 CAN 总线仍需外置 CAN 收发器。
- GPIO 为 3.3 V 逻辑电平，外接模块前请确认其电压兼容性。
- 修改时钟树后，应特别检查 USB 所需的准确 48 MHz 时钟。
- 板级连线、引脚复用和电气限制以官方文档、原理图及 ST 数据手册为准。

## 工程结构

```text
STM32F072CBU6/
├── Clion/
│   ├── 00GPIOBlink/       # GPIO 翻转
│   ├── 01UARTLog/         # USART1 日志输出
│   ├── 02I2CScan/         # I2C 地址扫描
│   ├── 03SPIBasic/        # SPI 全双工回环
│   ├── 04ADCBasic/        # ADC 模拟采样
│   ├── 05CANBasic/        # CAN 标准帧回环
│   ├── 06PWMBasic/        # TIM1/TIM2 PWM 输出
│   ├── 07USBCDC/          # USB CDC 虚拟串口回显
│   └── 08DACBasic/        # DAC 锯齿波输出
├── CubeIDE/
│   ├── 00GPIOBlink/       # STM32CubeIDE GPIO 翻转工程
│   ├── 01UARTLog/         # STM32CubeIDE USART1 日志工程
│   ├── 02I2CScan/         # STM32CubeIDE I2C 扫描工程
│   ├── 03SPIBasic/        # STM32CubeIDE SPI 回环工程
│   ├── 04ADCBasic/        # STM32CubeIDE ADC 采样工程
│   ├── 05CANBasic/        # STM32CubeIDE CAN 回环工程
│   ├── 06PWMBasic/        # STM32CubeIDE PWM 工程
│   ├── 07USBCDC/          # STM32CubeIDE USB CDC 回显工程
│   └── 08DACBasic/        # STM32CubeIDE DAC 输出工程
├── Keil/
│   ├── 00GPIOBlink/       # Keil GPIO 翻转工程
│   ├── 01UARTLog/         # Keil USART1 日志工程
│   ├── 02I2CScan/         # Keil I2C 扫描工程
│   ├── 03SPIBasic/        # Keil SPI 回环工程
│   ├── 04ADCBasic/        # Keil ADC 采样工程
│   ├── 05CANBasic/        # Keil CAN 回环工程
│   ├── 06PWMBasic/        # Keil PWM 工程
│   ├── 07USBCDC/          # Keil USB CDC 回显工程
│   └── 08DACBasic/        # Keil DAC 输出工程
└── README.md
```

单个示例通常包含：

```text
example/
├── Core/                  # 应用入口、中断和外设初始化代码
├── Drivers/               # CMSIS 与 STM32 HAL 驱动
├── Middlewares/           # 可选中间件
├── USB_DEVICE/            # USB Device 配置与应用代码
├── cmake/                 # CLion/CMake 工程的生成配置
├── MDK-ARM/               # Keil 工程文件（仅 Keil 版本）
├── *.ioc                  # STM32CubeMX 工程配置
├── CMakeLists.txt         # 仅 CLion/CMake 版本
└── CMakePresets.json      # 仅 CLion/CMake 版本
```

并非每个示例都会包含全部目录。

## 参考资料

- [开发板完整文档、原理图与引脚图](https://docs.voidworks.link/docs/VoidCore/ST/F0/voidcore-f0-stm32f072cbu6)
- [STM32F072CB 产品页面](https://www.st.com/en/microcontrollers-microprocessors/stm32f072cb.html)
- [STM32F072x8/xB 数据手册](https://www.st.com/resource/en/datasheet/stm32f072c8.pdf)
- [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html)

## 贡献

欢迎补充新的外设示例或其他 IDE 工程。建议沿用 `两位编号 + 功能名称` 的目录命名方式，并在提交前确认工程可以完整编译、引脚说明与 `.ioc` 配置一致，同时更新本 README 的示例列表。

如果您已经购买 **Kaelexvp VoidCore STM32F072CBU6 开发板**，还可以联系客服，根据实际开发和学习需求定制示例项目。具体支持范围及交付方式请以客服沟通结果为准。
