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
#include "luat_flash.h"
#include "luat_crypto.h"

#define FLASH_FOTA_REGION_END           (0x384000)

static void flash_example(void *param)
{
	luat_rtos_task_sleep(1500);
	
	char buff[256] = {0};
	
	// 注意, 这个addr是演示性质的
	uint32_t addr = FLASH_FOTA_REGION_END - 4096;

	// flash读写没有太多技巧
	luat_flash_read(buff, addr, 256);

	luat_flash_erase(addr, 4096);

	// 生成一些随机数据,模拟业务数据
	luat_crypto_trng(buff, 256);

	luat_flash_write(buff, addr, 256);

	luat_flash_read(buff, addr, 256);

	while(1)
	{
		luat_rtos_task_sleep(1000);
		LUAT_DEBUG_PRINT("flash demo is done");
	}
}

static void task_demoE_init(void)
{
	luat_rtos_task_create(NULL, 4*1024, 50, "flash", flash_example, NULL, 0);
}


//启动hw_demoA_init，启动位置硬件初始1级
// INIT_HW_EXPORT(hw_demoA_init, "1");
//启动hw_demoB_init，启动位置硬件初始2级
// INIT_HW_EXPORT(hw_demoB_init, "2");
//启动dr_demoC_init，启动位置驱动1级
// INIT_DRV_EXPORT(dr_demoC_init, "1");
//启动dr_demoD_init，启动位置驱动2级
// INIT_DRV_EXPORT(dr_demoD_init, "2");
//启动task_demoE_init，启动位置任务1级
INIT_TASK_EXPORT(task_demoE_init, "1");
//启动task_demoF_init，启动位置任务2级
// INIT_TASK_EXPORT(task_demoF_init, "2");

