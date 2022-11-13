	.syntax	unified
	.arch armv7-m
	.cpu cortex-m3

.global	__isr_vector
.global	Reset_Handler

.global	__StackTop
.global	__StackLimit

	.section .stack
	.align	3
#ifdef __STACK_SIZE
	.equ	Stack_Size, __STACK_SIZE
#else
	.equ	Stack_Size, 0x1000
#endif
__StackLimit:
	.space	Stack_Size
	.size	__StackLimit, . - __StackLimit
__StackTop:
	.size	__StackTop, . - __StackTop

	.section .isr_vector,"a",%progbits
	.align	2
	.type  __isr_vector, %object
	.size __isr_vector, .- __isr_vector
__isr_vector:
	.long	__StackTop			  /* Top of Stack */
	.long	Reset_Handler		  /* Reset Handler */
	.long	NMI_Handler			  /* NMI Handler */
	.long	HardFault_Handler	  /* Hard Fault Handler */
	.long	MemManage_Handler	  /* MPU Fault Handler */
	.long	BusFault_Handler	  /* Bus Fault Handler */
	.long	UsageFault_Handler	  /* Usage Fault Handler */
	.long	0					  /* Reserved */
	.long	0					  /* Reserved */
	.long	0					  /* Reserved */
	.long	0					  /* Reserved */
	.long	SVC_Handler			  /* SVCall Handler */
	.long	DebugMon_Handler	  /* Debug Monitor Handler */
	.long	0					  /* Reserved */
	.long	PendSV_Handler		  /* PendSV Handler */
	.long	SysTick_Handler		  /* SysTick Handler */
	
	/* External interrupts */
	.long	CP_SWWakeup_IntHandler		/* 0:  ap sw  */
	.long	CP_RTWakeup_IntHandler		/* 1:  tmu */
	.long	CP_BCWakeup_IntHandler		/* 2:  tmu */
	.long	CP_IPC0Wakeup_IntHandler	/* 3:  ipc */
	.long	CP_IPC1Wakeup_IntHandler	/* 4:  ipc */
	.long	CP_IPCMWakeup_IntHandler	/* 5:  ipc */
	.long	CP_SIPCWakeup_IntHandler	/* 6:  sipc */
	.long	CP_APDAPWakeup_IntHandler	/* 7:  ap dap */
	.long	CP_CPDAPWakeup_IntHandler	/* 8:  cp dap */
	.long	0							/* 9: Reserved */
	.long	0							/* 10: Reserved */
	.long	CP_CPUSBIntHandler			/* 11: */
	.long	XIC_IntHandler				/* 12:	*/
	.long	XIC_IntHandler				/* 13:	*/


	.text
	.thumb
	.thumb_func
	.align	2
	.type	Reset_Handler, %function
Reset_Handler:

	.extern SystemInit
	.extern cp_main
	.extern bootStack

	ldr r0, =bootStack
	adds r0, r0, #512
	msr msp, r0

	ldr	 r0, =0x4D000008      /* CP DFC vote false */
	ldr	 r1, =0x00000000
	str	 r1, [r0]
	
	ldr r0, =SystemInit
	blx	r0
	ldr r0, =cp_main
	bx	r0
	.size  Reset_Handler, .-Reset_Handler

	.thumb
	.thumb_func
	.align	2
	.type	HardFault_Handler, %function
HardFault_Handler:

	.extern  excepHardFaultHandler
	.extern  excepInfoStore

	mrs r1, PRIMASK		/* backup PRIMASK */
	mov r0, #1
	msr PRIMASK, r0

	ldr r12, =excepInfoStore
	add r12, r12, #12
	stmia r12!, {r0-r11}

	add r12, r12, #20
	mov r0, lr		/* store exc_return */
	stmia r12!, {r0}

	mrs r0, msp		/* store msp */
	stmia r12!, {r0}
	mrs r0, psp		/* store psp */
	stmia r12!, {r0}

	mrs r0, CONTROL
	stmia r12!, {r0}

	mrs r0, BASEPRI
	stmia r12!, {r0}
	mov r0, r1		/* restore PRIMASK */
	stmia r12!, {r0}
	mrs r0, FAULTMASK
	stmia r12!, {r0}

	mov r0, sp
	mrs r1, psp
	mov r2, lr
	B	excepHardFaultHandler
	.size  HardFault_Handler, .-HardFault_Handler
	
	.align	1
	.thumb
	.thumb_func
	.weak	Default_Handler
	.type	Default_Handler, %function
Default_Handler:
	b	.
	.size  Default_Handler, .-Default_Handler
	
	.macro	def_irq_handler	handler_name
	.weak	\handler_name
	.thumb_set	\handler_name, Default_Handler
	.endm

	def_irq_handler	NMI_Handler
	def_irq_handler	MemManage_Handler
	def_irq_handler	BusFault_Handler
	def_irq_handler	UsageFault_Handler
	def_irq_handler	SVC_Handler
	def_irq_handler	DebugMon_Handler
	def_irq_handler	PendSV_Handler
	def_irq_handler	SysTick_Handler
	def_irq_handler	CP_SWWakeup_IntHandler
	def_irq_handler	CP_RTWakeup_IntHandler
	def_irq_handler	CP_BCWakeup_IntHandler
	def_irq_handler	CP_IPC0Wakeup_IntHandler
	def_irq_handler	CP_IPC1Wakeup_IntHandler
	def_irq_handler	CP_IPCMWakeup_IntHandler
	def_irq_handler	CP_SIPCWakeup_IntHandler
	def_irq_handler	CP_APDAPWakeup_IntHandler
	def_irq_handler	CP_CPDAPWakeup_IntHandler
	def_irq_handler	CP_CPUSBIntHandler
	def_irq_handler	XIC_IntHandler

	.end
