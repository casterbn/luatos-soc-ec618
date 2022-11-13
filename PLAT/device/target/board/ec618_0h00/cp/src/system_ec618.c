/**************************************************************************//**
 * @file     system_ARMCM3.c
 * @brief    CMSIS Device System Source File for
 *           ARMCM3 Device Series
 * @version  V5.00
 * @date     07. September 2016
 ******************************************************************************/
/*
 * Copyright (c) 2009-2016 ARM Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "ec618_internal.h"
#include "mpu_armv7.h"
#include "driver_common.h"
#include "cache.h"
#include "mem_map.h"
#include "slpman.h"
#include "aonreg.h"
#include "cpmureg.h"
#include "cpmu.h"

#define SYS_ADDRESS  CP_FLASH_LOAD_ADDR



extern void mpu_init(void);
extern void mpu_deinit(void);
extern void SetFullEnvForCPProcess(void);
extern void CopyRamCodeForPhyProcess(void);
extern void CopyDataRWZIForPhyProcess(void);
extern void SetStackLimitFlag(void);


/*----------------------------------------------------------------------------
  Externals
 *----------------------------------------------------------------------------*/
#if defined (__VTOR_PRESENT) && (__VTOR_PRESENT == 1U)
#if defined(__CC_ARM)
   extern uint32_t __Vectors;
#elif defined(__GNUC__)
   extern uint32_t __isr_vector;
#endif
#endif

/*----------------------------------------------------------------------------
  System Core Clock Variable
 *----------------------------------------------------------------------------*/
#define  CORE_CLOCK_REGISER_ADDRESS    (0x4d000020)
uint32_t SystemCoreClock = DEFAULT_SYSTEM_CLOCK;

/*----------------------------------------------------------------------------
  System Core Clock update function
 *----------------------------------------------------------------------------*/
void SystemCoreClockUpdate (void)
{
#ifdef FPGA_TEST
    SystemCoreClock = 102400000U;
#else
    GPR_apAccessEnter();
    switch((*((uint32_t *)CORE_CLOCK_REGISER_ADDRESS)) & 0x3)
    {
        case 0:
            SystemCoreClock = 26000000U;
            break;
        case 1:
            SystemCoreClock = 204800000U;
            break;
        case 2:
            SystemCoreClock = 102400000U;
            break;
        case 3:
            SystemCoreClock = 32768U;
            break;
    }
    GPR_apAccessExit();
#endif
}

/*----------------------------------------------------------------------------
  System initialization function
 *----------------------------------------------------------------------------*/
void CPFullImageInit (void)
{
    mpu_deinit();

    SetFullEnvForCPProcess();

    SystemCoreClockUpdate ();

    mpu_init();

    DisableICache();                    // flush cache when ramcode update
    
    EnableICache();   
}


void SystemInit (void)
{
    bool powerFlag;

#if defined (__VTOR_PRESENT) && (__VTOR_PRESENT == 1U)
#if defined(__CC_ARM)
    SCB->VTOR = (uint32_t) &__Vectors;
#elif defined (__GNUC__)
    SCB->VTOR = (uint32_t) &__isr_vector;
#endif
#endif

    DisableICache();

    EnableICache();

    //// set clk on temporary /////
#if 0
    (*(uint32_t *)0x50000008) = 0x1;

    GPR_RMI->XP2YPACCESS |= 3;
    /////////////////////
#endif

#ifdef FEATURE_DUMP_CHECK
    EXCEP_CHECK_POINT(80);
#endif

    powerFlag = cpmuRegGetPowerFlagBeforeInit();

    cpmuRestoreBootFlagBeforeInit();

    if((powerFlag != true) && (cpmuIsWakeupFromCS2() || cpmuIsWakeupFromCOCH() || cpmuIsFromPowerOn()))
    {
#ifdef FEATURE_DUMP_CHECK
        if(cpmuIsWakeupFromCS2())
        {
            EXCEP_CHECK_POINT(88);
        }
#endif        
        CopyRamCodeForPhyProcess();

        CopyDataRWZIForPhyProcess();
    }

    SetStackLimitFlag();

    mpu_init();

    DisableICache();        // flush cache when ramcode update
    
    EnableICache();

  /*set NVIC priority group as 4(bits 7,6,5 is group priority bits ),
    3 bits for group priority. Since our CM3 core only implemented 3
    bits for priority and freertos recommend all bits should be
    group priority bits*/
    NVIC_SetPriorityGrouping(4);

    //enable div 0 trap,then div 0 operation will trigger hardfault
    SCB->CCR|=SCB_CCR_DIV_0_TRP_Msk;

    SystemCoreClockUpdate();

}

