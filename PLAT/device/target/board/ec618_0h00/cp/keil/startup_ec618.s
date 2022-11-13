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

Stack_Size      EQU     0x00001000

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
                DCD     CP_SWWakeup_IntHandler	  ;  0:  ap sw
                DCD     CP_RTWakeup_IntHandler    ;  1:  tmu
                DCD     CP_BCWakeup_IntHandler    ;  2:  tmu
                DCD     CP_IPC0Wakeup_IntHandler  ;  3:  ipc
                DCD     CP_IPC1Wakeup_IntHandler  ;  4:  ipc
                DCD     CP_IPCMWakeup_IntHandler  ;  5:  ipc
                DCD     CP_SIPCWakeup_IntHandler  ;  6:  sipc
                DCD     CP_APDAPWakeup_IntHandler ;  7:  ap dap
                DCD     CP_CPDAPWakeup_IntHandler ;  8:  cp dap
                DCD     0                         ;  9:  Reserved
                DCD     0                         ;  10: Reserved
                DCD     CP_CPUSBIntHandler        ;  11: Reserved
                DCD     XIC_IntHandler            ;  12:  
                DCD     XIC_IntHandler            ;  13: 

__Vectors_End

__Vectors_Size  EQU     __Vectors_End - __Vectors

                AREA    |.text|, CODE, READONLY


; Reset Handler

Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
                IMPORT  SystemInit
                IMPORT  cp_main
                IMPORT  bootStack
				
                LDR 	R0,=bootStack
                ADDS    R0,R0,#512
                MSR   	MSP,R0
				
                LDR     R0, =SystemInit
                BLX     R0
                LDR     R0, =cp_main
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
CP_SWWakeup_IntHandler 	PROC
                EXPORT  CP_SWWakeup_IntHandler               [WEAK]
                B       .
                ENDP
CP_RTWakeup_IntHandler 	PROC
                EXPORT  CP_RTWakeup_IntHandler               [WEAK]
                B       .
                ENDP
CP_BCWakeup_IntHandler 	PROC
				EXPORT  CP_BCWakeup_IntHandler               [WEAK]
				B       .
				ENDP
CP_IPC0Wakeup_IntHandler 	PROC
				EXPORT  CP_IPC0Wakeup_IntHandler             [WEAK]
				B       .
				ENDP
CP_IPC1Wakeup_IntHandler 	PROC
				EXPORT  CP_IPC1Wakeup_IntHandler             [WEAK]
				B       .
				ENDP
CP_IPCMWakeup_IntHandler 	PROC
				EXPORT  CP_IPCMWakeup_IntHandler             [WEAK]
				B       .
				ENDP
CP_SIPCWakeup_IntHandler 	PROC
				EXPORT  CP_SIPCWakeup_IntHandler             [WEAK]
				B       .
				ENDP
CP_APDAPWakeup_IntHandler 	PROC
				EXPORT  CP_APDAPWakeup_IntHandler            [WEAK]
				B       .
				ENDP
CP_CPDAPWakeup_IntHandler 	PROC
				EXPORT  CP_CPDAPWakeup_IntHandler            [WEAK]
				B       .
				ENDP
CP_CPUSBIntHandler 	        PROC
				EXPORT  CP_CPUSBIntHandler                   [WEAK]
				B       .
				ENDP
XIC_IntHandler              PROC
				EXPORT  XIC_IntHandler                       [WEAK]
				B       .
				ENDP
Default_Handler             PROC
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
