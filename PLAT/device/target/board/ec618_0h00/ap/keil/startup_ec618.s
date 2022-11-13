;/**************************************************************************//**
; * @file     startup_ARMCM3.s
; * @brief    CMSIS Core Device Startup File for
; *           ARMCM3 Device Series
; * @version  V5.00
; * @date     02. March 2016
; ******************************************************************************/
;/*
; * Copyright (c) 2009-2016 ARM Limited. All rights reserved.
; *
; * SPDX-License-Identifier: Apache-2.0
; *
; * Licensed under the Apache License, Version 2.0 (the License); you may
; * not use this file except in compliance with the License.
; * You may obtain a copy of the License at
; *
; * www.apache.org/licenses/LICENSE-2.0
; *
; * Unless required by applicable law or agreed to in writing, software
; * distributed under the License is distributed on an AS IS BASIS, WITHOUT
; * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
; * See the License for the specific language governing permissions and
; * limitations under the License.
; */

;/*
;//-------- <<< Use Configuration Wizard in Context Menu >>> ------------------
;*/


; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Stack_Size      EQU     0x00000800

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp

                EXPORT Stack_Size
; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

;Heap_Size       EQU     0x00000C00
;
;                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
;__heap_base
;Heap_Mem        SPACE   Heap_Size
;__heap_limit


                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset

                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

__Vectors       DCD     __initial_sp              ; Top of Stack
                DCD     Reset_Handler             ; Reset Handler
                DCD     NMI_Handler               ; NMI Handler
                DCD     HardFault_Handler         ; Hard Fault Handler
                DCD     MemManage_Handler         ; MPU Fault Handler
                DCD     BusFault_Handler          ; Bus Fault Handler
                DCD     UsageFault_Handler        ; Usage Fault Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     SVC_Handler               ; SVCall Handler
                DCD     DebugMon_Handler          ; Debug Monitor Handler
                DCD     0                         ; Reserved
                DCD     PendSV_Handler            ; PendSV Handler
                DCD     SysTick_Handler           ; SysTick Handler

                ; External Interrupts
                DCD     Pad0_WakeupIntHandler     ;  0:  Pad0 Wakeup
                DCD     Pad1_WakeupIntHandler     ;  1:  Pad1 Wakeup
                DCD     Pad2_WakeupIntHandler     ;  2:  Pad2 Wakeup
                DCD     Pad3_WakeupIntHandler     ;  3:  Pad3 Wakeup
                DCD     Pad4_WakeupIntHandler     ;  4:  Pad4 Wakeup
                DCD     Pad5_WakeupIntHandler     ;  5:  Pad5 Wakeup
                DCD     Lpuart_WakeupIntHandler   ;  6:  LPUART Wakeup
                DCD     LPUSB_WakeupIntHandler    ;  7:  LPUSB Wakeup
                DCD     PwrKey_WakeupIntHandler   ;  8:  PwrKey Wakeup
                DCD     ChrgPad_WakeupIntHandler  ;  9:  ChrgPad Wakeup
                DCD     RTC_WakeupIntHandler      ;  10: RTC Wakeup
                DCD     USB_IRQ_Handler           ;  11: USB Irq
                DCD     XIC_IntHandler            ;  12:
                DCD     XIC_IntHandler            ;  13
                DCD     XIC_IntHandler            ;  14:
		DCD     XIC_IntHandler            ;  15:

__Vectors_End

__Vectors_Size  EQU     __Vectors_End - __Vectors

                AREA    |.text|, CODE, READONLY


; Reset Handler

Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
                IMPORT  SystemInit
                IMPORT  ec_main
                LDR     R0, =0x4d020190 ; Not remapping
                LDR     R1, =0x00000000
                STR     R1, [R0]

                LDR     R0, =SystemInit
                BLX     R0
                LDR     R0, =ec_main
                BX      R0
                ENDP


; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler     PROC
                EXPORT  NMI_Handler               [WEAK]
                B       .
                ENDP

