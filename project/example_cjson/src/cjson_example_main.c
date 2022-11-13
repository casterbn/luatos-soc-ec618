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
#include "cJSON.h"


#define FILE_LENGTH 40960

void test_json_1(char* json_data)
{

	LUAT_DEBUG_PRINT("######## Test 12 ########\n");
	cJSON_Minify(json_data);
	LUAT_DEBUG_PRINT("json data = %s\n", json_data);

	// test 01
	cJSON* root = cJSON_Parse(json_data);
	cJSON* format = cJSON_GetObjectItemCaseSensitive(root, "format");
	cJSON* framerate_item = cJSON_GetObjectItemCaseSensitive(format, "frame rate");
	double framerate = 0;
	if(cJSON_IsNumber(framerate_item)) {
		framerate = framerate_item->valuedouble;
	}

	LUAT_DEBUG_PRINT("######## Test 01 ########\n");
	LUAT_DEBUG_PRINT("frame rate = %lf\n", framerate);

	// test 02
	cJSON_SetNumberValue(framerate_item, 25);
	char* rendered = cJSON_Print(root);
	
	LUAT_DEBUG_PRINT("######## Test 02 ########\n");
	LUAT_DEBUG_PRINT("json data = %s\n", rendered);

	LUAT_DEBUG_PRINT("######## Test 07 ########\n");
	LUAT_DEBUG_PRINT("root json size = %d\n", cJSON_GetArraySize(root));
	LUAT_DEBUG_PRINT("format json size = %d\n", cJSON_GetArraySize(format));
	
	LUAT_DEBUG_PRINT("######## Test 08 ########\n");
	cJSON *json_item1 = cJSON_GetArrayItem(root, 0);
	cJSON *json_item2 = cJSON_GetArrayItem(root, 1);
	LUAT_DEBUG_PRINT("json_item1 data = %s\n", cJSON_Print(json_item1));
	LUAT_DEBUG_PRINT("json_item2 data = %s\n", cJSON_Print(json_item2));

	LUAT_DEBUG_PRINT("######## Test 09 ########\n");
	cJSON* format1 = cJSON_GetObjectItem(root, "format");
	cJSON* framerate_item1 = cJSON_GetObjectItem(format1, "frame rate");
	LUAT_DEBUG_PRINT("json data = %s\n", cJSON_Print(format1));
	LUAT_DEBUG_PRINT("json data = %s\n", cJSON_Print(framerate_item1));
	
	LUAT_DEBUG_PRINT("######## Test 10 ########\n");
	cJSON_bool isHasItem1 = cJSON_HasObjectItem(root, "format");
	cJSON_bool isHasItem2 = cJSON_HasObjectItem(root, "formai");
	LUAT_DEBUG_PRINT("has format item = %d\n", (int)isHasItem1);
	LUAT_DEBUG_PRINT("has formai item = %d\n", (int)isHasItem2);

	LUAT_DEBUG_PRINT("######## Test 06 ########\n");
	cJSON_Delete(format);
	LUAT_DEBUG_PRINT("json data = %s\n", cJSON_Print(root));

	LUAT_DEBUG_PRINT("######## Test 11 ########\n");
	cJSON* cjson_null = cJSON_CreateNull();
	LUAT_DEBUG_PRINT("json data = %s\n", cJSON_Print(cjson_null));
	LUAT_DEBUG_PRINT("json data is null or not = %d\n", (int)cJSON_IsNull(cjson_null));
	cJSON_Delete(cjson_null);

	cJSON* cjson_true = cJSON_CreateTrue();
	LUAT_DEBUG_PRINT("json data = %s\n", cJSON_Print(cjson_true));
	LUAT_DEBUG_PRINT("json data is true or not = %d\n", (int)cJSON_IsTrue(cjson_true));
	cJSON_Delete(cjson_true);

	cJSON* cjson_false = cJSON_CreateFalse();
	LUAT_DEBUG_PRINT("json data = %s\n", cJSON_Print(cjson_false));
	LUAT_DEBUG_PRINT("json data is false or not = %d\n", (int)cJSON_IsFalse(cjson_false));
	cJSON_Delete(cjson_false);


	cJSON_bool bool_type = (cJSON_bool)1;
	cJSON* cjson_bool = cJSON_CreateBool(bool_type);
	LUAT_DEBUG_PRINT("json data = %s\n", cJSON_Print(cjson_bool));
	LUAT_DEBUG_PRINT("json data is bool or not = %d\n", (int)cJSON_IsBool(cjson_bool));
	cJSON_Delete(cjson_bool);

	double number_type = 88;
	cJSON* cjson_number = cJSON_CreateNumber(number_type);
	LUAT_DEBUG_PRINT("json data = %s\n", cJSON_Print(cjson_number));
	LUAT_DEBUG_PRINT("json data is number or not = %d\n", (int)cJSON_IsNumber(cjson_number));
	cJSON_Delete(cjson_number);
		
	char* string_type = "Welcome to luatOS";
	cJSON* cjson_string = cJSON_CreateString(string_type);
	LUAT_DEBUG_PRINT("json data = %s\n", cJSON_Print(cjson_string));
	LUAT_DEBUG_PRINT("json data is string or not = %d\n", (int)cJSON_IsString(cjson_string));
	cJSON_Delete(cjson_string);

	char* raw_type = "Welcome to luatOS raw";
	cJSON* cjson_raw = cJSON_CreateRaw(raw_type);
	LUAT_DEBUG_PRINT("json data = %s\n", cJSON_Print(cjson_raw));
	LUAT_DEBUG_PRINT("json data is raw or not = %d\n", (int)cJSON_IsRaw(cjson_raw));
	cJSON_Delete(cjson_raw);

	cJSON* cjson_array = cJSON_CreateArray();
	LUAT_DEBUG_PRINT("json data = %s\n", cJSON_Print(cjson_array));
	LUAT_DEBUG_PRINT("json data is array or not = %d\n", (int)cJSON_IsArray(cjson_array));
	cJSON_Delete(cjson_array);

	cJSON* cjson_object = cJSON_CreateObject();
	LUAT_DEBUG_PRINT("json data = %s\n", cJSON_Print(cjson_object));
	LUAT_DEBUG_PRINT("json data is object or not = %d\n", (int)cJSON_IsObject(cjson_object));
	cJSON_Delete(cjson_object);
	
}

