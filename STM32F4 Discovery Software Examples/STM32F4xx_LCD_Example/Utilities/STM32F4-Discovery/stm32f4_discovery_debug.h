/**
  ******************************************************************************
  * <h2><center>&copy; COPYRIGHT 2012 Embest Tech. Co., Ltd.</center></h2>
  * @file    stm32f4_discovery_debug.h
  * @author  CMP Team
  * @version V1.0.0
  * @date    28-December-2012
  * @brief   Header for stm32f4_discovery_debug.c module     
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
#ifndef __STM32F4_DISCOVERY_DEBUG_H
#define __STM32F4_DISCOVERY_DEBUG_H

#ifdef __cplusplus
 extern "C" {
#endif
                                              
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
   
/** @addtogroup Utilities
  * @{
  */
  
/** @addtogroup STM32F4_DISCOVERY
  * @{
  */
/** @defgroup STM32F4_DISCOVERY_LOW_LEVEL_Exported_Functions
  * @{
  */
void STM32f4_Discovery_Debug_Init(void);
int Get_Peek_Key(void);
/**
  * @}
  */
#ifdef __cplusplus
}
#endif

#endif /* __STM32F4_DISCOVERY_DEBUG_H */
/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */

 

/******************** COPYRIGHT 2012 Embest Tech. Co., Ltd.*****END OF FILE****/
