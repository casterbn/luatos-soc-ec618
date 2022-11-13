
#include "mem_map.h"

/* Entry Point */
ENTRY(Reset_Handler)

/* Specify the memory areas */
MEMORY
{
  CSMB_AREA(rwx)              : ORIGIN = 0x00000000, LENGTH = 0x010000      /* 64KB */
  MSMB_AREA(rwx)              : ORIGIN = 0x00500000, LENGTH = 0x40000      /* 0.25MB */
  FLASH_AREA(rx)              : ORIGIN = 0x00800000, LENGTH = 512K         /* 512KB */
}

/* Define output sections */
SECTIONS
{
  . = CP_FLASH_XIP_ADDR;
  .vector :
  {
    KEEP(*(.isr_vector))
  } >FLASH_AREA
  .cache : ALIGN(128)
  {
    Image$$UNLOAD_NOCACHE$$Base = .;
    *cache.o(.text*) 
  } >FLASH_AREA
  
  _etext = .;        /* define a global symbols at end of code */

  .load_cp_piram_csmb 0x20 : ALIGN(4)
  {
   . = ALIGN(4);
    Load$$LOAD_CP_PIRAM_CSMB$$Base = LOADADDR(.load_cp_piram_csmb);
    Image$$LOAD_CP_PIRAM_CSMB$$Base = .;
    *(.phyCodeCsmb_ICS)
    *(.phyCodeCsmb_UL)
    *(.phyCodeCsmb_RXDFE)
    *(.phyCodeCsmb_CE)
    *(.phyCodeCsmb_AXC)
    *(.phyCodeCsmb_RF)
    *(.phyCodeCsmb_SCHD)
    *(.phyCodeCsmb_MACSF)
    *(.phyCodeCsmb_MEAS)
    *(.phyCodeCsmb_PMU)
    *(.phyCodeCsmb_CCH)
    *(.phyCodeCsmb_CSI)
    *(.phyCodeCsmb_DCH)
    *(.phyCodeCsmb_DEC)
    *(.phyCodeCsmb_HARQ)
    *(.phyCodeCsmb_TMU)
    *(.phyCodeCsmb_COMN)
    *(.cpPlatCodeCsmb)
    *(.phyDataCsmb_Const)
    *memset.o(.text*)
    *memcpy-armv7m.o(.text*)
    *(.recordNodeRO)
    . = ALIGN(4);
  } >CSMB_AREA AT>FLASH_AREA

  Image$$LOAD_CP_PIRAM_CSMB$$Length = SIZEOF(.load_cp_piram_csmb);

  .load_cp_firam_csmb : ALIGN(4)
  {
    . = ALIGN(4);
    Load$$LOAD_CP_FIRAM_CSMB$$Base = LOADADDR(.load_cp_firam_csmb);
    Image$$LOAD_CP_FIRAM_CSMB$$Base = .;
    . = ALIGN(4);
  } >CSMB_AREA AT>FLASH_AREA

  Image$$LOAD_CP_FIRAM_CSMB$$Length = SIZEOF(.load_cp_firam_csmb);

  .unload_cpcsmbdata CSMB_PHY_AONMEM_ADDR-0x1000 (NOLOAD):
  {
    *(.phyDataCsmb_Slp2)
  } >CSMB_AREA

  .unload_cpaonmem CSMB_PHY_AONMEM_ADDR (NOLOAD):
  {
    *(.phyDataCsmb_AonMemBackup)
    *(.phyDataCsmb)
  } >CSMB_AREA

  .load_cp_slp2piram_msmb MSMB_CPMEM_START_ADDR : ALIGN(4)
  {
    . = ALIGN(4);
    Load$$LOAD_CP_SLP2PIRAM_MSMB$$Base = LOADADDR(.load_cp_slp2piram_msmb);
    Image$$LOAD_CP_SLP2PIRAM_MSMB$$Base = .;
    *(.phyCodeMsmb_SLP2)
    . = ALIGN(4);
  } >MSMB_AREA AT>FLASH_AREA

  Image$$LOAD_CP_SLP2PIRAM_MSMB$$Length = SIZEOF(.load_cp_slp2piram_msmb);

  .load_cp_piram_msmb : ALIGN(4)
  {
    . = ALIGN(4);
    Load$$LOAD_CP_PIRAM_MSMB$$Base = LOADADDR(.load_cp_piram_msmb);
    Image$$LOAD_CP_PIRAM_MSMB$$Base = .;
    *(.phyCodeMsmb_HIB)
    . = ALIGN(4);
  } >MSMB_AREA AT>FLASH_AREA

  Image$$LOAD_CP_PIRAM_MSMB$$Length = SIZEOF(.load_cp_piram_msmb);


  .load_cp_firam_msmb : ALIGN(4)
  {
    . = ALIGN(4);
    Load$$LOAD_CP_FIRAM_MSMB$$Base = LOADADDR(.load_cp_firam_msmb);
    Image$$LOAD_CP_FIRAM_MSMB$$Base = .;
    *(.phyFMRamcode)
    *(.phyCodeMsmb_ICS)
    *(.phyCodeMsmb_UL)
    *(.phyCodeMsmb_RXDFE)
    *(.phyCodeMsmb_CE)
    *(.phyCodeMsmb_AXC)
    *(.phyCodeMsmb_RF)
    *(.phyCodeMsmb_SCHD)
    *(.phyCodeMsmb_MACSF)
    *(.phyCodeMsmb_MEAS)
    *(.phyCodeMsmb_PMU)
    *(.phyCodeMsmb_CCH)
    *(.phyCodeMsmb_CSI)
    *(.phyCodeMsmb_DCH)
    *(.phyCodeMsmb_DEC)
    *(.phyCodeMsmb_HARQ)
    *(.phyCodeMsmb_TMU)
    *(.phyCodeMsmb_COMN)
    *(.phyDataMsmb_Const)
    *(.cpPlatCodeMsmb)
    . = ALIGN(4);
  } >MSMB_AREA AT>FLASH_AREA

  Image$$LOAD_CP_FIRAM_MSMB$$Length = SIZEOF(.load_cp_firam_msmb);

  .load_cpos : ALIGN(4)
  {
    . = ALIGN(4);
    Load$$LOAD_CPOS_IRAM$$Base = LOADADDR(.load_cpos);
    Image$$LOAD_CPOS_IRAM$$Base = .;
    *event_groups.o(.text*)
    *heap_6.o(.text*)
    *tlsf.o(.text*)
    *mm_debug.o(.text*)
    *list.o(.text*)
    *queue.o(.text*)
    *tasks.o(.text*)
    *timers.o(.text*)
    *port.o(.text*)
    *port_asm.o(.text*)
    *cmsis_os2.o(.text*)
    . = ALIGN(4);
  } >MSMB_AREA AT>FLASH_AREA

  Image$$LOAD_CPOS_IRAM$$Length = SIZEOF(.load_cpos);

  .unload_slpmem MSMB_CPDATA_START_ADDR (NOLOAD):		/* from 0x500000-0x52C000 is readonly */
  {
    *(.sleepmem)
  } >MSMB_AREA

  .load_cpdram_bsp_data : ALIGN(4)
  {
    . = ALIGN(4);
    Load$$LOAD_CPDRAM_BSP$$Base = LOADADDR(.load_cpdram_bsp_data);
    Image$$LOAD_CPDRAM_BSP$$Base = .;
    *gpr.o(.data*)
    *clock.o(.data*)
    *cpmu.o(.data*)
    *cp_main.o(.data*)
    *slpman.o(.data*)
    *exception_process.o(.data*)
    . = ALIGN(4);
  } >MSMB_AREA AT>FLASH_AREA

    Image$$LOAD_CPDRAM_BSP$$Length = SIZEOF(.load_cpdram_bsp_data);

  .load_cpdram_bsp_zi :
  {
    . = ALIGN(4);
    Image$$LOAD_CPDRAM_BSP$$ZI$$Base = .;
    *gpr.o(.bss.*)
    *clock.o(.bss.*)
    *cpmu.o(.bss.*)
    *cp_main.o(.bss.*)
    *slpman.o(.bss.*)
    *wdt.o(.bss.*)
    *exception_process.o(.bss.*)
    *(.recordNodeZI)
    . = ALIGN(4);
    Image$$LOAD_CPDRAM_BSP$$ZI$$Limit = .;
  } >MSMB_AREA

  .load_cpdram_shared_data : ALIGN(4)
  {
    . = ALIGN(4);
    Load$$LOAD_CPDRAM_SHARED$$Base = LOADADDR(.load_cpdram_shared_data);
    Image$$LOAD_CPDRAM_SHARED$$Base = .;
    *(.phyDataMsmb)
    *(.phyShareDataMsmb)
    *(.data*)
    . = ALIGN(4);
  } >MSMB_AREA AT>FLASH_AREA

  Image$$LOAD_CPDRAM_SHARED$$Length = SIZEOF(.load_cpdram_shared_data);

  .load_cpdram_shared_bss (NOLOAD):
  {
    . = ALIGN(4);
    Image$$LOAD_CPDRAM_SHARED$$ZI$$Base = .;
    *(.cpPlatDataMsmb)
    *(.bss*)
    . = ALIGN(4);
    *(.stack)             /* stack should be 4 byte align */
    Image$$LOAD_CPDRAM_SHARED$$ZI$$Limit = .;
  } >MSMB_AREA

  end_cp_data = . ;

  ASSERT(end_cp_data<CP_AONMEMBACKUP_START_ADDR,"cp use too much ram")

  .text :
  {
    *(.phyCodeFlash_ICS)
    *(.phyCodeFlash_UL)
    *(.phyCodeFlash_RXDFE)
    *(.phyCodeFlash_CE)
    *(.phyCodeFlash_AXC)
    *(.phyCodeFlash_RF)
    *(.phyCodeFlash_ASN1)
    *(.phyCodeFlash_SCHD)
    *(.phyCodeFlash_MACSF)
    *(.phyCodeFlash_MEAS)
    *(.phyCodeFlash_PMU)
    *(.phyCodeFlash_CCH)
    *(.phyCodeFlash_CSI)
    *(.phyCodeFlash_DCH)
    *(.phyCodeFlash_DEC)
    *(.phyCodeFlash_HARQ)
    *(.phyCodeFlash_TMU)
    *(.phyCodeFlash_COMN)
    *(.phyCodeFlash_WIFI)
    *(.cpPlatCodeFlash)
    *(.phyDataFlash_Const)
    *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
    *(.text*)
  } >FLASH_AREA

  .unload_cpaon CP_AONMEMBACKUP_START_ADDR (NOLOAD):
  {
    *(.phyDataMsmb_AonMemBackup)
  } >MSMB_AREA

  .load_xp_sharedinfo XP_SHAREINFO_BASE_ADDR (NOLOAD):
  {
    *(.shareInfo)
  } >MSMB_AREA
  
  .load_dbg_area XP_DBGRESERVED_BASE_ADDR (NOLOAD):
  {
  *(.resetFlag)
  } >MSMB_AREA
  
  .unload_xp_ipcmem IPC_SHAREDMEM_START_ADDR (NOLOAD):
  {

  } >MSMB_AREA
}

GROUP(
    libgcc.a
    libc.a
    libm.a
 )