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


#if defined(__CC_ARM)


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
extern UINT32 Stack_Size;


#endif

const mpu_setting_t mpu_region[] =
{
    // base_addr            offset  size                        access          cache   excute
    {0,                                0,      ARM_MPU_REGION_SIZE_32KB,    ARM_MPU_AP_RO,  1,      0},        // cp 0x0-0x8000
//    {(UINT32  *)MSMB_START_ADDR,       0,      ARM_MPU_REGION_SIZE_512KB,    ARM_MPU_AP_RO,  1,      0},   // ap region 0x400000-0x480000, do not protect upbuffer
//    {(UINT32  *)0x500000,              0,      ARM_MPU_REGION_SIZE_16KB,    ARM_MPU_AP_RO,  1,      0},   // cp code 0x500000-0x504000

};


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


void SetFullEnvForCPProcess(void)
{

    UINT32 *src;
    UINT32 *dst;
    UINT32 length;

#if defined(__CC_ARM)
    uint8_t cpBootFlag = cpmuGetBootFlag();

    if(AonRegGetCPSlpInPaging() == true)    // last time sleep in paging, so all fullimage code need copy
    {
        cpBootFlag = CP_BOOT_FROM_CO;
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
        break;

        default:
            EC_ASSERT(0, cpBootFlag, 0, 0);
    }


    DisableICache();                    // flush cache when ramcode update
    EnableICache();

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

    UINT32 *start_addr;
    UINT32 *end_addr  ;
    UINT32* stack_len = &(Stack_Size);
    start_addr = &(Image$$LOAD_CPDRAM_SHARED$$ZI$$Base) ;
    end_addr   = &(Image$$LOAD_CPDRAM_SHARED$$ZI$$Limit);
    length = (UINT32)end_addr - (UINT32)start_addr;
    __fast_memset((UINT32 *)start_addr, 0, length-(UINT32)stack_len);

    CopyDataRWZIForPhyProcess();

#endif
}


void CopyRamCodeForPhyProcess(void)
{
    UINT32 *src;
    UINT32 *dst;
    UINT32 length;

#if defined(__CC_ARM)

    uint8_t cpBootFlag = cpmuGetBootFlag();

    switch(cpBootFlag)
    {
        case CP_BOOT_FROM_POWER_ON:
        case CP_BOOT_FROM_CO:
        case CP_BOOT_FROM_CH:
        {
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

    DisableICache();        // flush cache when ramcode update
    EnableICache();

#endif

}


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