HardFault_Handler\
                PROC
                IMPORT  excepHardFaultHandler
                IMPORT  excepInfoStore
                EXPORT  HardFault_Handler         [WEAK]

                mrs r1, PRIMASK         ;;backup PRIMASK
                mov r0, #1
                msr PRIMASK, r0
                				
                ldr r12, =excepInfoStore
                ldr r0, =0xf2f0f1f3
                stmia r12!, {r0}
                ldr r0, =0xe2e0e1e3
                stmia r12!, {r0}
                add r12, r12, #8
                stmia r12!, {r0-r11}

                add r12, r12, #20
                mov r0, lr              ;;store exc_return
                stmia r12!, {r0}

                mrs r0, msp             ;;store msp
                stmia r12!, {r0}
                mrs r0, psp             ;;store psp
                stmia r12!, {r0}

                mrs r0, CONTROL
                stmia r12!, {r0}

                mrs r0, BASEPRI
                stmia r12!, {r0}
                mov r0, r1              ;;restore PRIMASK
                stmia r12!, {r0}
                mrs r0, FAULTMASK
                stmia r12!, {r0}

                mov r0, sp
                mrs r1, psp
                mov r2, lr
                B       excepHardFaultHandler
                ENDP
               
MemManage_Handler\
                PROC
                EXPORT  MemManage_Handler                    [WEAK]
                BL Default_Handler                
                B       .
                ENDP	
 
BusFault_Handler\
                PROC
                EXPORT  BusFault_Handler                     [WEAK]
                BL Default_Handler                
                B       .
                ENDP
UsageFault_Handler\
                PROC
                EXPORT  UsageFault_Handler                   [WEAK]
                BL Default_Handler                
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler                          [WEAK]
                B       .
                ENDP
DebugMon_Handler\
                PROC
                EXPORT  DebugMon_Handler                     [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler                       [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler                      [WEAK]
                B       .
                ENDP
Pad0_WakeupIntHandler   PROC
                EXPORT  Pad0_WakeupIntHandler             [WEAK]
                B       .
                ENDP
Pad1_WakeupIntHandler   PROC
                EXPORT  Pad1_WakeupIntHandler             [WEAK]
                B       .
                ENDP
Pad2_WakeupIntHandler   PROC
                EXPORT  Pad2_WakeupIntHandler             [WEAK]
                B       .
                ENDP
Pad3_WakeupIntHandler   PROC
                EXPORT  Pad3_WakeupIntHandler             [WEAK]
                B       .
                ENDP
Pad4_WakeupIntHandler   PROC
                EXPORT  Pad4_WakeupIntHandler             [WEAK]
                B       .
                ENDP
Pad5_WakeupIntHandler   PROC
                EXPORT  Pad5_WakeupIntHandler             [WEAK]
                B       .
                ENDP
Lpuart_WakeupIntHandler     PROC
                EXPORT  Lpuart_WakeupIntHandler           [WEAK]
                B       .
                ENDP
LPUSB_WakeupIntHandler  PROC
                EXPORT  LPUSB_WakeupIntHandler            [WEAK]
                B       .
                ENDP
PwrKey_WakeupIntHandler PROC
                EXPORT  PwrKey_WakeupIntHandler           [WEAK]
                B       .
                ENDP
ChrgPad_WakeupIntHandler PROC
                EXPORT  ChrgPad_WakeupIntHandler          [WEAK]
                B       .
                ENDP
RTC_WakeupIntHandler    PROC
                EXPORT  RTC_WakeupIntHandler              [WEAK]
                B       .
                ENDP
USB_IRQ_Handler    PROC
                EXPORT  USB_IRQ_Handler              [WEAK]
                B       .
                ENDP
XIC_IntHandler  PROC
                EXPORT  XIC_IntHandler                    [WEAK]
                B       .
                ENDP

Default_Handler PROC
                B       .
                ENDP
__clr_240K_mem  PROC
		MOVS	R0, #0
		STM	R6!,{R0}
		ADDS	R5, R5, #1
		CMP	R5, #0xEC00
		BCC	__clr_240K_mem
		LDR     R0, =0x08080000
		LDR	SP, [R0,#0]
		MOVS	LR, R7
		BX      LR

		ENDP


                ALIGN


; User Initial Stack & Heap

;                IF      :DEF:__MICROLIB

                EXPORT  __initial_sp
;                EXPORT  __heap_base
;                EXPORT  __heap_limit

;                ELSE

;                IMPORT  __use_two_region_memory
;                EXPORT  __user_initial_stackheap

;__user_initial_stackheap PROC
;                LDR     R0, =  Heap_Mem
;                LDR     R1, =(Stack_Mem + Stack_Size)
;                LDR     R2, = (Heap_Mem +  Heap_Size)
;                LDR     R3, = Stack_Mem
;                BX      LR
;                ENDP

;                ALIGN

;                ENDIF


                END
