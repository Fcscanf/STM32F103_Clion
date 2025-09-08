## CLion开发STM32工程
工程介绍：
- 使用STM32CubeMX生成模板工程
- 开发板使用正点原子的STM32F03ZET6精英开发板

### TIMER
功能描述：使用定时器6,实现500ms定时器更新中断,在中断里翻转LED0
- PB5引脚接LED0（对应板上灯DS0）
- PE5引脚接LED1（对应板上灯DS1）