/*
 * Copyright (c) 2022 OpenLuat & AirM2M
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef LUAT_USB_H
#define LUAT_USB_H

#include "luat_base.h"

/**
 * @ingroup luatos_device 外设接口
 * @{
 */
/**
 * @defgroup luatos_device_usb USB接口
 * @{
 */

/**
 * @brief 校验位
 */
#define LUAT_PARITY_NONE                     0  /**< 无校验 */
#define LUAT_PARITY_ODD                      1  /**< 偶校验 */
#define LUAT_PARITY_EVEN                     2  /**< 奇校验 */

/**
 * @brief 高低位顺序
 */
#define LUAT_BIT_ORDER_LSB                   0  /**< 低位有效 */
#define LUAT_BIT_ORDER_MSB                   1  /**< 高位有效 */

/**
 * @brief 停止位
 */
#define LUAT_0_5_STOP_BITS                   0xf0   /**< 0.5 */
#define LUAT_1_5_STOP_BITS                   0xf1   /**< 1.5 */

//#define LUAT_VUART_ID_0						0x20    

/**
 * @brief luat_usb
 */
typedef struct luat_usb {
    int id;                     /**< 串口id */
    int baud_rate;              /**< 波特率 */

    uint8_t data_bits;          /**< 数据位 */
    uint8_t stop_bits;          /**< 停止位 */ 
    uint8_t bit_order;          /**< 高低位 */ 
    uint8_t parity;             /**< 奇偶校验位 */ 

    size_t bufsz;               /**< 接收数据缓冲区大小 */ 
    uint32_t pin485;            /**< 转换485的pin, 如果没有则是0xffffffff*/ 
    uint32_t delay;             /**< 485翻转延迟时间，单位us */ 
    uint8_t rx_level;           /**< 接收方向的电平 */ 
} luat_usb_t;

/**
 * @brief usb初始化
 * 
 * @param usb luat_usb结构体
 * @return int 
 */
int luat_usb_setup(luat_usb_t* usb);

/**
 * @brief 串口写数据
 * 
 * @param usb_id 串口id
 * @param data 数据
 * @param length 数据长度
 * @return int 
 */
int luat_usb_write(int usb_id, void* data, size_t length);

/**
 * @brief 串口读数据
 * 
 * @param usb_id 串口id
 * @param buffer 数据
 * @param length 数据长度
 * @return int 
 */
int luat_usb_read(int usb_id, void* buffer, size_t length);

/**
 * @brief 关闭串口
 * 
 * @param usb_id 串口id
 * @return int 
 */
int luat_usb_close(int usb_id);


/**
 * @brief 串口控制参数
 */
typedef enum LUAT_USB_CTRL_CMD
{
    LUAT_USB_SET_RECV_CALLBACK,/**< 接收回调 */
    LUAT_USB_SET_SENT_CALLBACK/**< 发送回调 */
}LUAT_USB_CTRL_CMD_E;

/**
 * @brief 接收回调函数
 * 
 */
typedef void (*luat_usb_recv_callback_t)(int usb_id, uint32_t data_len);

/**
 * @brief 发送回调函数
 * 
 */
typedef void (*luat_usb_sent_callback_t)(int usb_id, void *param);

/**
 * @brief 串口控制参数
 * 
 */
typedef union luat_usb_ctrl_param
{
    luat_usb_recv_callback_t recv_callback_fun;/**< 接收回调函数 */
    luat_usb_sent_callback_t sent_callback_fun;/**< 发送回调函数 */
}luat_usb_ctrl_param_t;

/**
 * @brief 串口控制
 * 
 * @param usb_id 串口id
 * @param cmd 串口控制命令
 * @param param 串口控制参数
 * @return int 
 */
int luat_usb_ctrl(int usb_id, LUAT_USB_CTRL_CMD_E cmd, luat_usb_ctrl_param_t param);

/** @}*/
/** @}*/
#endif
