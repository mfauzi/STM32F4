/**
  ******************************************************************************
  * @file    ADC/ADC_DualModeRegulSimu/main.c 
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    18-January-2013
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#if defined (USE_STM324xG_EVAL)

  #define ADC_CCR_ADDRESS    ((uint32_t)0x40012308)

#else /* defined (USE_STM324x7I_EVAL)*/ 

  #define ADC_CCR_ADDRESS    ((uint32_t)0x40012308)

#endif
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
