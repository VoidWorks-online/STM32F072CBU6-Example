# VoidCore F0 STM32F072CBU6 示例库

本仓库收录 **VoidCore F0 – STM32F072CBU6** 开发板的 HAL 示例工程，用于学习和验证 GPIO、定时器 PWM 与 USB Device CDC。当前三个示例均同时提供 CLion/CMake 和 STM32CubeIDE 工程。

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
| 00 | [GPIO Blink](./Clion/00GPIOBlink) | PA0 | 每 50 ms 翻转一次 GPIO，完整周期约 100 ms（约 10 Hz） |
| 06 | [PWM Basic](./Clion/06PWMBasic) | PA8 / TIM1_CH1、PA0 / TIM2_CH1 | 两路约 1 kHz、50% 占空比 PWM 输出 |
| 07 | [USB CDC](./Clion/07USBCDC) | PA11 / USB_DM、PA12 / USB_DP | 枚举为 USB 虚拟串口，并以非阻塞队列原样回显收到的数据 |

USB CDC 示例的设计、CubeMX 配置和测试方法详见 [`Clion/07USBCDC/USB_CDC_ECHO_使用说明.md`](./Clion/07USBCDC/USB_CDC_ECHO_%E4%BD%BF%E7%94%A8%E8%AF%B4%E6%98%8E.md)。

### STM32CubeIDE

工程位于 [`CubeIDE`](./CubeIDE) 目录，可直接导入 STM32CubeIDE。

| 编号 | 示例 | 主要资源 | 功能说明 |
| --- | --- | --- | --- |
| 00 | [GPIO Blink](./CubeIDE/00GPIOBlink) | PA0 | 每 50 ms 翻转一次 GPIO，功能与 CLion 版本一致 |
| 06 | [PWM Basic](./CubeIDE/06PWMBasic) | PA8 / TIM1_CH1、PA0 / TIM2_CH1 | 两路约 1 kHz、50% 占空比 PWM 输出 |
| 07 | [USB CDC](./CubeIDE/07USBCDC) | PA11 / USB_DM、PA12 / USB_DP | USB CDC 虚拟串口非阻塞回显，功能与 CLion 版本一致 |

STM32CubeIDE 版本的 USB CDC 说明见 [`CubeIDE/07USBCDC/USB_CDC_ECHO_使用说明.md`](./CubeIDE/07USBCDC/USB_CDC_ECHO_%E4%BD%BF%E7%94%A8%E8%AF%B4%E6%98%8E.md)。

### Keil5 / MDK-ARM

Keil5 示例工程将在对应目录加入仓库后列于此处，当前版本暂无可用工程。

## 快速开始

### 准备工作

- VoidCore F0 STM32F072CBU6 开发板
- USB Type-C 数据线
- 支持 SWD 的调试器（如 ST-Link 或兼容调试器）
- GNU Arm Embedded Toolchain、CMake 和 Ninja（使用当前 `Clion` 工程时）
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
3. 将根目录指向所需工程，例如 `CubeIDE/00GPIOBlink`、`CubeIDE/06PWMBasic` 或 `CubeIDE/07USBCDC`，导入后编译、下载。

三个目录都包含 `.project`、`.cproject` 和 `.ioc` 文件。需要使用 STM32CubeMX 重新生成代码时，建议先提交或备份修改，并保留 `USER CODE` 区域中的自定义代码。

仓库当前没有 Keil MDK-ARM 工程。

不同 IDE 版本的示例应保持相同的外设配置和功能，但工程文件、启动文件及链接脚本不可直接混用。

## 示例验证

### GPIO Blink

在 PA0 与 GND 之间连接 LED 和限流电阻，烧录后可观察到快速闪烁。程序每 50 ms 翻转一次 PA0，因此高低电平完整周期约为 100 ms（约 10 Hz）。也可直接用逻辑分析仪或示波器观察。

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
│   ├── 06PWMBasic/        # TIM1/TIM2 PWM 输出
│   └── 07USBCDC/          # USB CDC 虚拟串口回显
├── CubeIDE/
│   ├── 00GPIOBlink/       # STM32CubeIDE GPIO 翻转工程
│   ├── 06PWMBasic/        # STM32CubeIDE PWM 工程
│   └── 07USBCDC/          # STM32CubeIDE USB CDC 回显工程
└── README.md
```

单个 CubeMX/CMake 工程通常包含：

```text
example/
├── Core/                  # 应用入口、中断和外设初始化代码
├── Drivers/               # CMSIS 与 STM32 HAL 驱动
├── Middlewares/           # 可选中间件
├── USB_DEVICE/            # USB Device 配置与应用代码
├── cmake/                 # CubeMX 生成的 CMake 配置
├── *.ioc                  # STM32CubeMX 工程配置
├── CMakeLists.txt
└── CMakePresets.json
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
