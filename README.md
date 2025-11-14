## CLion开发STM32工程
工程介绍：
- 使用STM32CubeMX生成模板工程
- 开发板使用正点原子的STM32F03ZET6精英开发板

### LED
板级驱动KEY按键扫描功能
#### 硬件资源
1. LED 灯
  - PB5引脚接LED0（对应板上灯DS0）
  - PE5引脚接LED1（对应板上灯DS1）
2. 独立按键
  - KEY0 – PE4
  - KEY1 – PE3
