/**
  ******************************************************************************
  * @file    FLASH/FLASH_Program/main.c 
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

#if defined (USE_STM324xG_EVAL)
  #include "stm324xg_eval.h"

#elif defined (USE_STM324x7I_EVAL) 
  #include "stm324x7i_eval.h"

#else
 #error "Please select first the Evaluation board used in your application (in Project Options)"
#endif

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define FLASH_USER_START_ADDR   ADDR_FLASH_SECTOR_4   /* Start @ of user Flash area */

#if defined (USE_STM324xG_EVAL)
#define FLASH_USER_END_ADDR     ADDR_FLASH_SECTOR_11   /* End @ of user Flash area */

#else /* USE_STM324x7I_EVAL*/
#define FLASH_USER_END_ADDR     ADDR_FLASH_SECTOR_23   /* End @ of user Flash area */
#endif 

/* Base address of the Flash sectors */ 
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base @ of Sector 0, 16 Kbytes */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) /* Base @ of Sector 1, 16 Kbytes */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) /* Base @ of Sector 2, 16 Kbytes */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) /* Base @ of Sector 3, 16 Kbytes */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) /* Base @ of Sector 4, 64 Kbytes */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) /* Base @ of Sector 5, 128 Kbytes */
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) /* Base @ of Sector 6, 128 Kbytes */
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) /* Base @ of Sector 7, 128 Kbytes */
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) /* Base @ of Sector 8, 128 Kbytes */
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000) /* Base @ of Sector 9, 128 Kbytes */
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000) /* Base @ of Sector 10, 128 Kbytes */
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000) /* Base @ of Sector 11, 128 Kbytes */

#if defined (USE_STM324x7I_EVAL)
#define ADDR_FLASH_SECTOR_12     ((uint32_t)0x08100000) /* Base @ of Sector 12, 16 Kbytes */
#define ADDR_FLASH_SECTOR_13     ((uint32_t)0x08104000) /* Base @ of Sector 13, 16 Kbytes */
#define ADDR_FLASH_SECTOR_14     ((uint32_t)0x08108000) /* Base @ of Sector 14, 16 Kbytes */
#define ADDR_FLASH_SECTOR_15     ((uint32_t)0x0810C000) /* Base @ of Sector 15, 16 Kbytes */
#define ADDR_FLASH_SECTOR_16     ((uint32_t)0x08110000) /* Base @ of Sector 16, 64 Kbytes */
#define ADDR_FLASH_SECTOR_17     ((uint32_t)0x08120000) /* Base @ of Sector 17, 128 Kbytes */
#define ADDR_FLASH_SECTOR_18     ((uint32_t)0x08140000) /* Base @ of Sector 18, 128 Kbytes */
#define ADDR_FLASH_SECTOR_19     ((uint32_t)0x08160000) /* Base @ of Sector 19, 128 Kbytes */
#define ADDR_FLASH_SECTOR_20     ((uint32_t)0x08180000) /* Base @ of Sector 20, 128 Kbytes  */
#define ADDR_FLASH_SECTOR_21     ((uint32_t)0x081A0000) /* Base @ of Sector 21, 128 Kbytes  */
#define ADDR_FLASH_SECTOR_22     ((uint32_t)0x081C0000) /* Base @ of Sector 22, 128 Kbytes */
#define ADDR_FLASH_SECTOR_23     ((uint32_t)0x081E0000) /* Base @ of Sector 23, 128 Kbytes */
#endif /* USE_STM324x7I_EVAL */

#define DATA_32                 ((uint32_t)0x12345678)

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */


#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
