## CLion开发STM32工程
工程介绍：
- 使用STM32CubeMX生成模板工程
- 开发板使用正点原子的STM32F03ZET6精英开发板

### LED
配置LED和UASRT
#### 硬件资源
1. LED 灯
  - PB5引脚接LED0（对应板上灯DS0）
  - PE5引脚接LED1（对应板上灯DS1）
2. 串口 1(PA9/PA10 连接在板载 USB 转串口芯片 CH340 上面)(USMART 使用)
