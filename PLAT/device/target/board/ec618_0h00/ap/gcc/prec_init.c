#include <string.h>
#include "commontypedef.h"
#include "ec618.h"
#include "cache.h"
#include "mpu_armv7.h"
#include "mem_map.h"
#include "exception_process.h"
#include "apmu_external.h"

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



extern UINT32 Load$$LOAD_BOOTCODE$$Base    ;
extern UINT32 Image$$LOAD_BOOTCODE$$Base   ;
extern UINT32 Image$$LOAD_BOOTCODE$$Length ;

extern UINT32 Load$$LOAD_APOS$$Base    ;
extern UINT32 Image$$LOAD_APOS$$Base   ;
extern UINT32 Image$$LOAD_APOS$$Length ;

extern UINT32 Load$$LOAD_DRAM_SHARED$$Base  ;
extern UINT32 Image$$LOAD_DRAM_SHARED$$Base ;
extern UINT32 Image$$LOAD_DRAM_SHARED$$Length   ;
extern UINT32 Image$$LOAD_DRAM_SHARED$$ZI$$Base;
extern UINT32 Image$$LOAD_DRAM_SHARED$$ZI$$Limit;

extern UINT32 Load$$LOAD_DRAM_BSP$$Base  ;
extern UINT32 Image$$LOAD_DRAM_BSP$$Base ;
extern UINT32 Image$$LOAD_DRAM_BSP$$Length   ;
extern UINT32 Image$$LOAD_DRAM_BSP$$ZI$$Base;
extern UINT32 Image$$LOAD_DRAM_BSP$$ZI$$Limit;

extern UINT32 Load$$LOAD_AP_FIRAM_MSMB$$Base    ;
extern UINT32 Image$$LOAD_AP_FIRAM_MSMB$$Base   ;
extern UINT32 Image$$LOAD_AP_FIRAM_MSMB$$Length ;

extern UINT32 Load$$LOAD_AP_PIRAM_ASMB$$Base  ;
extern UINT32 Image$$LOAD_AP_PIRAM_ASMB$$Base ;
extern UINT32 Image$$LOAD_AP_PIRAM_ASMB$$Length   ;

extern UINT32 Load$$LOAD_AP_PIRAM_MSMB$$Base  ;
extern UINT32 Image$$LOAD_AP_PIRAM_MSMB$$Base ;
extern UINT32 Image$$LOAD_AP_PIRAM_MSMB$$Length   ;

extern UINT32 Load$$LOAD_AP_FIRAM_ASMB$$Base  ;
extern UINT32 Image$$LOAD_AP_FIRAM_ASMB$$Base ;
extern UINT32 Image$$LOAD_AP_FIRAM_ASMB$$Length   ;

extern UINT32 Load$$LOAD_AP_FDATA_ASMB$$RW$$Base;
extern UINT32 Image$$LOAD_AP_FDATA_ASMB$$RW$$Base;
extern UINT32 Image$$LOAD_AP_FDATA_ASMB$$Length;
extern UINT32 Image$$LOAD_AP_FDATA_ASMB$$ZI$$Base;
extern UINT32 Image$$LOAD_AP_FDATA_ASMB$$ZI$$Limit;

extern UINT32 Load$$LOAD_PS_FDATA_ASMB$$RW$$Base;
extern UINT32 Image$$LOAD_PS_FDATA_ASMB$$RW$$Base;
extern UINT32 Image$$LOAD_PS_FDATA_ASMB$$RW$$Length;
extern UINT32 Image$$LOAD_PS_FDATA_ASMB$$ZI$$Base;
extern UINT32 Image$$LOAD_PS_FDATA_ASMB$$ZI$$Limit;

extern UINT32 Image$$UNLOAD_NOCACHE$$Base;

extern UINT32 __StackTop;
extern UINT32 __StackLimit;

UINT32 Image$$ER_IROM1$$Base = 0;//temp define for usbc


