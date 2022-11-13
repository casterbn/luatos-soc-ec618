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

#include "luat_adc.h"

luat_rtos_task_handle adc_task_handle;

static void task_test_adc(void *param)
{
    int val, val2;
    luat_adc_open(0 , NULL);
    luat_adc_open(1 , NULL);
    luat_adc_open(10, NULL);
    luat_adc_open(11, NULL);
    while (1)
    {
        luat_rtos_task_sleep(1000);
        luat_adc_read(0 , &val, &val2);
        LUAT_DEBUG_PRINT("adc0: %d %d\n",val, val2);
        luat_adc_read(1 , &val, &val2);
        LUAT_DEBUG_PRINT("adc1: %d %d\n",val, val2);
        luat_adc_read(10, &val, &val2);
        LUAT_DEBUG_PRINT("temp: %d %d\n",val, val2);
        luat_adc_read(11, &val, &val2);
        LUAT_DEBUG_PRINT("vbat: %d %d\n",val, val2);
    }
    
}

static void task_demo_adc(void)
{
    luat_rtos_task_create(&adc_task_handle, 1024, 20, "adc", task_test_adc, NULL, NULL);
}

INIT_TASK_EXPORT(task_demo_adc,"1");



