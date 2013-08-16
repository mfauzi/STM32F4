/**
  ******************************************************************************
  * @file    usbh_usr_lcd.c
  * @author  MCD Application Team
  * @version V2.1.0
  * @date    19-March-2012
  * @brief   This file includes the some user routines for LCD 
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
  * @file    usbh_usr_lcd.c
  * @author  CMP Team
  * @version V1.0.0
  * @date    28-December-2012
  * @brief   This file includes the some user routines for LCD 
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
#include "stm32f4_discovery_lcd.h"
#include "usbh_usr_lcd.h"
#include "usb_conf.h"

/** @addtogroup USBH_USER
* @{
*/


/** @defgroup USBH_USR_LCD 
* @brief    This file includes the some user routines for LCD 
* @{
*/ 

/** @defgroup USBH_USR_LCD_Private_TypesDefinitions
* @{
*/ 
/**
* @}
*/ 


/** @defgroup USBH_USR_LCD_Private_Defines
* @{
*/ 
/**
* @}
*/ 


/** @defgroup USBH_USR_LCD_Private_Macros
* @{
*/ 
/**
* @}
*/ 


/** @defgroup USBH_USR_LCD_Private_Variables
* @{
*/ 

int16_t  x_loc  = 0, y_loc  = 0; 
__IO int16_t  prev_x = 0, prev_y = 0;
/**
* @}
*/ 

/** @defgroup USBH_USR_LCD_Exported_Variables
* @{
*/ 
/**
* @}
*/ 


/** @defgroup USBH_USR_LCD_Private_FunctionPrototypes
* @{
*/ 
/**
* @}
*/ 


/** @defgroup USBH_USR_LCD_Private_Functions
* @{
*/ 

/**
* @brief  HID_MOUSE_UpdatePosition
*         The function is to handle mouse scroll to upadte the mouse position 
*         on display window
* @param   x : USB HID Mouse X co-ordinate
* @param   y :  USB HID Mouse Y co-ordinate
* @retval None
*/
void HID_MOUSE_UpdatePosition (int8_t x, int8_t y)
{
   /* Set the color for button press status */
  LCD_SetTextColor(LCD_COLOR_GREEN);
  LCD_SetBackColor(LCD_COLOR_BLACK);
  
  if ((x != 0) || (y != 0)) {
    
    x_loc += x/2;
    y_loc += y/2;
    
    if (y_loc > MOUSE_WINDOW_HEIGHT - 12){
      y_loc = MOUSE_WINDOW_HEIGHT - 12;
    }  
    if (x_loc > MOUSE_WINDOW_WIDTH - 10){
      x_loc = MOUSE_WINDOW_WIDTH - 10;
    }  
    if (y_loc < 2){
      y_loc = 2;
    }  
    if (x_loc < 2){
      x_loc = 2;
    } 
    
    if ((prev_x != 0) && (prev_y != 0))
    {
      LCD_DisplayChar(MOUSE_WINDOW_Y + prev_y, MOUSE_WINDOW_X + prev_x, ' ');
    } else if((prev_x == 0) && ( prev_y == 0)) {
      
      LCD_DisplayChar(MOUSE_WINDOW_Y + 1,
                                MOUSE_WINDOW_X + 1,
                                ' ');    
    }
    
    LCD_DisplayChar  (MOUSE_WINDOW_Y + y_loc ,\
      MOUSE_WINDOW_X + x_loc, 'x');
    
    prev_x = x_loc;
    prev_y = y_loc;
  }
}  

/**
* @brief  HID_MOUSE_ButtonPressed
*         The function is to handle mouse button press
* @param  button_idx : mouse button pressed
* @retval None
*/

void HID_MOUSE_ButtonPressed(uint8_t button_idx)
{
  
  /* Set the color for button press status */
  LCD_SetTextColor(LCD_COLOR_GREEN);
  LCD_SetBackColor(LCD_COLOR_GREEN);
  
  /* Change the color of button pressed to indicate button press*/
  switch (button_idx)
  {
    /* Left Button Pressed */
  case 0 :
    LCD_DrawFullRect(HID_MOUSE_BUTTON1_XCHORD, HID_MOUSE_BUTTON_YCHORD,\
    HID_MOUSE_BUTTON_WIDTH, HID_MOUSE_BUTTON_HEIGHT );
    break;
    
    
    /* Right Button Pressed */  
  case 1 :
    LCD_DrawFullRect(HID_MOUSE_BUTTON3_XCHORD,HID_MOUSE_BUTTON_YCHORD,\
    HID_MOUSE_BUTTON_WIDTH,HID_MOUSE_BUTTON_HEIGHT);
    break; 
    
    /* Middle button Pressed */  
  case 2 :
    LCD_DrawFullRect(HID_MOUSE_BUTTON2_XCHORD,HID_MOUSE_BUTTON_YCHORD,\
    HID_MOUSE_BUTTON_WIDTH,HID_MOUSE_BUTTON_HEIGHT);
    break;
    
  }
}

/**
* @brief  HID_Mouse_ButtonReleased
*         The function is to handle mouse button release
* @param  button_idx : mouse button released
* @retval None
*/

void HID_MOUSE_ButtonReleased(uint8_t button_idx)
{
  
  /* Set the color for release status */
  LCD_SetTextColor(LCD_COLOR_WHITE);
  LCD_SetBackColor(LCD_COLOR_WHITE);
  
  /* Change the color of button released to default button color*/
  switch (button_idx)
  {
    
    /* Left Button Released */
  case 0 :
    LCD_DrawFullRect(HID_MOUSE_BUTTON1_XCHORD, HID_MOUSE_BUTTON_YCHORD,\
    HID_MOUSE_BUTTON_WIDTH, HID_MOUSE_BUTTON_HEIGHT);
    break;
    
    /* Right Button Released */  
  case 1 :
    LCD_DrawFullRect(HID_MOUSE_BUTTON3_XCHORD, HID_MOUSE_BUTTON_YCHORD,\
    HID_MOUSE_BUTTON_WIDTH, HID_MOUSE_BUTTON_HEIGHT);
    break;
    
    /* Middle Button Released */  
  case 2 :
    LCD_DrawFullRect (HID_MOUSE_BUTTON2_XCHORD, HID_MOUSE_BUTTON_YCHORD,\
    HID_MOUSE_BUTTON_WIDTH, HID_MOUSE_BUTTON_HEIGHT);
    break;
  }
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

/********* Portions COPYRIGHT 2012 Embest Tech. Co., Ltd.*****END OF FILE******/
