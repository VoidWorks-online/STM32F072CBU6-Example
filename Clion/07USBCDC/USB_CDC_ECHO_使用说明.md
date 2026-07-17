# STM32F072 USB CDC 回显示例

本示例实现“上位机通过 USB 虚拟串口发送什么，单片机就原样返回什么”。工程采用非阻塞方式：USB 接收回调只复制数据，真正的发送在主循环完成。

## 一、CubeMX 配置步骤

1. 新建 STM32F072CBU6 工程。
2. 在 `Connectivity -> USB` 中启用 `Device (Full Speed)`。STM32F072 的 USB 引脚为 PA11（DM）和 PA12（DP）。
3. 在 `Middleware and Software Packs -> USB_DEVICE` 中选择 `Communication Device Class (Virtual Port Com)`。
4. 时钟给 USB 提供准确的 48 MHz。本工程使用 HSI48；在需要免晶振工作的硬件上，建议同时根据板级时钟方案配置 CRS/USB SOF 同步。
5. 在 `Project Manager -> Code Generator` 中勾选 **Keep User Code when re-generating**。
6. 生成工程。CubeMX 会创建 `usb_device.c`、`usbd_cdc_if.c/.h` 和 USB Device Library。

## 二、生成后业务代码写在哪里

CubeMX 生成的文件中，只有 `/* USER CODE BEGIN ... */` 与对应 `/* USER CODE END ... */` 之间适合长期保存自定义代码。重新生成前仍建议使用版本控制或备份。

本示例有三个关键位置：

1. `USB_DEVICE/App/usbd_cdc_if.c` 的 `CDC_Receive_FS()`：USB 收到一包数据后进入这里。代码把数据复制到回显队列，然后立即重新使能接收端点。
2. `USB_DEVICE/App/usbd_cdc_if.c` 的 `CDC_EchoProcess_FS()`：检查上一包是否发送完成，并启动下一包异步发送。
3. `Core/Src/main.c` 的 `while (1)`：持续调用 `CDC_EchoProcess_FS()`。

不要在 `CDC_Receive_FS()` 内使用 `while (CDC_Transmit_FS(...) == USBD_BUSY)` 或 `HAL_Delay()`。接收函数运行在 USB 处理上下文中，阻塞会影响 USB 通信，严重时会造成断连。也不要直接异步发送 `Buf` 后马上重新接收，因为下一包可能覆盖尚未发送完成的数据。

## 三、数据流

```text
PC 虚拟串口发送
       |
       v
CDC_Receive_FS() --复制--> 8 槽回显队列
       |                         |
       +--立即重新使能接收       v
                         CDC_EchoProcess_FS()
                                  |
                                  v
                          PC 虚拟串口接收
```

每个 USB 全速 CDC 数据包最多 64 字节。队列当前有 8 个槽，占用 512 字节 RAM；环形队列会保留一个槽用于区分“空”和“满”，因此最多暂存 7 包。可通过 `CDC_EchoGetDroppedPackets_FS()` 查看队列满导致的丢包数，正常情况下应为 0。

## 四、测试方法

1. 编译并烧录固件，将 USB DP/DM 正确连接到电脑。
2. Linux 通常会出现 `/dev/ttyACM0`；Windows 会在设备管理器中出现一个 COM 口。
3. 用串口工具打开端口，发送文本或十六进制数据，应收到完全相同的数据。
4. CDC 的波特率设置只是主机传给设备的“线路编码”参数，本示例没有桥接物理 UART，因此选择 115200、9600 等通常不影响实际 USB 传输。

Linux 也可简单测试：

```bash
python3 -m serial.tools.miniterm /dev/ttyACM0 115200
```

如果设备没有枚举，优先检查 USB 48 MHz 时钟、PA11/PA12 接线、D+/D- 是否接反、USB 中断是否启用，以及描述符 VID/PID。若能枚举但没有回显，检查主循环是否持续调用 `CDC_EchoProcess_FS()`。
