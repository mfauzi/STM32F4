; ******************************************************************************
; * <h2><center>&copy; COPYRIGHT 2012 Embest Tech. Co., Ltd.</center></h2>
; * @file    cpu_a.asm
; * @author  CMP Team
; * @version V1.0.0
; * @date    28-December-2012
; * @brief   cpu configuration and port layer file.                        
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
        PUBLIC  CPU_IntDis
        PUBLIC  CPU_IntEn

        PUBLIC  CPU_SR_Save
        PUBLIC  CPU_SR_Restore

        PUBLIC  CPU_CntLeadZeros
        PUBLIC  CPU_RevBits

        PUBLIC  CPU_WaitForInt
        PUBLIC  CPU_WaitForExcept


;code generation directives-----------------------------------------------------

        RSEG CODE:CODE:NOROOT(2)


;$PAGE
;*******************************************************************************
;                                    DISABLE and ENABLE INTERRUPTS
;
; Description : Disable/Enable interrupts.
;
; Prototypes  : void  CPU_IntDis(void);
;               void  CPU_IntEn (void);
;*******************************************************************************
CPU_IntDis
        CPSID   I
        BX      LR


CPU_IntEn
        CPSIE   I
        BX      LR


;*******************************************************************************
;                           CRITICAL SECTION FUNCTIONS
;
; Description : Disable/Enable interrupts by preserving the state of interrupts. 
;               Generally speaking, the state of the interrupt disable flag is 
;               stored in the local variable 'cpu_sr' & interrupts are then disabled 
;               ('cpu_sr' is allocated in all functions that need to disable interrupts).
;               The previous interrupt state is restored by copying 'cpu_sr' into 
;               the CPU's status register.
;
; Prototypes  : CPU_SR  CPU_SR_Save   (void);
;               void    CPU_SR_Restore(CPU_SR cpu_sr);
;
;
; Note(s)     : (1) These functions are used in general like this:
;
;                   void  Task (void *p_arg)
;                   {
;                                                               /* Allocate storage for CPU status register */
;                   #if (CPU_CFG_CRITICAL_METHOD == CPU_CRITICAL_METHOD_STATUS_LOCAL)
;                       CPU_SR  cpu_sr;
;                   #endif
;
;                            :
;                            :
;                       CPU_CRITICAL_ENTER();                   /* cpu_sr = CPU_SR_Save();                  */
;                            :
;                            :
;                       CPU_CRITICAL_EXIT();                    /* CPU_SR_Restore(cpu_sr);                  */
;                            :
;                            :
;                   }
;*******************************************************************************
CPU_SR_Save
        MRS     R0, PRIMASK                     ; Set prio int mask to mask all (except faults)
        CPSID   I
        BX      LR


CPU_SR_Restore                                  ; See Note #2.
        MSR     PRIMASK, R0
        BX      LR


;$PAGE
;*******************************************************************************
;                               COUNT LEADING ZEROS
;
; Description : Counts the number of binary zero bits before the first binary one bit
;               in the argument. If the argument is zero, the value 32 is returned.
;
; Prototypes : CPU_INT32U  CPU_CntLeadZeros (CPU_INT32U  val)
;
; Argument(s) : val     variable to count leading zeros
;*******************************************************************************

CPU_CntLeadZeros:
        CLZ     R0, R0                          ; Count leading zeros
        BX      LR


;*******************************************************************************
;                                   REVERSE BITS
;
; Description : Reverses the bits in the argument.
;
; Prototypes : CPU_INT32U  CPU_RevBits (CPU_INT32U  val)
;
; Argument(s) : val     variable to reverse
;*******************************************************************************

CPU_RevBits:
        RBIT    R0, R0                          ; Reverse bits
        BX      LR


;$PAGE
;*******************************************************************************
;                                 WAIT FOR INTERRUPT
;
; Description : Enters sleep state, which will be exited when an interrupt is received.
;
; Prototypes  : void  CPU_WaitForInt (void)
;
; Argument(s) : none.
;*******************************************************************************

CPU_WaitForInt:
        WFI                                     ; Wait for interrupt
        BX      LR


;*******************************************************************************
;                                WAIT FOR EXCEPTION
;
; Description : Enters sleep state, which will be exited when an exception is received.
;
; Prototypes  : void  CPU_WaitForExcept (void)
;
; Argument(s) : none.
;*******************************************************************************

CPU_WaitForExcept:
        WFE                                     ; Wait for exception
        BX      LR


;*******************************************************************************
;                            CPU ASSEMBLY PORT FILE END
;*******************************************************************************

        END

;******************** COPYRIGHT 2012 Embest Tech. Co., Ltd.*****END OF FILE*****
