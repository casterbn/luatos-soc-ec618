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
#include "wdt.h"
#include "luat_rtos.h"
#include "luat_mobile.h"
#include "luat_network_adapter.h"
#include "ps_event_callback.h"
#include "networkmgr.h"
#ifdef LUAT_USE_LVGL
#include "lvgl.h"
#include "luat_lvgl.h"
#endif

extern int luat_main(void);
extern void luat_heap_init(void);
const char *soc_get_sdk_type(void)
{
	return "LuatOS-SoC";
}
const char *soc_get_sdk_version(void)
{
	return LUAT_BSP_VERSION;
}

#ifdef LUAT_USE_LVGL
static void luat_lvgl_callback(void *param){
    lv_tick_inc(10);
    rtos_msg_t msg = {0};
    msg.handler = lv_task_handler;
    luat_msgbus_put(&msg, 0);
}
#endif

luat_rtos_timer_t lvgl_timer_handle;

static INT32 ps_callback(PsEventID eventID, void *param, UINT32 paramLen)
{
	NmAtiNetInfoInd *net_info = (NmAtiNetInfoInd *)param;
	ip_addr_t dns_ip;
	if(PS_URC_ID_PS_NETINFO == eventID)
	{
		if (NM_NETIF_ACTIVATED == net_info->netifInfo.netStatus)
		{
			if (net_info->netifInfo.ipv4Cid != 0xFF)
			{
				dns_ip.type = IPADDR_TYPE_V4;
				dns_ip.u_addr.ip4 = net_info->netifInfo.ipv4Info.dns[0];
				network_set_dns_server(NW_ADAPTER_INDEX_LWIP_GPRS, 2, &dns_ip);
				dns_ip.u_addr.ip4 = net_info->netifInfo.ipv4Info.dns[1];
				network_set_dns_server(NW_ADAPTER_INDEX_LWIP_GPRS, 3, &dns_ip);

			}
			else if (net_info->netifInfo.ipv6Cid != 0xFF)
			{

			}
			net_lwip_set_link_state(NW_ADAPTER_INDEX_LWIP_GPRS, 1);
		}
	}
	return 0;
}

static void luatos_task(void *param)
{
	net_lwip_init();
	net_lwip_register_adapter(NW_ADAPTER_INDEX_LWIP_GPRS);
	network_register_set_default(NW_ADAPTER_INDEX_LWIP_GPRS);
	registerPSEventCallback(PS_GROUP_PS_MASK, ps_callback);
	luat_heap_init();
	//DBG("LuatOS starting ...");

#ifdef LUAT_USE_LVGL
    lv_init();

	luat_rtos_timer_create(&lvgl_timer_handle);

	luat_rtos_timer_start(lvgl_timer_handle, 10 / portTICK_PERIOD_MS, true, luat_lvgl_callback, NULL);

#endif

	luat_main();
	while (1) {
		DBG("LuatOS exit");
		luat_rtos_task_sleep(1000);
	}
}

static void mobile_event_cb(LUAT_MOBILE_EVENT_E event, uint8_t index, uint8_t status)
{
	luat_mobile_cell_info_t cell_info;
	luat_mobile_signal_strength_info_t signal_info;
	uint8_t csq, i;
	char imsi[20];
	char iccid[24] = {0};

	switch(event)
	{
	case LUAT_MOBILE_EVENT_CFUN:
		break;
	case LUAT_MOBILE_EVENT_SIM:
		break;
	case LUAT_MOBILE_EVENT_REGISTER_STATUS:
		break;
	case LUAT_MOBILE_EVENT_CELL_INFO:
		break;
	case LUAT_MOBILE_EVENT_PDP:
		break;
	case LUAT_MOBILE_EVENT_NETIF:
		switch (status)
		{
		case LUAT_MOBILE_NETIF_LINK_ON:
			break;
		default:
			break;
		}
		break;
	case LUAT_MOBILE_EVENT_TIME_SYNC:
		break;
	case LUAT_MOBILE_EVENT_SMS:
		break;
	case LUAT_MOBILE_EVENT_CSCON:
		DBG("CSCON %d", status);
		break;
	default:
		break;
	}
}

static void luatos_task_init(void)
{
	WDT_deInit();
	luat_mobile_event_register_handler(mobile_event_cb);
	luat_mobile_set_period_work(0, 10000, 4);
//	luat_mobile_set_rrc_auto_release_time(1);
	luat_rtos_task_handle task_handle;
	// xTaskCreateStatic(task1, "luatos", VM_STACK_SIZE, NULL, 20, s_vm_stackbuff, pxVMTaskTCBBuffer);
	luat_rtos_task_create(&task_handle, 16 * 1024, 80, "luatos", luatos_task, NULL, 0);
}

INIT_TASK_EXPORT(luatos_task_init, "1");