const mpu_setting_t mpu_region[] =
{
    // base_addr                    offset  size                        access          cache   excute
    {0,                              0,        ARM_MPU_REGION_SIZE_8KB,    ARM_MPU_AP_RO,  1,    0},
    {&Image$$UNLOAD_NOCACHE$$Base,   0,        ARM_MPU_REGION_SIZE_128B,   ARM_MPU_AP_RO,  0,    0},
    {(UINT32 *)MSMB_APMEM_END_ADDR,  0,        ARM_MPU_REGION_SIZE_128KB,  ARM_MPU_AP_RO,  1,    0},  // cp region: 0x500000-0x520000
    {(UINT32 *)MSMB_APMEM_END_ADDR,  0x20000,  ARM_MPU_REGION_SIZE_64KB,   ARM_MPU_AP_RO,  1,    0},  // cp region: 0x520000-0x530000
    {(UINT32 *)MSMB_APMEM_END_ADDR,  0x30000,  ARM_MPU_REGION_SIZE_32KB,   ARM_MPU_AP_RO,  1,    0},  // cp region: 0x530000-0x538000
    {(UINT32 *)MSMB_APMEM_END_ADDR,  0x38000,  ARM_MPU_REGION_SIZE_16KB,   ARM_MPU_AP_RO,  1,    0},  // cp region: 0x538000-0x53c000
    {(UINT32 *)MSMB_APMEM_END_ADDR,  0x3C000,  ARM_MPU_REGION_SIZE_8KB,    ARM_MPU_AP_RO,  1,    0},  // cp region: 0x53c000-0x53e000
};

#pragma GCC push_options
#pragma GCC optimize("O1")
void psFDataInAsmbInit(void)
{
    UINT32 *src;
    UINT32 *dst;
    UINT32 length;
    UINT32 *start_addr;
    UINT32 *end_addr  ;

    dst    = &(Image$$LOAD_PS_FDATA_ASMB$$RW$$Base);
    src    = &(Load$$LOAD_PS_FDATA_ASMB$$RW$$Base);
    length = (UINT32)&(Image$$LOAD_PS_FDATA_ASMB$$RW$$Length);
    length /= sizeof(UINT32);

    if(dst != src)
    {
        while(length >0)
        {
            dst[length-1]=src[length-1];
            length--;
        }
    }

    start_addr = &(Image$$LOAD_PS_FDATA_ASMB$$ZI$$Base) ;
    end_addr   = &(Image$$LOAD_PS_FDATA_ASMB$$ZI$$Limit);
    length = (UINT32)end_addr - (UINT32)start_addr;
    __fast_memset((UINT32 *)start_addr, 0, length);

}


void platFDataInAsmbInit(void)
{
    UINT32 *start_addr;
    UINT32 *end_addr  ;
    UINT32 *src;
    UINT32 *dst;
    UINT32 length;

    dst    = &(Image$$LOAD_AP_FDATA_ASMB$$RW$$Base);
    src    = &(Load$$LOAD_AP_FDATA_ASMB$$RW$$Base);
    length = (UINT32)&(Image$$LOAD_AP_FDATA_ASMB$$Length);
    length /= sizeof(UINT32);
    
    if(dst != src)
    {
        while(length >0)
        {
            dst[length-1]=src[length-1];
            length--;
        }
    }

    start_addr = &(Image$$LOAD_AP_FDATA_ASMB$$ZI$$Base) ;
    end_addr   = &(Image$$LOAD_AP_FDATA_ASMB$$ZI$$Limit);
    length = (UINT32)end_addr - (UINT32)start_addr;
    __fast_memset((UINT32 *)start_addr, 0, length);

}


void SetZIDataToZero(void)
{
    UINT32 *start_addr;
    UINT32 *end_addr  ;
    UINT32 length;
    UINT32* stack_len = (UINT32*)(&__StackTop - &__StackLimit);

    start_addr = &(Image$$LOAD_DRAM_SHARED$$ZI$$Base) ;
    end_addr   = &(Image$$LOAD_DRAM_SHARED$$ZI$$Limit);
    length = (UINT32)end_addr - (UINT32)start_addr;
    __fast_memset((UINT32 *)start_addr, 0, length-(UINT32)stack_len);
}


