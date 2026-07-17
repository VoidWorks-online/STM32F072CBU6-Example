# STM32F072 USB CDC 回显示例

本工程实现 USB 虚拟串口回显：上位机发送的数据会由单片机按原顺序返回。接收回调只负责复制数据，实际发送由主循环以非阻塞方式完成。

## 数据流程

```text
上位机发送 -> CDC_Receive_FS() -> 8 槽回显队列
                                      |
上位机接收 <- CDC_EchoProcess_FS() <--+
```

USB 全速 CDC 单包最大为 64 字节。队列占用 512 字节 RAM，并保留一个槽区分空与满，因此最多暂存 7 包。可调用 `CDC_EchoGetDroppedPackets_FS()` 查看队列满造成的丢包数。

不要在 `CDC_Receive_FS()` 中等待 `CDC_Transmit_FS()` 或调用 `HAL_Delay()`，否则会阻塞 USB 处理。也不要把接收缓冲区直接交给异步发送后立即重新接收，下一包可能覆盖尚未发完的数据。

## 测试方法

1. 在 STM32CubeIDE 中编译并烧录工程。
2. 将 PA11（USB DM）和 PA12（USB DP）正确连接至电脑。
3. Linux 通常枚举为 `/dev/ttyACM0`，Windows 则显示为 COM 端口。
4. 用串口工具发送文本或十六进制数据，应收到完全相同的数据。

Linux 可使用：

```bash
python3 -m serial.tools.miniterm /dev/ttyACM0 115200
```

本示例未桥接物理 UART，因此虚拟串口的波特率设置通常不影响 USB 实际传输。若设备无法枚举，请检查 48 MHz USB 时钟、PA11/PA12 接线、D+/D- 极性及 USB 中断配置。
