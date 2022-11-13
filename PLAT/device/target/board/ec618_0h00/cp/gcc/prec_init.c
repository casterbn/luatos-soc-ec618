#include <string.h>
#include "commontypedef.h"
#include "ec618_internal.h"
#include "cache.h"
#include "mpu_armv7.h"
#include "mem_map.h"
#include "cpmu.h"
#include "exception_process.h"

typedef struct
{
    UINT32 *base_addr;
    INT32 offset;
    UINT32 size;
    UINT8 access_permission;
    UINT8 cacheable;
    UINT8 excute_disabled;
}mpu_setting_t;


#if defined(__CC_ARM)
__asm void __fast_memset(UINT32 *dst, UINT32 value, UINT32 length)
{
    push {r4-r9}
    cmp r2, #0
    beq memset_return
    mov r4, r1
    mov r5, r1
    mov r6, r1
    mov r7, r1
    and r8, r2,#0xf
    mov r9, r8
    cmp r8, #0
    beq clr_16Byte
clr_4Byte
    stmia r0!,{r4}
    subs r8,r8,#4
    cmp r8,#0
    bne clr_4Byte
    cmp r2, #0xf
    bls memset_return
    sub r2,r2,r9
clr_16Byte
    stmia r0!,{r4,r5,r6,r7}
    subs r2,r2,#16
    cmp r2,#0
    bne clr_16Byte
memset_return
    pop {r4-r9}
    bx lr
}
#elif defined(__GNUC__)
__attribute__((__noinline__)) void __fast_memset(UINT32 *dst, UINT32 value, UINT32 length)
{
    asm volatile(
    "push {r4-r9}\n\t"
    "cmp r2, #0\n\t"
    "beq memset_return\n\t"
    "mov r4, r1\n\t"
    "mov r5, r1\n\t"
    "mov r6, r1\n\t"
    "mov r7, r1\n\t"
    "and r8, r2,#0xf\n\t"
    "mov r9, r8\n\t"
    "cmp r8, #0\n\t"
    "beq clr_16Byte\n\t"
"clr_4Byte:\n\t"
    "stmia r0!,{r4}\n\t"
    "subs r8,r8,#4\n\t"
    "cmp r8,#0\n\t"
    "bne clr_4Byte\n\t"
    "cmp r2, #0xf\n\t"
    "bls memset_return\n\t"
    "sub r2,r2,r9\n\t"
"clr_16Byte:\n\t"
    "stmia r0!,{r4,r5,r6,r7}\n\t"
    "subs r2,r2,#16\n\t"
    "cmp r2,#0\n\t"
    "bne clr_16Byte\n\t"
"memset_return:\n\t"
    "pop {r4-r9}\n\t"
    "bx lr\n\t"
    );
}
#endif

////////// LOAD_CPVECTOR ///////////////
// copy by ap code before cp power on //
////////////////////////////////////////

////////// LOAD_CPOS_IRAM //////////////
extern UINT32 Load$$LOAD_CPOS_IRAM$$Base;
extern UINT32 Image$$LOAD_CPOS_IRAM$$Base;
extern UINT32 Image$$LOAD_CPOS_IRAM$$Length;
////////////////////////////////////////

///////////// LOAD_CP_PIRAM_CSMB //////////////
extern UINT32 Load$$LOAD_CP_PIRAM_CSMB$$Base    ;
extern UINT32 Image$$LOAD_CP_PIRAM_CSMB$$Base   ;
extern UINT32 Image$$LOAD_CP_PIRAM_CSMB$$Length ;
/////////////////////////////////////////

///////////// LOAD_CP_SLP2PIRAM_MSMB //////////////
extern UINT32 Load$$LOAD_CP_SLP2PIRAM_MSMB$$Base    ; 
extern UINT32 Image$$LOAD_CP_SLP2PIRAM_MSMB$$Base   ;  
extern UINT32 Image$$LOAD_CP_SLP2PIRAM_MSMB$$Length ; 
/////////////////////////////////////////

///////////// LOAD_CP_PIRAM_MSMB //////////////
extern UINT32 Load$$LOAD_CP_PIRAM_MSMB$$Base    ;
extern UINT32 Image$$LOAD_CP_PIRAM_MSMB$$Base   ;
extern UINT32 Image$$LOAD_CP_PIRAM_MSMB$$Length ;
/////////////////////////////////////////

