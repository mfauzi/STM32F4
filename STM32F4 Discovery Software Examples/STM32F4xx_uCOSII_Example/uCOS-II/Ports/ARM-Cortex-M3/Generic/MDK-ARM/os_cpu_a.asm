; ******************************************************************************
; * <h2><center>&copy; COPYRIGHT 2012 Embest Tech. Co., Ltd.</center></h2>
; * @file    os_cpu_a.asm
; * @author  CMP Team
; * @version V1.0.0
; * @date    28-December-2012
; * @brief   arm cortex-m4 port.                        
; ******************************************************************************
; * @attention
; *
; * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
; * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
; * TIME. AS A RESULT, Embest SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
; * OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
; * OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
; * CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
; ******************************************************************************

;Exported functions ------------------------------------------------------------

    EXTERN  OSRunning                      ; External references
    EXTERN  OSPrioCur
    EXTERN  OSPrioHighRdy
    EXTERN  OSTCBCur
    EXTERN  OSTCBHighRdy
    EXTERN  OSIntNesting
    EXTERN  OSIntExit
    EXTERN  OSTaskSwHook
    EXTERN  OS_TCB_OPT_OFFSET

    EXPORT  OS_CPU_SR_Save                 ; Functions declared in this file
    EXPORT  OS_CPU_SR_Restore
    EXPORT  OSStartHighRdy
    EXPORT  OSCtxSw
    EXPORT  OSIntCtxSw
    EXPORT  OS_CPU_PendSVHandler

;equates------------------------------------------------------------------------

NVIC_INT_CTRL   EQU     0xE000ED04         ; Interrupt control state register.
NVIC_SYSPRI14   EQU     0xE000ED22         ; System priority register (priority 14).
NVIC_PENDSV_PRI EQU     0x00000000         ; PendSV priority value (lowest).
NVIC_PENDSVSET  EQU     0x10000000         ; Value to trigger PendSV exception.

;code generation directives-----------------------------------------------------

    AREA |.text|, CODE, READONLY, ALIGN=3
    THUMB
    REQUIRE8
    PRESERVE8

;*******************************************************************************
;                                   CRITICAL SECTION METHOD 3 FUNCTIONS
;
; Description: Disable/Enable interrupts by preserving the state of interrupts.  Generally speaking you
;              would store the state of the interrupt disable flag in the local variable 'cpu_sr' and then
;              disable interrupts.  'cpu_sr' is allocated in all of uC/OS-II's functions that need to
;              disable interrupts.  You would restore the interrupt disable state by copying back 'cpu_sr'
;              into the CPU's status register.
;
; Prototypes :     OS_CPU_SR  OS_CPU_SR_Save(void);
;                  void       OS_CPU_SR_Restore(OS_CPU_SR cpu_sr);
;
;
; Note(s)    : 1) These functions are used in general like this:
;
;                 void Task (void *p_arg)
;                 {
;                 #if OS_CRITICAL_METHOD == 3          /* Allocate storage for CPU status register */
;                     OS_CPU_SR  cpu_sr;
;                 #endif
;
;                          :
;                          :
;                     OS_ENTER_CRITICAL();             /* cpu_sr = OS_CPU_SaveSR();                */
;                          :
;                          :
;                     OS_EXIT_CRITICAL();              /* OS_CPU_RestoreSR(cpu_sr);                */
;                          :
;                          :
;                 }
;*******************************************************************************

OS_CPU_SR_Save
    MRS     R0, PRIMASK                ; Set prio int mask to mask all (except faults)
    CPSID   I
    BX      LR

OS_CPU_SR_Restore
    MSR     PRIMASK, R0
    BX      LR

;*******************************************************************************
;                                          START MULTITASKING
;                                       void OSStartHighRdy(void)
;
; Note(s) : 1) This function triggers a PendSV exception (essentially, causes a context switch) to cause
;              the first task to start.
;
;           2) OSStartHighRdy() MUST:
;              a) Setup PendSV exception priority to lowest;
;              b) Set initial PSP to 0, to tell context switcher this is first run;
;              c) Set OSRunning to TRUE;
;              d) Trigger PendSV exception;
;              e) Enable interrupts (tasks will run with interrupts enabled).
;*******************************************************************************

OSStartHighRdy
    LDR     R0, =NVIC_SYSPRI14         ; Set the PendSV exception priority
    LDR     R1, =NVIC_PENDSV_PRI
    STRB    R1, [R0]

    MOVS    R0, #0                     ; Set the PSP to 0 for initial context switch call
    MSR     PSP, R0

    LDR     R0, =OSRunning             ; OSRunning = TRUE
    MOVS    R1, #1
    STRB    R1, [R0]

    LDR     R0, =NVIC_INT_CTRL         ; Trigger the PendSV exception (causes context switch)
    LDR     R1, =NVIC_PENDSVSET
    STR     R1, [R0]

    CPSIE   I                          ; Enable interrupts at processor level

OSStartHang
    B       OSStartHang                ; Should never get here


;*******************************************************************************
;                               PERFORM A CONTEXT SWITCH (From task level)
;                                           void OSCtxSw(void)
;
; Note(s) : 1) OSCtxSw() is called when OS wants to perform a task context switch.  This function
;              triggers the PendSV exception which is where the real work is done.
;*******************************************************************************

OSCtxSw
    LDR     R0, =NVIC_INT_CTRL         ; Trigger the PendSV exception (causes context switch)
    LDR     R1, =NVIC_PENDSVSET
    STR     R1, [R0]
    BX      LR