void test_json_2()
{
	cJSON *root;
	cJSON *fmt;
	root = cJSON_CreateObject();
	cJSON_AddItemToObject(root, "name", cJSON_CreateString("Jack (\"Bee\") Nimble"));
	cJSON_AddItemToObject(root, "format", fmt = cJSON_CreateObject());
	cJSON_AddStringToObject(fmt, "type", "rect");
	cJSON_AddNumberToObject(fmt, "width", 1920);
	cJSON_AddNumberToObject(fmt, "height", 1080);
	cJSON_AddFalseToObject(fmt, "interlace");
	cJSON_AddNumberToObject(fmt, "frame rate", 24);
	
	char* result = cJSON_Print(root);
	LUAT_DEBUG_PRINT("######## Test 03 ########\n");
	LUAT_DEBUG_PRINT("json data = %s\n", result);
	
	LUAT_DEBUG_PRINT("######## Test 04 ########\n");
	LUAT_DEBUG_PRINT("json version = %s\n", cJSON_Version());

	LUAT_DEBUG_PRINT("######## Test 05 ########\n");
	LUAT_DEBUG_PRINT("json data = %s\n", cJSON_PrintUnformatted(root));

	LUAT_DEBUG_PRINT("######## Test 12 ########\n");
	cJSON_AddNullToObject(root, "null");
	cJSON_AddTrueToObject(root, "true");
	cJSON_AddFalseToObject(root, "false");
	cJSON_AddBoolToObject(root, "bool", 1);
	cJSON_AddNumberToObject(root, "number", 88);
	cJSON_AddStringToObject(root, "string", "Welcome to luatOS");
	cJSON_AddRawToObject(root, "Raw", "Welcome to luatOS raw");
	LUAT_DEBUG_PRINT("json data = %s\n", cJSON_Print(root));
	
	
	LUAT_DEBUG_PRINT("######## Test 06 ########\n");
	cJSON_Delete(fmt);
	LUAT_DEBUG_PRINT("json data = %s\n", cJSON_Print(root));
	
	
	LUAT_DEBUG_PRINT("######## Test 11 ########\n");
	int a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	cJSON * int_array_root = cJSON_CreateIntArray(a, 10);
	LUAT_DEBUG_PRINT("json data = %s\n", cJSON_Print(int_array_root));
	cJSON_AddItemToArray(int_array_root, cJSON_CreateNumber(11));
	LUAT_DEBUG_PRINT("json data = %s\n", cJSON_Print(int_array_root));
	cJSON_Delete(int_array_root);
	
	//float b[] = {1.1, 2.2, 3.3, 4.4, 5.5, 6.5, 7.5, 8.5, 9.5, 10.5};
	float b[] = {1.5, 2.5, 3.5, 4.5, 5.5, 6.5, 7.5, 8.5, 9.5, 10.5};
	cJSON * float_array_root = cJSON_CreateFloatArray(b, 10);
	LUAT_DEBUG_PRINT("json data = %s\n", cJSON_Print(float_array_root));
	cJSON_AddItemToArray(float_array_root, cJSON_CreateNumber(11.5));
	LUAT_DEBUG_PRINT("json data = %s\n", cJSON_Print(float_array_root));
	cJSON_Delete(float_array_root);

	double c[] = {1.1, 2.2, 3.3, 4.4, 5.5, 6.5, 7.5, 8.5, 9.5, 10.5};
	//double c[] = {1.5, 2.5, 3.5, 4.5, 5.5, 6.5, 7.5, 8.5, 9.5, 10.5};
	cJSON * double_array_root = cJSON_CreateDoubleArray(c, 10);
	LUAT_DEBUG_PRINT("json data = %s\n", cJSON_Print(double_array_root));
	cJSON_AddItemToArray(double_array_root, cJSON_CreateNumber(11.5));
	LUAT_DEBUG_PRINT("json data = %s\n", cJSON_Print(double_array_root));
	cJSON_Delete(double_array_root);

	// note: must const char ** 
	const char* d[] = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten"};
	cJSON * string_array_root = cJSON_CreateStringArray(d, 10);
	LUAT_DEBUG_PRINT("json data = %s\n", cJSON_Print(string_array_root));
	cJSON_AddItemToArray(string_array_root, cJSON_CreateString("eleven"));
	LUAT_DEBUG_PRINT("json data = %s\n", cJSON_Print(string_array_root));
	cJSON_Delete(string_array_root);
	
}