///////////// LOAD_CP_FIRAM_CSMB //////////////
extern UINT32 Load$$LOAD_CP_FIRAM_CSMB$$Base    ;
extern UINT32 Image$$LOAD_CP_FIRAM_CSMB$$Base   ;
extern UINT32 Image$$LOAD_CP_FIRAM_CSMB$$Length ;
/////////////////////////////////////////

///////////// LOAD_CP_FIRAM_MSMB //////////////
extern UINT32 Load$$LOAD_CP_FIRAM_MSMB$$Base;
extern UINT32 Image$$LOAD_CP_FIRAM_MSMB$$Base;
extern UINT32 Image$$LOAD_CP_FIRAM_MSMB$$Length;
/////////////////////////////////////////


///////////// LOAD_DRAM_BSP /////////////
extern UINT32 Load$$LOAD_CPDRAM_BSP$$Base ;
extern UINT32 Image$$LOAD_CPDRAM_BSP$$Base    ;
extern UINT32 Image$$LOAD_CPDRAM_BSP$$Length  ;
extern UINT32 Image$$LOAD_CPDRAM_BSP$$ZI$$Base;
extern UINT32 Image$$LOAD_CPDRAM_BSP$$ZI$$Limit;
/////////////////////////////////////////

////////////// LOAD_CPDRAM_SHARED ////////
extern UINT32 Load$$LOAD_CPDRAM_SHARED$$Base  ;
extern UINT32 Image$$LOAD_CPDRAM_SHARED$$Base ;
extern UINT32 Image$$LOAD_CPDRAM_SHARED$$Length   ;
extern UINT32 Image$$LOAD_CPDRAM_SHARED$$ZI$$Base;
extern UINT32 Image$$LOAD_CPDRAM_SHARED$$ZI$$Limit;

extern UINT32 Image$$UNLOAD_NOCACHE$$Base;

extern UINT32 __StackTop;
extern UINT32 __StackLimit;


const mpu_setting_t mpu_region[8] =     // eight mpu region at most
{
    // base_addr            offset  size                        access          cache   excute
    {0,                                0,      ARM_MPU_REGION_SIZE_32KB,     ARM_MPU_AP_RO,  1,      0},        // cp 0x0-0x8000
    {(UINT32  *)0x8000,                0,      ARM_MPU_REGION_SIZE_16KB,     ARM_MPU_AP_RO,  1,      0},        // cp 0x8000-0xB000
    {(UINT32  *)0xB000,                0,      ARM_MPU_REGION_SIZE_8KB,      ARM_MPU_AP_RO,  1,      0},        // cp 0xB000-0xE000
    {(UINT32  *)0x500000,              0,      ARM_MPU_REGION_SIZE_128KB,    ARM_MPU_AP_RO,  1,      0},   // cp code 0x500000-0x520000
    {(UINT32  *)0x520000,              0,      ARM_MPU_REGION_SIZE_32KB,     ARM_MPU_AP_RO,  1,      0},   // cp code 0x520000-0x528000
    {(UINT32  *)0x528000,              0,      ARM_MPU_REGION_SIZE_16KB,     ARM_MPU_AP_RO,  1,      0},   // cp code 0x528000-0x52C000
    {(UINT32  *)0x400000,              0,      ARM_MPU_REGION_SIZE_512KB,    ARM_MPU_AP_RO,  1,      0},   // ap region 0x400000-0x480000
    #ifdef LOW_SPEED_SERVICE_ONLY
    {(UINT32  *)0x480000,              0,      ARM_MPU_REGION_SIZE_256KB,    ARM_MPU_AP_RO,  1,      0},   // ap region 0x480000-0x4C0000, do not protect upbuffer
    #else
    {(UINT32  *)0x480000,              0,      ARM_MPU_REGION_SIZE_64KB,     ARM_MPU_AP_RO,  1,      0},   // ap region 0x480000-0x490000, do not protect upbuffer
    #endif
};

#pragma GCC push_options
#pragma GCC optimize("O1")

void SetStackLimitFlag(void)
{
    uint32_t *stack_limit = &__StackLimit;
    *stack_limit = 0x44444444;
}

bool pIramMsmbCopyBeforePaging(void)
{
    uint8_t cpBootFlag = cpmuGetBootFlag();
    if ((CP_BOOT_FROM_CS2 == cpBootFlag) && (TRUE == AonRegGetCpMsmbSlp2RetFlagBeforeInit()))
    {
        cpmuSetPIRamMSMBCopyFlag(true);
        return true;
    }
    else
    {
        cpmuSetPIRamMSMBCopyFlag(false);
        return false;
    }
}


