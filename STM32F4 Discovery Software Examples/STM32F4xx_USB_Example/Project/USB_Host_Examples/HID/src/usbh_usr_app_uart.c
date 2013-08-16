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
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
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

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
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
  
  if ((x != 0) || (y != 0)) 
  {
    
    x_loc += x/2;
    y_loc += y/2;
    
    if(y_loc > MOUSE_WINDOW_Y_MAX){
      y_loc = MOUSE_WINDOW_Y_MAX;
    }  
    if(x_loc > MOUSE_WINDOW_X_MAX){
      x_loc = MOUSE_WINDOW_X_MAX;
    }  
    if(y_loc < 2){
      y_loc = 2;
    }  
    if(x_loc < 2){
      x_loc = 2;
    } 
		printf("Mouse x-POS:%d",x_loc);
		printf("      Mouse y-POS:%d",y_loc);
		printf("\n\r");
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

  /* Change the color of button pressed to indicate button press*/
  switch (button_idx)
  {
    /* Left Button Pressed */
  case 0 :
		printf("> Left Button Pressed \n\r");
    break;
    
    
    /* Right Button Pressed */  
  case 1 :
	  printf("> Right Button Pressed \n\r");
    break; 
    
    /* Middle button Pressed */  
  case 2 :
	  printf("> Middle Button Pressed \n\r");
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

  /* Change the color of button released to default button color*/
  switch (button_idx)
  {
    
    /* Left Button Released */
  case 0 :
	  printf("> Left Button Released \n\r");
    break;
    
    /* Right Button Released */  
  case 1 :
    printf("> Right Button Released \n\r");
    break;
    
    /* Middle Button Released */  
  case 2 :
    printf("> Middle Button Released \n\r");
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