void CopyRWDataFromBin(void)
{
    UINT32 *src;
    UINT32 *dst;
    UINT32 length;
    APBootFlag_e apBootFlag = apmuGetAPBootFlag();

    switch(apBootFlag)
    {
        case AP_BOOT_FROM_POWER_ON:
        case AP_BOOT_FROM_AO:
        case AP_BOOT_FROM_AH:
        {
            dst    = &(Image$$LOAD_AP_FIRAM_ASMB$$Base);
            src    = &(Load$$LOAD_AP_FIRAM_ASMB$$Base);
            length = (unsigned int)&(Image$$LOAD_AP_FIRAM_ASMB$$Length);
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
        case AP_BOOT_FROM_AS2:
        {
            dst    = &(Image$$LOAD_AP_FIRAM_MSMB$$Base);
            src    = &(Load$$LOAD_AP_FIRAM_MSMB$$Base);
            length = (unsigned int)&(Image$$LOAD_AP_FIRAM_MSMB$$Length);
            length /= sizeof(unsigned int);

            if(dst != src)
            {
                while(length >0)
                {
                    dst[length-1]=src[length-1];
                    length--;
                }
            }

            dst    = &(Image$$LOAD_APOS$$Base);
            src    = &(Load$$LOAD_APOS$$Base);
            length = (unsigned int)&(Image$$LOAD_APOS$$Length);
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
            EC_ASSERT(0, apBootFlag, 0, 0);
    }

    DisableICache();                    // flush cache when ramcode update
    EnableICache();

    dst    = &(Image$$LOAD_DRAM_SHARED$$Base);
    src    = &(Load$$LOAD_DRAM_SHARED$$Base);
    length = (UINT32)&(Image$$LOAD_DRAM_SHARED$$Length);
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


void CopyRamCodeForDeepSleep(void)
{
    UINT32 *src;
    UINT32 *dst;
    UINT32 length;

    APBootFlag_e apBootFlag = apmuGetAPLLBootFlag();
    switch(apBootFlag)
    {
        case AP_BOOT_FROM_POWER_ON:
        case AP_BOOT_FROM_AO:
        case AP_BOOT_FROM_AH:
        {
            dst    = &(Image$$LOAD_BOOTCODE$$Base);
            src    = &(Load$$LOAD_BOOTCODE$$Base);
            length = (unsigned int)&(Image$$LOAD_BOOTCODE$$Length);
            length /= sizeof(unsigned int);

            if(dst != src)
            {
                while(length >0)
                {
                    dst[length-1]=src[length-1];
                    length--;
                }
            }

            dst    = &(Image$$LOAD_AP_PIRAM_ASMB$$Base);
            src    = &(Load$$LOAD_AP_PIRAM_ASMB$$Base);
            length = (unsigned int)&(Image$$LOAD_AP_PIRAM_ASMB$$Length);
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
        case AP_BOOT_FROM_AS2:
        {
            dst    = &(Image$$LOAD_AP_PIRAM_MSMB$$Base);
            src    = &(Load$$LOAD_AP_PIRAM_MSMB$$Base);
            length = (unsigned int)&(Image$$LOAD_AP_PIRAM_MSMB$$Length);
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
            EC_ASSERT(0, apBootFlag, 0, 0);
    }

    DisableICache();        // flush cache when ramcode update
    EnableICache();

}


void CopyDataRWZIForDeepSleep(void)
{
    UINT32 *src;
    UINT32 *dst;
    UINT32 length;

    dst    = &(Image$$LOAD_DRAM_BSP$$Base);
    src    = &(Load$$LOAD_DRAM_BSP$$Base);
    length = (UINT32)&(Image$$LOAD_DRAM_BSP$$Length);
    length /= sizeof(UINT32);

    if(dst != src)
    {
        while(length >0)
        {
            dst[length-1]=src[length-1];
            length--;
        }
    }

    UINT32 *start_addr;
    UINT32 *end_addr  ;
    start_addr = &(Image$$LOAD_DRAM_BSP$$ZI$$Base) ;
    end_addr   = &(Image$$LOAD_DRAM_BSP$$ZI$$Limit);
    length = (UINT32)end_addr - (UINT32)start_addr;
    __fast_memset((UINT32 *)start_addr, 0, length);
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


void mpu_deinit()
{
    int i=0;

    if(MPU->TYPE==0)
        return;

    ARM_MPU_Disable();

    for(i=0;i<8;i++)
        ARM_MPU_ClrRegion(i);

}




