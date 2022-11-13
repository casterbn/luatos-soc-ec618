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

#include "common_api.h"
#include "FreeRTOS.h"
#include "task.h"

#include "luat_base.h"
#include "luat_uart.h"
#include "luat_rtos.h"

#ifdef __LUATOS__
#include "luat_malloc.h"
#include "luat_msgbus.h"
#include "luat_timer.h"
#endif

#include <stdio.h>
#include "bsp_custom.h"

#if RTE_UART0
extern ARM_DRIVER_USART Driver_USART0;
static ARM_DRIVER_USART *USARTdrv0 = &Driver_USART0;
#endif
#if RTE_UART1
extern ARM_DRIVER_USART Driver_USART1;
static ARM_DRIVER_USART *USARTdrv1 = &Driver_USART1;
#endif
#if RTE_UART2
extern ARM_DRIVER_USART Driver_USART2;
static ARM_DRIVER_USART *USARTdrv2 = &Driver_USART2;
#endif

static ARM_DRIVER_USART * uart_drvs[3] = {0, 0, 0};
//static int _uart_state[3] = {0 ,0 ,0};
#define UART_BUFFSIZE 1024
static char uart0_buff[UART_BUFFSIZE];
char uart1_buff[UART_BUFFSIZE];
static char uart2_buff[UART_BUFFSIZE];
//static uint8_t serials_marks[3];
static luat_rtos_task_handle uart_task_handle;
static void task_uart(void *param);
static void uart_task_init(void)
{
    luat_rtos_task_create(&uart_task_handle, 2048, 20, "uart", task_uart, NULL, NULL);
}


static luat_uart_ctrl_param_t uart_cb[3]={0};

luat_rtos_queue_t uart_queue_handle;

typedef struct luat_uart_queue
{
    int id;
    uint32_t event;
}luat_uart_queue_t;


static void uart_init() {
    #if RTE_UART0
    uart_drvs[0] = &Driver_USART0;
    #endif
    #if RTE_UART1
    uart_drvs[1] = &Driver_USART1;
    #endif
    #if RTE_UART2
    uart_drvs[2] = &Driver_USART2;
    #endif
}

#ifdef __LUATOS__
void luat_uart_recv_cb(int uart_id, uint32_t data_len){
        rtos_msg_t msg;
        msg.handler = l_uart_handler;
        msg.ptr = NULL;
        msg.arg1 = uart_id;
        msg.arg2 = data_len;
        int re = luat_msgbus_put(&msg, 0);
}

void luat_uart_sent_cb(int uart_id, void *param){
        rtos_msg_t msg;
        msg.handler = l_uart_handler;
        msg.ptr = NULL;
        msg.arg1 = uart_id;
        msg.arg2 = 0;
        int re = luat_msgbus_put(&msg, 0);
}
#endif

static void uart2ReceiverCallback(uint32_t event) {
    luat_uart_queue_t uart_queue = {
        .id = 2,
        .event = event
    };
    luat_rtos_queue_send(uart_queue_handle, &uart_queue, sizeof(luat_uart_queue_t), 0);
}

static void uart1ReceiverCallback(uint32_t event) {
    luat_uart_queue_t uart_queue = {
        .id = 1,
        .event = event
    };
    luat_rtos_queue_send(uart_queue_handle, &uart_queue, sizeof(luat_uart_queue_t), 0);
}

static void uart0ReceiverCallback(uint32_t event) {
    luat_uart_queue_t uart_queue = {
        .id = 0,
        .event = event
    };
    luat_rtos_queue_send(uart_queue_handle, &uart_queue, sizeof(luat_uart_queue_t), 0);
}

