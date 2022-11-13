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

/*
 * ADC操作
 * 
 */
#include "common_api.h"
#include "FreeRTOS.h"
#include "task.h"

#include "luat_base.h"
#ifdef __LUATOS__
#include "luat_malloc.h"
#include "luat_msgbus.h"
#include "luat_timer.h"
#endif
#include "luat_adc.h"

#include "adc.h"
#include "hal_adc.h"
#include "ic.h"
#include "hal_trim.h"

#define ADC_ID_TMP  10
#define ADC_ID_VBAT 11

/*
注意, 按硬件应用手册的描述, ADC0 对应的是 AIO3, ADC1 对应的 AIO4

而 VBAT和TEMP 则使用LuatOS定义的10/11, 与硬件无关.


*/

static uint8_t adc_state[4] = {0}; // 注意实际映射, 当前不支持 AIO1/AIO2的映射
static volatile uint32_t aio3ChannelResult = 0;
static volatile uint32_t aio4ChannelResult = 0;
static volatile uint32_t vbatChannelResult = 0;
static volatile uint32_t thermalChannelResult = 0;

static int adc_exist(int id) {
    if (id >= 0 && id < 5)
        return 1;
    if (id == 10 || id == 11)
        return 1;
    return 0;
}

static void adc0_cb(uint32_t result) {
    aio3ChannelResult = result;
    adc_state[0] = 1;
}

static void adc1_cb(uint32_t result) {
    aio4ChannelResult = result;
    adc_state[1] = 1;
}

static void adc_vbat_cb(uint32_t result) {
    vbatChannelResult = result;
    adc_state[2] = 1;
}
static void adc_temp_cb(uint32_t result) {
    thermalChannelResult = result;
    adc_state[3] = 1;
}

static volatile uint8_t adc_init = 1;

int luat_adc_open(int id, void* ptr) {
    AdcConfig_t adcConfig;
    if (!adc_exist(id))
        return -1;
    ADC_getDefaultConfig(&adcConfig);
    if (adc_init){
        trimAdcSetGolbalVar();
        adc_init = 0;
    }
    
    switch (id)
    {
    case 0:
        adcConfig.channelConfig.aioResDiv = ADC_AIO_RESDIV_RATIO_3OVER16;
        ADC_channelInit(ADC_CHANNEL_AIO3, ADC_USER_APP, &adcConfig, adc0_cb);
        adc_state[0] = 0;
        break;
    case 1:
        adcConfig.channelConfig.aioResDiv = ADC_AIO_RESDIV_RATIO_3OVER16;
        ADC_channelInit(ADC_CHANNEL_AIO4, ADC_USER_APP, &adcConfig, adc1_cb);
        adc_state[1] = 0;
        break;
    case ADC_ID_VBAT: // vbat
        adcConfig.channelConfig.vbatResDiv = ADC_VBAT_RESDIV_RATIO_3OVER16;
        ADC_channelInit(ADC_CHANNEL_VBAT, ADC_USER_APP, &adcConfig, adc_vbat_cb);
        adc_state[2] = 0;
        break;
    case ADC_ID_TMP: // temp
        adcConfig.channelConfig.vbatResDiv = ADC_VBAT_RESDIV_RATIO_3OVER16;
        ADC_channelInit(ADC_CHANNEL_THERMAL, ADC_USER_APP, NULL, adc_temp_cb);
        adc_state[3] = 0;
        break;
    default:
        return -1;
    }
    
    return 0;
}

int luat_adc_read(int id, int* val, int* val2) {
    if (!adc_exist(id))
        return -1;
    int t = 10000;
    switch (id)
    {
    case 0:
        ADC_startConversion(ADC_CHANNEL_AIO3, ADC_USER_APP);
        while(adc_state[0] == 0 && t > 0) {
            t --;
        }; // 1w个循环,通常需要4000个循环
        break;
    case 1:
        ADC_startConversion(ADC_CHANNEL_AIO4, ADC_USER_APP);
        while(adc_state[1] == 0 && t > 0) {
            t --;
        }; // 1w个循环,通常需要4000个循环
        break;
    case ADC_ID_VBAT:
        ADC_startConversion(ADC_CHANNEL_VBAT, ADC_USER_APP);
        while(adc_state[2] == 0 && t > 0) {
            t --;
        }; // 1w个循环,通常需要4000个循环
        break;
    case ADC_ID_TMP:
        ADC_startConversion(ADC_CHANNEL_THERMAL, ADC_USER_APP);
        while(adc_state[3] == 0 && t > 0) {
            t --;
        }; // 1w个循环,通常需要4000个循环
        break;
    default:
        return -1;
    }

    
    switch (id)
    {
    case 0:
        *val = aio3ChannelResult;
#ifdef __LUATOS__
        *val2 = (int)HAL_ADC_CalibrateRawCode(aio3ChannelResult) * 16 / 3 /1000;
#else
        *val2 = (int)HAL_ADC_CalibrateRawCode(aio3ChannelResult) * 16 / 3 ;
#endif
        break;
    case 1:
        *val = aio4ChannelResult;
#ifdef __LUATOS__
        *val2 = (int)HAL_ADC_CalibrateRawCode(aio4ChannelResult) * 16 / 3 /1000;
#else
        *val2 = (int)HAL_ADC_CalibrateRawCode(aio4ChannelResult) * 16 / 3 ;
#endif
        break;
    case ADC_ID_VBAT:
        *val = vbatChannelResult;
        *val2 =  (int)HAL_ADC_CalibrateRawCode(vbatChannelResult) * 16 / 3 / 1000;
        break;
    case ADC_ID_TMP:
        *val = thermalChannelResult;
        *val2 = (int)HAL_ADC_ConvertThermalRawCodeToTemperature(thermalChannelResult);
        break;
    default:
        return -1;
    }

    return 0;
}

int luat_adc_close(int id) {
    switch (id)
    {
    case 0:
        ADC_channelDeInit(ADC_CHANNEL_AIO3, ADC_USER_APP);
        break;
    case 1:
        ADC_channelDeInit(ADC_CHANNEL_AIO4, ADC_USER_APP);
        break;
    case ADC_ID_VBAT:
        ADC_channelDeInit(ADC_CHANNEL_VBAT, ADC_USER_APP);
        break;
    case ADC_ID_TMP:
        ADC_channelDeInit(ADC_CHANNEL_THERMAL, ADC_USER_APP);
        break;
    default:
        return -1;
    }
    return 0;
}


int luat_adc_global_config(int tp, int val) {
    return -1;
}


