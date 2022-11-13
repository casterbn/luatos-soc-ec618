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
#include "luat_rtos.h"
#include "luat_debug.h"
#include "luat_gpio.h"
#include "platform_define.h"

/*
移芯618平台上GPIO使用有如下注意事项：

系统有几种工作模式：ACTIVE、IDLE、SLEEP1、SLEEP2、HIBERNATE，几种模式的区别，参考example_pm的说明；

1、IO分为normal IO和retention/AON IO两种
   normal IO：普通IO
              在ACTIVE和IDLE工作模式下，可以正常控制使用；
			  在SLEEP1、SLEEP2、HIBERNATE工作模式下，IO电源被强制关闭，无法使用；
   retention/AON IO：保持IO
              在ACTIVE和IDLE工作模式下，可以正常控制使用；
			  在SLEEP1、SLEEP2、HIBERNATE工作模式下，只能作为输出使用，而且只能维持一种输出电平状态，无法翻转控制；

2、带AON功能的引脚，做retention/AON IO功能或者其他外设功能使用时，做软件配置上拉无效，只能使用默认的上下拉；

3、系统进入SLEEP1、SLEEP2、HIBERNATE休眠模式后，除了软件运行代码调用唤醒接口可以主动唤醒外，可以唤醒的外部事件有以下几种：
   (1). 配置为wakeup软件功能的wakeup pad引脚；
   (2). low power uart，如果波特率为9600，唤醒时数据也不会丢失，其他波特率会丢失前面的部分数据；
   (3). low power usb
   (4). power key；
   (5). charge pad；
   (6). rtc；
   详情参考example_pm的说明。
*/

//本example基于EVB_Air780E_V1.5硬件

#define NET_LED_PIN HAL_GPIO_27
#define LCD_RST_PIN HAL_GPIO_1
#define LCD_RS_PIN HAL_GPIO_10
#define LCD_CS_PIN HAL_GPIO_8


//控制NET指示灯闪烁
static void task_gpio_output_run(void *param)
{	
	luat_gpio_cfg_t gpio_cfg;
	luat_gpio_set_default_cfg(&gpio_cfg);

	gpio_cfg.pin = NET_LED_PIN;
	luat_gpio_open(&gpio_cfg);
	while(1)
	{
		luat_gpio_set(NET_LED_PIN, 1);
		LUAT_DEBUG_PRINT("net led on");
		luat_rtos_task_sleep(1000);
		luat_gpio_set(NET_LED_PIN, 0);
		LUAT_DEBUG_PRINT("net led off");
		luat_rtos_task_sleep(1000);
	}	
}

void task_gpio_output_init(void)
{
	luat_rtos_task_handle task_gpio_output_handle;
	luat_rtos_task_create(&task_gpio_output_handle, 4 * 1204, 50, "gpio_output_test", task_gpio_output_run, NULL, 32);
}


//LCD_RST引脚和NET引脚短接
//NET引脚：通过task_gpio_output_run函数每隔一秒翻转一次输出高低电平
//LCD_RST引脚：每隔一秒读取一次输入电平
void task_gpio_input_run(void)
{
	luat_gpio_cfg_t gpio_cfg;
	luat_gpio_set_default_cfg(&gpio_cfg);

	gpio_cfg.pin = LCD_RST_PIN;
	gpio_cfg.mode = LUAT_GPIO_INPUT;
	luat_gpio_open(&gpio_cfg);

	int level = 0;

	while(1)
	{
		level = luat_gpio_get(LCD_RST_PIN);
		LUAT_DEBUG_PRINT("get lcd rst pin %d",level);
		luat_rtos_task_sleep(1000);
		level = luat_gpio_get(LCD_RST_PIN);
		LUAT_DEBUG_PRINT("get lcd rst pin %d",level);
		luat_rtos_task_sleep(1000);
	}
}

void task_gpio_input_init(void)
{
	luat_rtos_task_handle task_gpio_input_handle;
	luat_rtos_task_create(&task_gpio_input_handle, 4 * 1204, 50, "gpio_input_test", task_gpio_input_run, NULL, 32);
}


int gpio_irq(int pin, void* args)
{
	LUAT_DEBUG_PRINT("gpio_irq pin:%d, level:%d", pin, luat_gpio_get(pin));
}

//GPIO中断测试还有问题，待下个版本解决
void task_gpio_interrupt_run(void)
{
	luat_gpio_cfg_t gpio_cfg;

	//配置LCD_CS_PIN为中断引脚
	luat_gpio_set_default_cfg(&gpio_cfg);
	gpio_cfg.pin = LCD_CS_PIN;
	gpio_cfg.mode = LUAT_GPIO_IRQ;
	gpio_cfg.irq_type = LUAT_GPIO_BOTH_IRQ;
	gpio_cfg.irq_cb = gpio_irq;	
	luat_gpio_open(&gpio_cfg);

	//配置LCD_RS_PIN为输出引脚
	luat_gpio_set_default_cfg(&gpio_cfg);
	gpio_cfg.pin = LCD_RS_PIN;
	luat_gpio_open(&gpio_cfg);
	while(1)
	{
		luat_gpio_set(LCD_RS_PIN, 1);
		LUAT_DEBUG_PRINT("LCD_RS output 1, LCD_CS input %d",luat_gpio_get(LCD_CS_PIN));
		luat_rtos_task_sleep(1000);
		luat_gpio_set(LCD_RS_PIN, 0);
		LUAT_DEBUG_PRINT("LCD_RS output 0, LCD_CS input %d",luat_gpio_get(LCD_CS_PIN));
		luat_rtos_task_sleep(1000);
	}	

}

void task_gpio_interrupt_init(void)
{
	luat_rtos_task_handle task_gpio_interrupt_handle;
	luat_rtos_task_create(&task_gpio_interrupt_handle, 4 * 1204, 50, "gpio_interrupt_test", task_gpio_interrupt_run, NULL, 32);
}

INIT_TASK_EXPORT(task_gpio_output_init, "0");
INIT_TASK_EXPORT(task_gpio_input_init, "1");
INIT_TASK_EXPORT(task_gpio_interrupt_init, "2");