void CopyDataRWZIForPhyProcess(void)     // may optimise by using different sector
{
    UINT32 *start_addr;
    UINT32 *end_addr  ;
    UINT32 *src;
    UINT32 *dst;
    UINT32 length;

    dst    = &(Image$$LOAD_CPDRAM_BSP$$Base);
    src    = &(Load$$LOAD_CPDRAM_BSP$$Base);
    length = (UINT32)&(Image$$LOAD_CPDRAM_BSP$$Length);
    length /= sizeof(UINT32);

    if(dst != src)
    {
        while(length >0)
        {
          dst[length-1]=src[length-1];
          length--;
        }
    }

    start_addr = &(Image$$LOAD_CPDRAM_BSP$$ZI$$Base) ;
    end_addr   = &(Image$$LOAD_CPDRAM_BSP$$ZI$$Limit);
    length = (UINT32)end_addr - (UINT32)start_addr;
    __fast_memset((UINT32 *)start_addr, 0, length);

}


// called by CPFullImageInit when power on or transfer to full image from paging image
void SetFullEnvForCPProcess(void)
{
    UINT32 *src;
    UINT32 *dst;
    UINT32 length;

    uint8_t cpBootFlag = CP_BOOT_FROM_POWER_ON;  // all data need copy, as we do not know the bootflag before this time

    // when wakeup from CO/CH or CS2 withou Meas, need to copy MSMB_SLP2 when transfer to full image
    // for other case, MSMB_SLP2 has been copied in CopyRamCodeForPhyProcess
    // when wakeup from paging sleep1, still need to copy code
    if(cpmuGetPIRamMSMBCopyFlag() == false)
    {
        // load_cp_slp2piram_msmb(phyCodeMsmb_SLP2)
        dst    = &(Image$$LOAD_CP_SLP2PIRAM_MSMB$$Base);
        src    = &(Load$$LOAD_CP_SLP2PIRAM_MSMB$$Base);
        length = (unsigned int)&(Image$$LOAD_CP_SLP2PIRAM_MSMB$$Length);
        length /= sizeof(unsigned int);
    
        if(dst != src)
        {
            while(length >0)
            {
                dst[length-1]=src[length-1];
                length--;
            }
        }
    }
    switch(cpBootFlag)
    {
        case CP_BOOT_FROM_POWER_ON:
        case CP_BOOT_FROM_CO:
        case CP_BOOT_FROM_CH:
        {
            dst    = &(Image$$LOAD_CP_FIRAM_CSMB$$Base);
            src    = &(Load$$LOAD_CP_FIRAM_CSMB$$Base);
            length = (UINT32)&(Image$$LOAD_CP_FIRAM_CSMB$$Length);
            length /= sizeof(UINT32);

            if(dst != src)
            {
                while(length >0)
                {
                    dst[length-1]=src[length-1];
                    length--;
                }
            }
        }
        case CP_BOOT_FROM_CS2:
        {
            // load_cp_firam_msmb(phyCodeMsmb_XXX)
            dst    = &(Image$$LOAD_CP_FIRAM_MSMB$$Base);
            src    = &(Load$$LOAD_CP_FIRAM_MSMB$$Base);
            length = (unsigned int)&(Image$$LOAD_CP_FIRAM_MSMB$$Length);
            length /= sizeof(unsigned int);

            if(dst != src)
            {
                while(length >0)
                {
                    dst[length-1]=src[length-1];
                    length--;
                }
            }

            // load_cpos(CP PLAT code in MSMB)
            dst    = &(Image$$LOAD_CPOS_IRAM$$Base);
            src    = &(Load$$LOAD_CPOS_IRAM$$Base);
            length = (UINT32)&(Image$$LOAD_CPOS_IRAM$$Length);
            length /= sizeof(UINT32);

            if(dst != src)
            {
                while(length >0)
                {
                    dst[length-1]=src[length-1];
                    length--;
                }
            }
        }
        case CP_BOOT_FROM_CS1:
            break;
        default:
            EC_ASSERT(0, cpBootFlag, 0, 0);
    }

    // load_cpdram_shared_data(phyDataMsmb,phyShareDataMsmb)
    dst    = &(Image$$LOAD_CPDRAM_SHARED$$Base);
    src    = &(Load$$LOAD_CPDRAM_SHARED$$Base);
    length = (UINT32)&(Image$$LOAD_CPDRAM_SHARED$$Length);
    length /= sizeof(UINT32);

    if(dst != src)
    {
        while(length >0)
        {
            dst[length-1]=src[length-1];
            length--;
        }
    }

    // load_cpdram_shared_bss(cpPlatDataMsmb)
    UINT32 *start_addr;
    UINT32 *end_addr  ;
    UINT32* stack_len = (UINT32*)(&__StackTop - &__StackLimit);
    start_addr = &(Image$$LOAD_CPDRAM_SHARED$$ZI$$Base) ;
    end_addr   = &(Image$$LOAD_CPDRAM_SHARED$$ZI$$Limit);
    length = (UINT32)end_addr - (UINT32)start_addr;
    __fast_memset((UINT32 *)start_addr, 0, length-(UINT32)stack_len);

    CopyDataRWZIForPhyProcess();

}


