/**
  ******************************************************************************
  * <h2><center>&copy; COPYRIGHT 2012 Embest Tech. Co., Ltd.</center></h2>
  * @file    cpu_def.h 
  * @author  CMP Team
  * @version V1.0.0
  * @date    28-December-2012
  * @brief   cpu configuration and port layer file.                       
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
#ifndef __CPU_DEF_H__
#define __CPU_DEF_H__
/* Exported define -----------------------------------------------------------*/
/* ------------------------------ CPU WORD SIZE ----------------------------- */
/*  8-bit word size = sizeof(CPU_INT08x). */
#define  CPU_WORD_SIZE_08                          1    
/* 16-bit word size = sizeof(CPU_INT16x). */
#define  CPU_WORD_SIZE_16                          2    
/* 32-bit word size = sizeof(CPU_INT32x). */
#define  CPU_WORD_SIZE_32                          4
/* 64-bit word size = sizeof(CPU_INT64x) [see Note #1a]. */
#define  CPU_WORD_SIZE_64                          8    


/* --------------------------- CPU WORD-ENDIAN ORDER -------------------------*/                                                        
#define  CPU_ENDIAN_TYPE_NONE                      0 
/* Big-   endian word order (CPU words' most  significant */
#define  CPU_ENDIAN_TYPE_BIG                       1    
/* Little-endian word order (CPU words' least significant */
#define  CPU_ENDIAN_TYPE_LITTLE                    2   


/* ----------------------- CPU CRITICAL SECTION METHODS --------------------- */
#define  CPU_CRITICAL_METHOD_NONE                  0 
/* DIS/EN ints */
#define  CPU_CRITICAL_METHOD_INT_DIS_EN            1 
/* Push/Pop int status onto stk. */
#define  CPU_CRITICAL_METHOD_STATUS_STK            2 
/* Save/Restore int status to local var */
#define  CPU_CRITICAL_METHOD_STATUS_LOCAL          3    

#endif /*__CPU_DEF_H__*/

/******************** COPYRIGHT 2012 Embest Tech. Co., Ltd.*****END OF FILE****/
