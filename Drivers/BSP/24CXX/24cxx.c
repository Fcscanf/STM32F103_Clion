//
// Created by fcant on 2025/11/13 星期四.
//

#include "24cxx.h"

#include <stdio.h>
#include "../KEY/key.h"
#include "../IIC/iic.h"

void at24c02_init(void) {
    iic_init();
}

void at24c02_write_one_byte(uint8_t addr, uint8_t data) {
    /* 1、发送起始信号 */
    iic_start();

    /* 2、发送通讯地址(写操作地址) */
    iic_send_byte(0xA0);

    /* 3、等待应答信号 */
    iic_wait_ack();

    /* 4、发送内存地址 */
    iic_send_byte(addr);

    /* 5、等待应答信号 */
    iic_wait_ack();

    /* 6、发送写入数据 */
    iic_send_byte(data);

    /* 7、等待应答信号 */
    iic_wait_ack();

    /* 8、发送停止信号 */
    iic_stop();

    /* 等待EEPROM写入完成 */
    HAL_Delay(1);
}

uint8_t at24c02_read_one_byte(uint8_t addr) {
    uint8_t rec = 0;

    /* 1、发送起始信号 */
    iic_start();

    /* 2、发送通讯地址(写操作地址) */
    iic_send_byte(0xA0);

    /* 3、等待应答信号 */
    iic_wait_ack();

    /* 4、发送内存地址 */
    iic_send_byte(addr);

    /* 5、等待应答信号 */
    iic_wait_ack();

    /* 6、发送起始信号 */
    iic_start();

    /* 7、发送通讯地址(读操作地址) */
    iic_send_byte(0xA1);

    /* 8、等待应答信号 */
    iic_wait_ack();

    /* 9、等待接收数据 */
    rec = iic_read_byte(0);
    /* 10、发送非应答（获取该地址即可） */

    /* 11、发送停止信号 */
    iic_stop();

    return rec;
}

void at24c02_op_by_key(void) {
    uint8_t key = KEY_SCAN(0);
    if (key == KEY1_PRES) {
        at24c02_write_one_byte(100, 66);
        printf("write data \r\n");
    }

    if (key == KEY0_PRES) {
        uint8_t data = at24c02_read_one_byte(100);
        printf("read data:%d \r\n", data);
    }
}
