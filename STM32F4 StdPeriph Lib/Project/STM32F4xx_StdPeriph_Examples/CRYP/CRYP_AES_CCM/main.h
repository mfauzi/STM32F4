/**
  ******************************************************************************
  * @file    CRYP/CRYP_AES_CCM/main.h 
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
#include "stdio.h"

#if defined (USE_STM324x7I_EVAL) 
  #include "stm324x7i_eval.h"

#else
 #error "Please select first the Evaluation board used in your application (in Project Options)"
#endif

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* The size of the plain text in bytes. It must be a multiple of 16.
   AES is a block cipher algorithm. The plaint text must be multiple of 128 bits = 16 bytes */
#define PLAINTEXT_SIZE    16
/* The size of the key in bits must be 128 bits (16 bytes), 192 bits (24 bytes)
   or 256 bits (32 bytes) */
#define KEY_SIZE          128 /* Key size in bits */
/* The nonce size can be 7, 8, 9, 10, 11, 12 or 13 */
#define NONCE_SIZE        8
/* The size of the header called also associated data length */
#define HEADER_SIZE       16
/* The MAC(TAG) size can be 4, 6, 8, 10, 12, 14 or 16 */
#define MAC_SIZE          6
#define AES_MODE_CCM      1

/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