int luat_uart_setup(luat_uart_t* uart) {
    if (!luat_uart_exist(uart->id)) {
        DBG("uart.setup FAIL!!!");
        return -1;
    }
    DBG("setup id=%d", uart->id);
    if (!uart_task_handle)
    {
    	uart_task_init();
    }
    //配置串口设置
    uint32_t uart_settings = ARM_USART_MODE_ASYNCHRONOUS | ARM_USART_FLOW_CONTROL_NONE;
    switch (uart->data_bits)
    {
    case 5:
        uart_settings |= ARM_USART_DATA_BITS_5;
        break;
    case 6:
        uart_settings |= ARM_USART_DATA_BITS_6;
        break;
    case 7:
        uart_settings |= ARM_USART_DATA_BITS_7;
        break;
    case 8:
        uart_settings |= ARM_USART_DATA_BITS_8;
        break;
    case 9:
        uart_settings |= ARM_USART_DATA_BITS_9;
        break;
    default:
        break;
    }
    switch (uart->stop_bits)
    {
    case 1:
        uart_settings |= ARM_USART_STOP_BITS_1;
        break;
    case 2:
        uart_settings |= ARM_USART_STOP_BITS_2;
        break;
    default:
        break;
    }
    switch (uart->parity)
    {
    case LUAT_PARITY_NONE:
        uart_settings |= ARM_USART_PARITY_NONE;
        break;
    case LUAT_PARITY_EVEN:
        uart_settings |= ARM_USART_PARITY_EVEN;
        break;
    case LUAT_PARITY_ODD:
        uart_settings |= ARM_USART_PARITY_ODD;
        break;
    default:
        break;
    }

#if RTE_UART2
    if (uart->id == 2) {
        // 强制设置一次uart2的管脚
        // pad_config_t padConfig;
        // PAD_GetDefaultConfig(&padConfig);

        // padConfig.mux = PAD_MuxAlt2;
        // PAD_SetPinConfig(13, &padConfig); // GPIO2/UART2_RX
        // PAD_SetPinConfig(14, &padConfig); // GPIO3/UART2_TX

        USARTdrv2->Initialize(uart2ReceiverCallback);
        USARTdrv2->PowerControl(ARM_POWER_FULL);
        USARTdrv2->Control(uart_settings, (unsigned long)uart->baud_rate);
        USARTdrv2->Receive(uart2_buff, UART_BUFFSIZE);
    }
#endif
#if RTE_UART1
    if (uart->id == 1) {
        USARTdrv1->Initialize(uart1ReceiverCallback);
        USARTdrv1->PowerControl(ARM_POWER_FULL);
        USARTdrv1->Control(uart_settings, (unsigned long)uart->baud_rate);
        USARTdrv1->Receive(uart1_buff, UART_BUFFSIZE);
    }
#endif
#if RTE_UART0
    else if (uart->id == 0) {
        USARTdrv0->Initialize(uart0ReceiverCallback);
        USARTdrv0->PowerControl(ARM_POWER_FULL);
        USARTdrv0->Control(uart_settings, (unsigned long)uart->baud_rate);
        USARTdrv0->Receive(uart0_buff, UART_BUFFSIZE);
    }
#endif
    return 0;
}

int luat_uart_write(int uartid, void* data, size_t length) {
    if (luat_uart_exist(uartid)) {
        uart_drvs[uartid]->Send(data, length);
    }
    else {
        DBG("not such uart id=%ld", uartid);
    }
    return 0;
}

