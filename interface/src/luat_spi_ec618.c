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


#include "luat_base.h"
#include "luat_spi.h"

#include "common_api.h"
#include <stdio.h>
#include <string.h>
#include "bsp_custom.h"

/** \brief SPI data width */
#define TRANFER_DATA_WIDTH_8_BITS        (8)
#define TRANFER_DATA_WIDTH_16_BITS       (16)
#define TRANSFER_DATA_WIDTH              TRANFER_DATA_WIDTH_8_BITS

#if RTE_SPI0
extern ARM_DRIVER_SPI Driver_SPI0;
static ARM_DRIVER_SPI *spiDrv0 = &CREATE_SYMBOL(Driver_SPI, 0);
#endif
#if RTE_SPI1
extern ARM_DRIVER_SPI Driver_SPI1;
static ARM_DRIVER_SPI *spiDrv1 = &CREATE_SYMBOL(Driver_SPI, 1);
#endif

static int spi_exist(int id) {
    #if RTE_SPI0
    if (id == 0)
        return 1;
    #endif
    #if RTE_SPI1
    if (id == 1)
        return 1;
    #endif
    return 0;
}

#ifdef __LUATOS__

int luat_spi_device_config(luat_spi_device_t* spi_dev) {
    int bus_id = spi_dev->bus_id;
    if (!spi_exist(bus_id))
        return -1;
    int ret = -1;
    uint32_t mark = ARM_SPI_MODE_MASTER;
    if (spi_dev->spi_config.CPOL == 0) {
        if (spi_dev->spi_config.CPHA == 0) {
            mark |= ARM_SPI_CPOL0_CPHA0;
        }
        else {
            mark |= ARM_SPI_CPOL0_CPHA1;
        }
    }
    else {
        if (spi_dev->spi_config.CPHA == 0) {
            mark |= ARM_SPI_CPOL1_CPHA0;
        }
        else {
            mark |= ARM_SPI_CPOL1_CPHA1;
        }
    }
    mark |= ARM_SPI_DATA_BITS(TRANSFER_DATA_WIDTH); // 固定8bit吧
    if (spi_dev->spi_config.bit_dict == 0) {
        mark |= ARM_SPI_MSB_LSB;
    }
    else {
        mark |= ARM_SPI_MSB_LSB;
    }
    mark |= ARM_SPI_SS_MASTER_UNUSED;
    
#if RTE_SPI0
    if (bus_id==0){
        ret = spiDrv0->Control(mark, spi_dev->spi_config.bandrate > 0 ? spi_dev->spi_config.bandrate : 25600000U);
    }
#endif
#if RTE_SPI1
    if(bus_id==1){
        ret = spiDrv1->Control(mark, spi_dev->spi_config.bandrate > 0 ? spi_dev->spi_config.bandrate : 25600000U);
    }
#endif
    return ret;
}

int luat_spi_bus_setup(luat_spi_device_t* spi_dev){
    int bus_id = spi_dev->bus_id;
    if (!spi_exist(bus_id))
        return -1;
    int ret = -1;
    CLOCK_clockEnable(CLK_HF51M); // open 51M
#if RTE_SPI0
    if (bus_id==0){
        CLOCK_setClockSrc(FCLK_SPI0, FCLK_SPI0_SEL_51M); // choose 51M
        ret = spiDrv0->Initialize(NULL);
        ret = spiDrv0->PowerControl(ARM_POWER_FULL);
    }
#endif
#if RTE_SPI1
    if(bus_id==1){
        CLOCK_setClockSrc(FCLK_SPI1, FCLK_SPI1_SEL_51M); // choose 51M
        ret = spiDrv1->Initialize(NULL);
        ret = spiDrv1->PowerControl(ARM_POWER_FULL);
    }
#endif
    return ret;
}

#endif