// called by systemInit when poweron or wakeup from CO/CH/CS2
void CopyRamCodeForPhyProcess(void)
{
    UINT32 *src;
    UINT32 *dst;
    UINT32 length;

    uint8_t cpBootFlag = cpmuGetBootFlag();

    switch(cpBootFlag)
    {
        case CP_BOOT_FROM_POWER_ON:
        case CP_BOOT_FROM_CO:
        case CP_BOOT_FROM_CH:
        {
            // load_cp_piram_csmb(phyCodeCsmb_XXX)
            dst    = &(Image$$LOAD_CP_PIRAM_CSMB$$Base);
            src    = &(Load$$LOAD_CP_PIRAM_CSMB$$Base);
            length = (unsigned int)&(Image$$LOAD_CP_PIRAM_CSMB$$Length);
            length /= sizeof(unsigned int);

            if(dst != src)
            {
                while(length >0)
                {
                    dst[length-1]=src[length-1];
                    length--;
                }
            }
        }
        case CP_BOOT_FROM_CS2:
        {
            // when PowerOn or SLEEP2 with Meas, MSMB_SLP2 need to copy
            if (pIramMsmbCopyBeforePaging() == true)
            {
                // load_cp_slp2piram_msmb(phyCodeMsmb_SLP2)
                dst    = &(Image$$LOAD_CP_SLP2PIRAM_MSMB$$Base);
                src    = &(Load$$LOAD_CP_SLP2PIRAM_MSMB$$Base);
                length = (unsigned int)&(Image$$LOAD_CP_SLP2PIRAM_MSMB$$Length);
                length /= sizeof(unsigned int);

                if(dst != src)
                {
                    while(length >0)
                    {
                        dst[length-1]=src[length-1];
                        length--;
                    }
                }
            }

            // load_cp_piram_msmb  (phyCodeMsmb_HIB)
            dst    = &(Image$$LOAD_CP_PIRAM_MSMB$$Base);
            src    = &(Load$$LOAD_CP_PIRAM_MSMB$$Base);
            length = (unsigned int)&(Image$$LOAD_CP_PIRAM_MSMB$$Length);
            length /= sizeof(unsigned int);

            if(dst != src)
            {
                while(length >0)
                {
                    dst[length-1]=src[length-1];
                    length--;
                }
            }
            
        }
        break;

        default:
            EC_ASSERT(0, cpBootFlag, 0, 0);
    }

}

#pragma GCC pop_options

void mpu_init(void)
{
    int i=0;
    uint8_t region_num = 0;

    if(MPU->TYPE==0)
        return;

    ARM_MPU_Disable();

    for(i=0;i<8;i++)
        ARM_MPU_ClrRegion(i);

    region_num = sizeof(mpu_region)/sizeof(mpu_setting_t);

    for(i=0;i<region_num;i++)
    {
        ARM_MPU_SetRegionEx(i,((uint32_t)mpu_region[i].base_addr)+mpu_region[i].offset,ARM_MPU_RASR(mpu_region[i].excute_disabled,mpu_region[i].access_permission,0,0,mpu_region[i].cacheable,1,0,mpu_region[i].size));
    }

    ARM_MPU_Enable(MPU_CTRL_PRIVDEFENA_Msk);

}


void mpu_deinit(void)
{
    int i=0;

    if(MPU->TYPE==0)
        return;

    ARM_MPU_Disable();

    for(i=0;i<8;i++)
        ARM_MPU_ClrRegion(i);

}


