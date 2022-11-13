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

#ifndef Luat_ADC_H
#define Luat_ADC_H

#include "luat_base.h"
#include "luat_adc_legacy.h"

/**
 * @ingroup luatos_device 外设接口
 * @{
 */
/**
 * @defgroup luatos_device_adc ADC接口
 * @{
*/
/**
 * luat_adc_open
 * @brief 打开一个adc通道
 * 
 * @param pin[in] adc通道的序号
 * @param args[in] 保留用,传NULL
 * @return 0 成功, 其他值为失败
 */
int luat_adc_open(int pin, void *args);

/**
 * luat_adc_read
 * @brief 读取adc通道的值
 * 
 * @param pin[in] adc通道的序号
 * @param val[out] adc通道的原始值
 * @param val2[out] adc通道的计算值,与具体通道有关
 * @return 0 成功, 其他值为失败
 */
int luat_adc_read(int pin, int *val, int *val2);

/**
 * luat_adc_close
 * @brief 关闭adc通道
 * 
 * @param pin[in] adc通道的序号
 * @return 0 成功, 其他值为失败
 */
int luat_adc_close(int pin);

/**
 * @brief ADC控制命令
*/
typedef enum LUAT_ADC_CTRL_CMD
{
	LUAT_ADC_SET_GLOBAL_RANGE,/**< 内部分压 */
}LUAT_ADC_CTRL_CMD_E;

/**
 * @brief ADC控制参数
*/
typedef union luat_adc_ctrl_param
{
	void *userdata;/**< 预留 */
} luat_adc_ctrl_param_t;

/**
 * luat_adc_ctrl
 * @brief adc控制
 * 
 * @param pin[in] adc通道的序号
 * @param cmd adc控制命令
 * @param param adc控制参数
 * @return 0 成功, 其他值为失败
 */
int luat_adc_ctrl(int pin, LUAT_ADC_CTRL_CMD_E cmd, luat_adc_ctrl_param_t param);
/** @}*/
/** @}*/
#endif
