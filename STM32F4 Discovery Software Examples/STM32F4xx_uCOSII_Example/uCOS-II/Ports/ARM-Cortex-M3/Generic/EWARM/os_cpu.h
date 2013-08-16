/**
  ******************************************************************************
  * <h2><center>&copy; COPYRIGHT 2012 Embest Tech. Co., Ltd.</center></h2>
  * @file    os_cpu.h 
  * @author  CMP Team
  * @version V1.0.0
  * @date    28-December-2012
  * @brief   arm cortex-m4 port.                      
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, Embest SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
  * OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
  * OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
  * CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef  OS_CPU_H
#define  OS_CPU_H


#ifdef   OS_CPU_GLOBALS
#define  OS_CPU_EXT
#else
#define  OS_CPU_EXT  extern
#endif

/* Exported types ------------------------------------------------------------*/

typedef unsigned char  BOOLEAN;
typedef unsigned char  INT8U;                    /* Unsigned  8 bit quantity */
typedef signed   char  INT8S;                    /* Signed    8 bit quantity */
typedef unsigned short INT16U;                   /* Unsigned 16 bit quantity */
typedef signed   short INT16S;                   /* Signed   16 bit quantity */
typedef unsigned int   INT32U;                   /* Unsigned 32 bit quantity */
typedef signed   int   INT32S;                   /* Signed   32 bit quantity */
typedef float          FP32;                     /* Single precision floating point  */
typedef double         FP64;                     /* Double precision floating point */

typedef unsigned int   OS_STK;                   /* Each stack entry is 32-bit wide */
typedef unsigned int   OS_CPU_SR;                /* Define size of CPU status register (PSR = 32 bits) */

/* Exported define -----------------------------------------------------------*/

#define  OS_CRITICAL_METHOD   3

#if OS_CRITICAL_METHOD == 3
#define  OS_ENTER_CRITICAL()  {cpu_sr = OS_CPU_SR_Save();}
#define  OS_EXIT_CRITICAL()   {OS_CPU_SR_Restore(cpu_sr);}
#endif


#define  OS_STK_GROWTH        1                   /* Stack grows from HIGH to LOW memory on ARM */

#define  OS_TASK_SW()         OSCtxSw()

/* Exported functions ------------------------------------------------------- */

#if OS_CRITICAL_METHOD == 3                       /* See OS_CPU_A.ASM   */
OS_CPU_SR  OS_CPU_SR_Save(void);
void       OS_CPU_SR_Restore(OS_CPU_SR cpu_sr);
#endif

void       OSCtxSw(void);
void       OSIntCtxSw(void);
void       OSStartHighRdy(void);

void       OS_CPU_PendSVHandler(void);

                                                  /* See OS_CPU_C.C   */
void       OS_CPU_SysTickHandler(void);
void       OS_CPU_SysTickInit(void);

                                                  /* See BSP.C       */
INT32U     OS_CPU_SysTickClkFreq(void);
#endif
/******************** COPYRIGHT 2012 Embest Tech. Co., Ltd.*****END OF FILE****/