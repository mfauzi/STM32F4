/**
  ******************************************************************************
  * @file    usbd_usr.c
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    19-March-2012
  * @brief   This file includes the user application layer
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Portions COPYRIGHT 2012 STMicroelectronics</center></h2>
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
/**
  ******************************************************************************
  * <h2><center>&copy; Portions COPYRIGHT 2012 Embest Tech. Co., Ltd.</center></h2>
  * @file    usbd_usr.c
  * @author  CMP Team
  * @version V1.0.0
  * @date    28-December-2012
  * @brief   This file includes the user application layer
  *          Modified to support the STM32F4DISCOVERY, STM32F4DIS-BB and
  *          STM32F4DIS-LCD modules.      
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
/* Includes ------------------------------------------------------------------*/
#include "usbd_usr.h"
#include "stm32f4_discovery_lcd.h"
#include "lcd_log.h"
#include <stdio.h>
/** @addtogroup USBD_USER
  * @{
  */

/** @addtogroup USBD_MSC_DEMO_USER_CALLBACKS
  * @{
  */
  
/** @defgroup USBD_USR 
  * @brief    This file includes the user application layer
  * @{
  */ 

/** @defgroup USBD_USR_Private_TypesDefinitions
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup USBD_USR_Private_Defines
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup USBD_USR_Private_Macros
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup USBD_USR_Private_Variables
  * @{
  */ 
/*  Points to the DEVICE_PROP structure of current device */
/*  The purpose of this register is to speed up the execution */

USBD_Usr_cb_TypeDef USR_cb =
{
  USBD_USR_Init,
  USBD_USR_DeviceReset,
  USBD_USR_DeviceConfigured,
  USBD_USR_DeviceSuspended,
  USBD_USR_DeviceResumed,
  
  USBD_USR_DeviceConnected,
  USBD_USR_DeviceDisconnected,    
};

/**
  * @}
  */

/** @defgroup USBD_USR_Private_Constants
  * @{
  */ 

#define USER_INFORMATION1  "INFO : Single Lun configuration" 
#define USER_INFORMATION2  "INFO : microSD is used"
/**
  * @}
  */



/** @defgroup USBD_USR_Private_FunctionPrototypes
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup USBD_USR_Private_Functions
  * @{
  */ 

/**
* @brief  Displays the message on LCD on device lib initialization
* @param  None
* @retval None
*/
void USBD_USR_Init(void)
{
  /* Initialize LEDs */
  STM_EVAL_LEDInit(LED4);   

  /* Init LCD */
  STM32f4_Discovery_LCD_Init();
  
  LCD_LOG_Init();
  
#ifdef USE_USB_OTG_HS 
  LCD_LOG_SetHeader(" USB OTG HS MSC Device");
#else
  LCD_LOG_SetHeader(" USB OTG FS MSC Device");
#endif
  LCD_UsrLog("> USB device library started.\n"); 
  LCD_LOG_SetFooter ("     USB Device Library v1.1.0" );
  
  
  /* Information panel */
  LCD_SetTextColor(Green);
  LCD_DisplayStringLine( LCD_PIXEL_HEIGHT - 42, USER_INFORMATION1);
  LCD_DisplayStringLine( LCD_PIXEL_HEIGHT - 30, USER_INFORMATION2);
  LCD_SetTextColor(LCD_LOG_DEFAULT_COLOR);  
}

/**
* @brief  Displays the message on LCD on device reset event
* @param  speed : device speed
* @retval None
*/
void USBD_USR_DeviceReset (uint8_t speed)
{
 switch (speed)
 {
  case USB_OTG_SPEED_HIGH: 
     LCD_LOG_SetFooter ("     USB Device Library v1.1.0  [HS]" );
     break;

  case USB_OTG_SPEED_FULL: 
     LCD_LOG_SetFooter ("     USB Device Library v1.1.0  [FS]" );
     break;

  default:
     LCD_LOG_SetFooter ("     USB Device Library v1.1.0  [??]" );
 }
}


/**
* @brief  Displays the message on LCD on device config event
* @param  None
* @retval Staus
*/
void USBD_USR_DeviceConfigured (void)
{
  LCD_UsrLog("> MSC Interface started.\n");

}
/**
* @brief  Displays the message on LCD on device suspend event 
* @param  None
* @retval None
*/
void USBD_USR_DeviceSuspended(void)
{
  LCD_UsrLog("> Device In suspend mode.\n");
}


/**
* @brief  Displays the message on LCD on device resume event
* @param  None
* @retval None
*/
void USBD_USR_DeviceResumed(void)
{

}

/**
* @brief  USBD_USR_DeviceConnected
*         Displays the message on LCD on device connection Event
* @param  None
* @retval Staus
*/
void USBD_USR_DeviceConnected (void)
{
  LCD_UsrLog("> USB Device Connected.\n");
}


/**
* @brief  USBD_USR_DeviceDisonnected
*         Displays the message on LCD on device disconnection Event
* @param  None
* @retval Staus
*/
void USBD_USR_DeviceDisconnected (void)
{
  LCD_UsrLog("> USB Device Disconnected.\n");
}
/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */

/**
  * @}
  */

/********* Portions COPYRIGHT 2012 Embest Tech. Co., Ltd.*****END OF FILE******/
