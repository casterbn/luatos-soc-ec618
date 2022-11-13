/*luat_debug_print
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
#include "luat_mobile.h"
#include "luat_debug.h"
#include "MQTTClient.h"


#define MQTT_HOST    	"lbsmqtt.airm2m.com"   				// MQTT服务器的地址和端口号
#define MQTT_PORT		1884
#define CLIENT_ID    	"123456789"          
#define USERNAME    	"username"                 
#define PASSWORD    	"password"   

static char mqtt_sub_topic[] = "test_topic";
static char mqtt_pub_topic[] = "test_topic";
static char mqtt_send_payload[] = "hello mqtt_test!!!";

uint8_t g_s_is_link_up = 0;


void messageArrived(MessageData* data)
{
	LUAT_DEBUG_PRINT("mqtt Message arrived on topic %.*s: %.*s", data->topicName->lenstring.len, data->topicName->lenstring.data,
		data->message->payloadlen, data->message->payload);
}

static void mqtt_demo(void){
	int rc = 0;
    static MQTTClient mqttClient;
    static Network n = {0};
    MQTTMessage message;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 4;
    connectData.clientID.cstring = CLIENT_ID;
    connectData.username.cstring = USERNAME;
    connectData.password.cstring = PASSWORD;
    connectData.keepAliveInterval = 120;

    
    //mqtts的话自行配置Network,详情查看Network结构体
    // n.isMqtts = TRUE;
    // n.timeout_r = 20;
    // n.seclevel = 1;
    // n.ciphersuite[0] = 0xFFFF;
    // n.ignore = 1;

    while(1)
    {
        while(!g_s_is_link_up)
		{
			luat_rtos_task_sleep(1000);
		}

        if ((rc = mqtt_connect(&mqttClient, &n,MQTT_HOST, MQTT_PORT, &connectData)) != 0)
            LUAT_DEBUG_PRINT("mqtt Return code from MQTT mqtt_connect is %d\n", rc);
        
        if ((rc = MQTTSubscribe(&mqttClient, mqtt_sub_topic, 0, messageArrived)) != 0)
            LUAT_DEBUG_PRINT("mqtt Return code from MQTT subscribe is %d\n", rc);
        while(1)
        {
            int len = strlen(mqtt_send_payload);
            message.qos = 1;
            message.retained = 0;
            message.payload = mqtt_send_payload;
            message.payloadlen = len;
            LUAT_DEBUG_PRINT("mqtt_demo send data");
            MQTTPublish(&mqttClient, mqtt_pub_topic, &message);
    #if !defined(MQTT_TASK)
            if ((rc = MQTTYield(&mqttClient, 1000)) != 0)
                LUAT_DEBUG_PRINT("mqtt_demo Return code from yield is %d\n", rc);
    #endif
            luat_rtos_task_sleep(5000);
        }
    }

    
}


static void mqttclient_task_init(void)
{
	luat_rtos_task_handle mqttclient_task_handle;
	luat_rtos_task_create(&mqttclient_task_handle, 2048, 20, "mqttclient", mqtt_demo, NULL, NULL);
}

static void mobile_event_cb(LUAT_MOBILE_EVENT_E event, uint8_t index, uint8_t status)
{
	switch(event)
	{
	case LUAT_MOBILE_EVENT_CFUN:
		LUAT_DEBUG_PRINT("CFUN消息，status %d", status);
		break;
	case LUAT_MOBILE_EVENT_SIM:
		LUAT_DEBUG_PRINT("SIM卡消息");
		switch(status)
		{
		case LUAT_MOBILE_SIM_READY:
			LUAT_DEBUG_PRINT("SIM卡正常工作");
			break;
		case LUAT_MOBILE_NO_SIM:
			LUAT_DEBUG_PRINT("SIM卡不存在");
			break;
		case LUAT_MOBILE_SIM_NEED_PIN:
			LUAT_DEBUG_PRINT("SIM卡需要输入PIN码");
			break;
		}
		break;
	case LUAT_MOBILE_EVENT_REGISTER_STATUS:
		LUAT_DEBUG_PRINT("移动网络服务状态变更，当前为%d", status);
		break;
	case LUAT_MOBILE_EVENT_CELL_INFO:
		switch(status)
		{
		case LUAT_MOBILE_CELL_INFO_UPDATE:
			break;
		case LUAT_MOBILE_SIGNAL_UPDATE:
			break;
		}
		break;
	case LUAT_MOBILE_EVENT_PDP:
		LUAT_DEBUG_PRINT("CID %d PDP激活状态变更为 %d", index, status);
		break;
	case LUAT_MOBILE_EVENT_NETIF:
		LUAT_DEBUG_PRINT("internet工作状态变更为 %d", status);
		switch (status)
		{
		case LUAT_MOBILE_NETIF_LINK_ON:
			LUAT_DEBUG_PRINT("可以上网");
            g_s_is_link_up = 1;
			break;
		default:
            g_s_is_link_up = 0;
			LUAT_DEBUG_PRINT("不能上网");
			break;
		}
		break;
	case LUAT_MOBILE_EVENT_TIME_SYNC:
		LUAT_DEBUG_PRINT("通过移动网络同步了UTC时间");
		break;
	case LUAT_MOBILE_EVENT_SMS:
		LUAT_DEBUG_PRINT("短信相关消息");
		break;
	case LUAT_MOBILE_EVENT_CSCON:
		LUAT_DEBUG_PRINT("RRC状态 %d", status);
		break;
	default:
		break;
	}
}

static void task_init(void){
    luat_mobile_event_register_handler(mobile_event_cb);
}

INIT_HW_EXPORT(task_init, "0");
INIT_TASK_EXPORT(mqttclient_task_init, "1");

