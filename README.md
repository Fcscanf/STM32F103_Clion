## CLion开发STM32工程
工程介绍：
- 使用STM32CubeMX生成模板工程
- 开发板使用正点原子的STM32F03ZET6精英开发板

### OLED
使用 8080 并口模式驱动或者使用 4 线 SPI 串口模式，驱动 OLED 模块，不停的显示 ASCII 码和码值。LED0 闪烁，提示程序运行。
#### 硬件资源
1. LED 灯
    - PB5引脚接LED0（对应板上灯DS0）
    - PE5引脚接LED1（对应板上灯DS1）
2. ALIENTEK 0.96 寸 OLED 模块，在硬件上，OLED 与开发板的 IO 口对应关系如下：
   OLED_CS 对应 OV_WRST，即：PD6；
   OLED_RS 对应 OV_SCL，即：PD3，高电平为命令，低电平为数据；
   OLED_WR 对应 OV_RRST，即：PG14；
   OLED_RD 对应 DCMI_SDA，即：PG13；
   OLED_RST 对应 DCMI_RESET，即：PG15；
   OLED_D[7:0]对应 DCMI_D[7:0]，即：PC[7:0]