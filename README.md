## CLion开发STM32工程
工程介绍：
- 使用STM32CubeMX生成模板工程
- 开发板使用正点原子的STM32F03ZET6精英开发板

### RTC
实时时钟(Real Time Clock,RTC)本质是一个计数器,计数频率常为秒,专门用来记录时间  
驱动RTC,使用串口打印当前时间
#### 硬件资源
1. LED 灯
  - PB5引脚接LED0（对应板上灯DS0）
  - PE5引脚接LED1（对应板上灯DS1）
2. RTC(实时时钟)