/**
  ******************************************************************************
  * @file    app.c
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    19-March-2012
  * @brief   This file provides all the Application firmware functions.
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
  * @file    app.c
  * @author  CMP Team
  * @version V1.0.0
  * @date    28-December-2012
  * @brief   This file provides all the Application firmware functions.
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
#include "usbd_msc_core.h"
#include "usbd_usr.h"
#include "usbd_desc.h"
#include "usb_conf.h"

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @{
  */


/** @defgroup APP_MSC 
  * @brief Mass storage application module
  * @{
  */ 

/** @defgroup APP_MSC_Private_TypesDefinitions
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup APP_MSC_Private_Defines
  * @{
  */ 

/**
  * @}
  */ 


/** @defgroup APP_MSC_Private_Macros
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup APP_MSC_Private_Variables
  * @{
  */ 

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */

__ALIGN_BEGIN USB_OTG_CORE_HANDLE     USB_OTG_dev __ALIGN_END ;

/**
  * @}
  */ 


/** @defgroup APP_MSC_Private_FunctionPrototypes
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup APP_MSC_Private_Functions
  * @{
  */ 

/**
  * @brief  Program entry point
  * @param  None
  * @retval None
  */
int main(void)
{
  __IO uint32_t i = 0;

  /*!< At this stage the microcontroller clock setting is already configured, 
  this is done through SystemInit() function which is called from startup
  file (startup_stm32fxxx_xx.s) before to branch to application main.
  To reconfigure the default setting of SystemInit() function, refer to
  system_stm32fxxx.c file
  */ 

  USBD_Init(&USB_OTG_dev,
#ifdef USE_USB_OTG_HS 
            USB_OTG_HS_CORE_ID,
#else            
            USB_OTG_FS_CORE_ID,
#endif            
            &USR_desc,
            &USBD_MSC_cb, 
            &USR_cb);

  while (1)
  {
    if (i++ == 0x100000)
    {
      STM_EVAL_LEDToggle(LED4);
      i = 0;
    }    
  }
} 

#ifdef USE_FULL_ASSERT
/**
* @brief  assert_failed
*         Reports the name of the source file and the source line number
*         where the assert_param error has occurred.
* @param  File: pointer to the source file name
* @param  Line: assert_param error line source number
* @retval None
*/
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
  ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  
  /* Infinite loop */
  while (1)
  {}
}
#endif

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
