			AREA    |.text|, CODE, READONLY
			IMPORT pxCurrentTCB
			IMPORT uxCriticalNesting
			IMPORT vTaskSwitchContext
vPortSVCHandler	PROC
                EXPORT  vPortSVCHandler
				ldr	r3, =pxCurrentTCB
				ldr r1, [r3]
				ldr r0, [r1]
				ldmia r0!, {r4-r11}
				msr psp, r0
				isb
				mov r0, #0
				msr	basepri, r0
				orr r14, #0xd
				bx r14
                ENDP
				
xPortPendSVHandler PROC
				EXPORT  xPortPendSVHandler
				mrs r0, psp
				isb

				ldr	r3, =pxCurrentTCB
				ldr	r2, [r3]

				stmdb r0!, {r4-r11}
				str r0, [r2]

				stmdb sp!, {r3, r14}
				mov r0, #16
				msr basepri, r0
				dsb
				isb
				bl vTaskSwitchContext
				mov r0, #0
				msr basepri, r0
				ldmia sp!, {r3, r14}

				ldr r1, [r3]
				ldr r0, [r1]		
				ldmia r0!, {r4-r11}	
				msr psp, r0
				isb
				bx r14
				nop
				ENDP
			
			END

				