static void usb_serial_input_dummy_cb(uint8_t channel, uint8_t *input, uint32_t len)
{
	DBG("dev_sim_example usb serial get %dbyte, test mode, send back", len);
	usb_serial_output(channel, input, len);
}

void cjson_demo_init()
{
	LUAT_DEBUG_PRINT("The cJSON version: %s", cJSON_Version());

	char json_data[FILE_LENGTH] = "{\"name\": \"Jack (\"Bee\") Nimble\",\"format\": \
	{\"type\":\"rect\",\"width\":1920,\"height\":1080,\"interlace\": false,\"frame rate\": 24}}";
	
	LUAT_DEBUG_PRINT("json_data = %s\n", json_data);

	// test
	test_json_1(json_data);
	test_json_2();


}

static void cjson_demo_task(void *arg)
{
	while (1)
	{
		LUAT_DEBUG_PRINT("cjson demo is running!");
		cjson_demo_init();
		luat_rtos_task_sleep(1000);
	}
}

static void task_demoE_init(void)
{
	luat_rtos_task_create(NULL, 4*1024, 50, "cjson", cjson_demo_task, NULL, 0);
}

//启动hw_demoA_init，启动位置硬件初始1级
// INIT_HW_EXPORT(hw_demoA_init, "1");
//启动hw_demoB_init，启动位置硬件初始2级
// INIT_HW_EXPORT(hw_demoB_init, "2");
//启动dr_demoC_init，启动位置驱动1级
// INIT_DRV_EXPORT(dr_demoC_init, "1");
// 启动dr_demoD_init，启动位置驱动2级
// INIT_DRV_EXPORT(dr_demoD_init, "2");
//启动task_demoE_init，启动位置任务1级
INIT_TASK_EXPORT(task_demoE_init, "1");
//启动task_demoF_init，启动位置任务2级
// INIT_TASK_EXPORT(task_demoF_init, "2");