;*******************************************************************************
;                             PERFORM A CONTEXT SWITCH (From interrupt level)
;                                         void OSIntCtxSw(void)
;
; Notes:    1) OSIntCtxSw() is called by OSIntExit() when it determines a context switch is needed as
;              the result of an interrupt.  This function simply triggers a PendSV exception which will
;              be handled when there are no more interrupts active and interrupts are enabled.
;*******************************************************************************

OSIntCtxSw
    LDR     R0, =NVIC_INT_CTRL         ; Trigger the PendSV exception (causes context switch)
    LDR     R1, =NVIC_PENDSVSET
    STR     R1, [R0]
    BX      LR

;*******************************************************************************
;                              HANDLE PendSV EXCEPTION
;                          void OS_CPU_PendSVHandler(void)
;
; Note(s) : 1) PendSV is used to cause a context switch.  This is a recommended method for performing
;              context switches with Cortex-M4.  This is because the Cortex-M4 auto-saves half of the
;              processor context on any exception, and restores same on return from exception.  So only
;              saving of R4-R11 is required and fixing up the stack pointers.  Using the PendSV exception
;              this way means that context saving and restoring is identical whether it is initiated from
;              a thread or occurs due to an interrupt or exception.
;
;           2) Pseudo-code is:
;              a) Get the process SP, if 0 then skip (goto d) the saving part (first context switch);
;              b) Save remaining regs r4-r11 on process stack;
;                 b+) If FPU is used then save remaining float regs s16-s31 on process stack
;              c) Save the process SP in its TCB, OSTCBCur->OSTCBStkPtr = SP;
;              d) Call OSTaskSwHook();
;              e) Get current high priority, OSPrioCur = OSPrioHighRdy;
;              f) Get current ready thread TCB, OSTCBCur = OSTCBHighRdy;
;              g) Get new process SP from TCB, SP = OSTCBHighRdy->OSTCBStkPtr;
;              h) Restore R4-R11 from new process stack;
;                 h+) If FPU is used then restore S16-S31 from new process stack
;              i) Perform exception return which will restore remaining context.
;
;           3) On entry into PendSV handler:
;              a) The following have been saved on the process stack (by processor):
;                 xPSR, PC, LR, R12, R0-R3
;              b) Processor mode is switched to Handler mode (from Thread mode)
;              c) Stack is Main stack (switched from Process stack)
;              d) OSTCBCur      points to the OS_TCB of the task to suspend
;                 OSTCBHighRdy  points to the OS_TCB of the task to resume
;
;           4) Since PendSV is set to lowest priority in the system (by OSStartHighRdy() above), we
;              know that it will only be run when no other exception or interrupt is active, and
;              therefore safe to assume that context being switched out was using the process stack (PSP).
;*******************************************************************************

OS_CPU_PendSVHandler
    CPSID   I                       ; Prevent interruption during context switch
    MRS     R0, PSP                 ; PSP is process stack pointer
    CBZ     R0, OS_CPU_PendSVHandler_nosave ; Skip register save the first time

    STMFD   R0!, {R4-R11}           ; Save remaining regs r4-11 on process stack
    
    IF      {FPU} != "SoftVFP"
    LSRS    R1, LR, #0x05
    BCS	    PendSV_nosavefpu        ; If FPU is not used then skip loading FPU registers
    ORR     LR, LR, #0x10           ; Set exception return not uses floating-point
    VSTMFD  R0!, {D8-D15}           ; save FPU regs d8-d15(s16-s31) on process 
    ENDIF

PendSV_nosavefpu
    
    LDR     R1, =OSTCBCur           ; OSTCBCur->OSTCBStkPtr = SP;
    LDR     R1, [R1]
    STR     R0, [R1]                ; R0 is SP of process being switched out

                                    ; At this point, entire context of process has been saved
OS_CPU_PendSVHandler_nosave


    PUSH    {R14}                   ; Save LR exc_return value
    LDR     R0, =OSTaskSwHook       ; OSTaskSwHook();
    BLX     R0
    POP     {R14}

    LDR     R0, =OSPrioCur          ; OSPrioCur = OSPrioHighRdy;
    LDR     R1, =OSPrioHighRdy
    LDRB    R2, [R1]
    STRB    R2, [R0]

    LDR     R4, =OSTCBCur           ; OSTCBCur  = OSTCBHighRdy;
    LDR     R6, =OSTCBHighRdy
    LDR     R6, [R6]
    STR     R6, [R4]

    LDR     R0, [R6]                ; R0 is new process SP; SP = OSTCBHighRdy->OSTCBStkPtr; 
    
    IF      {FPU} != "SoftVFP"					
    ADD     R6, R6, #0x10           ;get opt to see if FPU is enable
    LDR     R7, [R6]
    LSRS    R7, R7, #0x03
    BCC     PendSV_norestorefpu	    ; If FPU is not used then skip loading FPU registers
    VLDMFD  R0!, {D8-D15}           ; restore FPU regs d8-d15(s16-s31) from new process stack
    BIC.W   LR, LR, #0x10           ; Set exception return uses floating-point 
    ENDIF

PendSV_norestorefpu                                         

    LDMFD   R0!, {R4-R11}           ; Restore r4-11 from new process stack
    MSR     PSP, R0                 ; Load PSP with new process SP
    ORR     LR, LR, #0x04           ; Ensure exception return uses process stack
    CPSIE   I
    BX      LR                      ; Exception return will restore remaining context

    END
;******************** COPYRIGHT 2012 Embest Tech. Co., Ltd.*****END OF FILE*****