int luat_spi_setup(luat_spi_t* spi) {
    uint8_t spi_id = spi->id;
    if (!spi_exist(spi_id))
        return -1;
    int ret = -1;
    CLOCK_clockEnable(CLK_HF51M); // open 51M
#if RTE_SPI0
    if (spi_id==0){
        CLOCK_setClockSrc(FCLK_SPI0, FCLK_SPI0_SEL_51M); // choose 51M
        ret = spiDrv0->Initialize(NULL);
        ret = spiDrv0->PowerControl(ARM_POWER_FULL);
    }
#endif
#if RTE_SPI1
    if(spi_id==1){
        CLOCK_setClockSrc(FCLK_SPI1, FCLK_SPI1_SEL_51M); // choose 51M
        ret = spiDrv1->Initialize(NULL);
        ret = spiDrv1->PowerControl(ARM_POWER_FULL);
    }
#endif
    // Configure slave spi bus
    uint32_t mark = ARM_SPI_MODE_MASTER;
    if (spi->CPOL == 0) {
        if (spi->CPHA == 0) {
            mark |= ARM_SPI_CPOL0_CPHA0;
        }
        else {
            mark |= ARM_SPI_CPOL0_CPHA1;
        }
    }
    else {
        if (spi->CPHA == 0) {
            mark |= ARM_SPI_CPOL1_CPHA0;
        }
        else {
            mark |= ARM_SPI_CPOL1_CPHA1;
        }
    }
    mark |= ARM_SPI_DATA_BITS(TRANSFER_DATA_WIDTH); // 固定8bit吧
    if (spi->bit_dict == 0) {
        mark |= ARM_SPI_MSB_LSB;
    }
    else {
        mark |= ARM_SPI_MSB_LSB;
    }
    mark |= ARM_SPI_SS_MASTER_UNUSED;
#if RTE_SPI0
    if (spi_id==0){
        ret = spiDrv0->Control(mark, spi->bandrate > 0 ? spi->bandrate : 25600000U);
    }
#endif
#if RTE_SPI1
    if(spi_id==1){
        ret = spiDrv1->Control(mark, spi->bandrate > 0 ? spi->bandrate : 25600000U);
    }
#endif
    return ret;
}

//关闭SPI，成功返回0
int luat_spi_close(int spi_id) {
    if (!spi_exist(spi_id))
        return -1;
#if RTE_SPI0
    if (spi_id==0){
        spiDrv0->Uninitialize();
        spiDrv0->PowerControl(ARM_POWER_OFF);
    }
#endif
#if RTE_SPI1
    if(spi_id==1){
        spiDrv1->Uninitialize();
        spiDrv1->PowerControl(ARM_POWER_OFF);
    }
#endif
    return 0;
}

//收发SPI数据，返回接收字节数
int luat_spi_transfer(int spi_id, const char* send_buf, size_t send_length, char* recv_buf, size_t recv_length) {
    if (!spi_exist(spi_id))
        return -1;
#if RTE_SPI0
    if (spi_id==0){
        if(spiDrv0->Transfer(send_buf, recv_buf, recv_length) == ARM_DRIVER_OK)
            return recv_length;
    }
#endif
#if RTE_SPI1
    if(spi_id==1){
        if(spiDrv1->Transfer(send_buf, recv_buf, recv_length) == ARM_DRIVER_OK)
            return recv_length;
    }
#endif
    return 0;
}

//收SPI数据，返回接收字节数
int luat_spi_recv(int spi_id, char* recv_buf, size_t length) {
    if (!spi_exist(spi_id))
        return -1;
    char* send_buff = (char*)malloc(length);
    if(!send_buff) {
        return 0;
    }
#if RTE_SPI0
    memset(send_buff,0,length);
    int result = -1;
    if (spi_id==0){
        result = spiDrv0->Transfer(send_buff, recv_buf, length);
    }
#endif
#if RTE_SPI1
    if(spi_id==1){
        result = spiDrv1->Transfer(send_buff, recv_buf, length);
    }
#endif
    free(send_buff);
    if(result == ARM_DRIVER_OK)
        return length;
    return result;
}
//发SPI数据，返回发送字节数
int luat_spi_send(int spi_id, const char* send_buf, size_t length) {
    if (!spi_exist(spi_id))
        return -1;
    char* recv_buff = (char*)malloc(length);
    if(!recv_buff)
        return 0;
    int result = -1;
#if RTE_SPI0
    if (spi_id==0){
        result = spiDrv0->Transfer(send_buf, recv_buff, length);
    }
#endif
#if RTE_SPI1
    if(spi_id==1){
        result = spiDrv1->Transfer(send_buf, recv_buff, length);
    }
#endif
    free(recv_buff);
    if(result == ARM_DRIVER_OK)
        return length;
    return 0;
}