int luat_uart_read(int uartid, void* buffer, size_t len) {
    int rcount = 0;
    if (luat_uart_exist(uartid)) {
        #if RTE_UART2
        if (uartid == 2) {
            rcount = USARTdrv2->GetRxCount();
            if (buffer == NULL) return rcount;
            if (rcount > 0) {
                if (rcount > len)
                    rcount = len;
                memcpy(buffer, uart2_buff, rcount);
            }
            // 复位读取标志
            USARTdrv2->Receive(uart2_buff, UART_BUFFSIZE);
        }
        #endif
        #if RTE_UART1
        if (uartid == 1) {
            rcount = USARTdrv1->GetRxCount();
            if (buffer == NULL) return rcount;
            if (rcount > 1) {
                if (rcount > len)
                    rcount = len;
                memcpy(buffer, uart1_buff, rcount);
            }
            // 复位读取标志
            USARTdrv1->Receive(uart1_buff, UART_BUFFSIZE);
        }
        #endif
        #if RTE_UART0
        if (uartid == 0) {
            rcount = USARTdrv0->GetRxCount();
            if (buffer == NULL) return rcount;
            if (rcount > 0) {
                if (rcount > len)
                    rcount = len;
                memcpy(buffer, uart0_buff, rcount);
            }
            // 复位读取标志
            USARTdrv0->Receive(uart0_buff, UART_BUFFSIZE);
        }
        #endif
    }
    return rcount;
}
int luat_uart_close(int uartid) {
    if (luat_uart_exist(uartid)) {
        uart_drvs[uartid]->PowerControl(ARM_POWER_OFF);
        uart_drvs[uartid]->Uninitialize();
    }
    return 0;
}
int luat_uart_exist(int uartid) {
    // EC618仅支持 uart0,uart1,uart2
    // uart0是ulog的输出
    // uart1是支持下载,也是用户可用
    // uart2是用户可用的
    // if (uartid == 0 || uartid == 1 || uartid == 2) {
    // 暂时只支持UART1和UART2
    if (uartid == 1 || uartid == 2) {
        if (!uart_drvs[uartid])
            uart_init();
        return 1;
    }
    else {
        DBG("uart%ld not exist", uartid);
        return 0;
    }
}

int luat_setup_cb(int uartid, int received, int sent) {
    if (luat_uart_exist(uartid)) {
#ifdef __LUATOS__
        if (received){
            uart_cb[uartid].recv_callback_fun = luat_uart_recv_cb;
        }

        if (sent){
            uart_cb[uartid].sent_callback_fun = luat_uart_sent_cb;
        }
#endif
    }
    return 0;
}

int luat_uart_ctrl(int uart_id, LUAT_UART_CTRL_CMD_E cmd, void* param){
    if (luat_uart_exist(uart_id)) {
        if (cmd == LUAT_UART_SET_RECV_CALLBACK){
            uart_cb[uart_id].recv_callback_fun = param;
        }else if(cmd == LUAT_UART_SET_SENT_CALLBACK){
            uart_cb[uart_id].sent_callback_fun = param;
        }
    }
    return 0;
}


static void task_uart(void *param)
{
    int rcount;
    luat_uart_queue_t uart_queue;
    luat_rtos_queue_create(&uart_queue_handle, 100, sizeof(luat_uart_queue_t));
    while (1)
    {
        luat_rtos_queue_recv(uart_queue_handle, &uart_queue, sizeof(luat_uart_queue_t), LUAT_WAIT_FOREVER);
        // DBG("id:%d event:%d",uart_queue.id,uart_queue.event);
        if (uart_queue.event & ARM_USART_EVENT_RX_TIMEOUT || uart_queue.event & ARM_USART_EVENT_RECEIVE_COMPLETE)
        {
            if (uart_cb[uart_queue.id].recv_callback_fun)
            {
                if (uart_queue.id == 0){
#if RTE_UART0
                    rcount = USARTdrv0->GetRxCount();
#endif
                }
#if RTE_UART1
                else if(uart_queue.id == 1){
                    rcount = USARTdrv1->GetRxCount();
                }
#endif
#if RTE_UART2
                else if(uart_queue.id == 2){
                    rcount = USARTdrv2->GetRxCount();
                }
#endif
                uart_cb[uart_queue.id].recv_callback_fun(uart_queue.id,rcount);
            }
        }else if(uart_queue.event & ARM_USART_EVENT_SEND_COMPLETE){
            if (uart_cb[uart_queue.id].sent_callback_fun){
                uart_cb[uart_queue.id].sent_callback_fun(uart_queue.id,rcount);
            }
        }
    }
}


//INIT_TASK_EXPORT(uart_task_init,"1");
