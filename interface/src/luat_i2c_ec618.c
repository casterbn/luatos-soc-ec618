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

#include "FreeRTOS.h"
#include "luat_i2c.h"
#include "common_api.h"
#include <stdio.h>
#include <string.h>
#include "bsp_custom.h"

#if RTE_I2C0
extern ARM_DRIVER_I2C Driver_I2C0;
static ARM_DRIVER_I2C   *i2cDrv0 = &CREATE_SYMBOL(Driver_I2C, 0);
#endif
#if RTE_I2C1
extern ARM_DRIVER_I2C Driver_I2C1;
static ARM_DRIVER_I2C   *i2cDrv1 = &CREATE_SYMBOL(Driver_I2C, 1);
#endif

int luat_i2c_exist(int id) {
    #if RTE_I2C0
    if (id == 0)
        return 1;
    #endif
    #if RTE_I2C1
    if (id == 1)
        return 1;
    #endif
    return 0;
}
int luat_i2c_setup(int id, int speed) {
    if (!luat_i2c_exist(id)) {
        return -1;
    }
    int ret = 0;
#if RTE_I2C0
    if (id==0)
    {
        ret = i2cDrv0->Initialize(NULL);
        if (ret)  DBG("i2c0 setup error -- Initialize - %ld", ret);
        ret = i2cDrv0->PowerControl(ARM_POWER_FULL);
        if (ret)  DBG("i2c0 setup error -- PowerControl - %ld", ret);
        if (speed == 0)//(100kHz)
            ret = i2cDrv0->Control(ARM_I2C_BUS_SPEED, ARM_I2C_BUS_SPEED_STANDARD);
        else if (speed == 1)//(400kHz)
            ret = i2cDrv0->Control(ARM_I2C_BUS_SPEED, ARM_I2C_BUS_SPEED_FAST);
        else if (speed == 2)//(  1MHz)
            ret = i2cDrv0->Control(ARM_I2C_BUS_SPEED, ARM_I2C_BUS_SPEED_FAST_PLUS);
        else if (speed == 3)//(3.4MHz)
            ret = i2cDrv0->Control(ARM_I2C_BUS_SPEED, ARM_I2C_BUS_SPEED_HIGH);
        if (ret)  DBG("i2c0 setup error -- Control SPEED - %ld", ret);
        ret = i2cDrv0->Control(ARM_I2C_BUS_CLEAR, 0);
        if (ret)  DBG("i2c0 setup error -- Control CLEAR - %ld", ret);
        DBG("i2c0 setup complete");
    }
#endif
#if RTE_I2C1
    if (id==1)
    {
        ret = i2cDrv1->Initialize(NULL);
        if (ret)  DBG("i2c1 setup error -- Initialize - %ld", ret);
        ret = i2cDrv1->PowerControl(ARM_POWER_FULL);
        if (ret)  DBG("i2c1 setup error -- PowerControl - %ld", ret);
        if (speed == 0)//(100kHz)
            ret = i2cDrv1->Control(ARM_I2C_BUS_SPEED, ARM_I2C_BUS_SPEED_STANDARD);
        else if (speed == 1)//(400kHz)
            ret = i2cDrv1->Control(ARM_I2C_BUS_SPEED, ARM_I2C_BUS_SPEED_FAST);
        else if (speed == 2)//(  1MHz)
            ret = i2cDrv1->Control(ARM_I2C_BUS_SPEED, ARM_I2C_BUS_SPEED_FAST_PLUS);
        else if (speed == 3)//(3.4MHz)
            ret = i2cDrv1->Control(ARM_I2C_BUS_SPEED, ARM_I2C_BUS_SPEED_HIGH);
        if (ret)  DBG("i2c1 setup error -- Control SPEED - %ld", ret);
        ret = i2cDrv1->Control(ARM_I2C_BUS_CLEAR, 0);
        if (ret)  DBG("i2c1 setup error -- Control CLEAR - %ld", ret);
        DBG("i2c1 setup complete");
    }
#endif
    return ret;
}

int luat_i2c_close(int id) {
    if (!luat_i2c_exist(id)) {
        return -1;
    }
#if RTE_I2C0
    if (id==0){
        i2cDrv0->PowerControl(ARM_POWER_OFF);
        i2cDrv0->Uninitialize();
    }
#endif
#if RTE_I2C1
    if (id==1){
        i2cDrv1->PowerControl(ARM_POWER_OFF);
        i2cDrv1->Uninitialize();
    }
#endif
    return 0;
}

int luat_i2c_send(int id, int addr, void* buff, size_t len, uint8_t stop) {
    if (!luat_i2c_exist(id)) {
        return -1;
    }
#if RTE_I2C0
    if (id==0){
        if (stop)
            return i2cDrv0->MasterTransmit(addr, buff, len, false);
        else
            return i2cDrv0->MasterTransmit(addr, buff, len, true);
    }
#endif
#if RTE_I2C1
    if (id==1){
        if (stop)
            return i2cDrv1->MasterTransmit(addr, buff, len, false);
        else
            return i2cDrv1->MasterTransmit(addr, buff, len, true);
    }
#endif
}

int luat_i2c_recv(int id, int addr, void* buff, size_t len) {
    if (!luat_i2c_exist(id)) {
        return -1;
    }
#if RTE_I2C0
    if (id==0){
        return i2cDrv0->MasterReceive(addr, buff, len, false);
    }
#endif
#if RTE_I2C1
    if (id==1){
        return i2cDrv1->MasterReceive(addr, buff, len, false);
    }
#endif
    
}

