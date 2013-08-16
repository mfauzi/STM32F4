/**
  ******************************************************************************
  * @file    usbh_usr_lcd.h
  * @author  MCD Application Team
  * @version V2.0.0
  * @date    22-July-2011
  * @brief   This file includes the declarations for user routines for LCD 
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; Portions COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 
/**
  ******************************************************************************
  * <h2><center>&copy; Portions COPYRIGHT 2012 Embest Tech. Co., Ltd.</center></h2>
  * @file    usbh_usr_lcd.h
  * @author  CMP Team
  * @version V1.0.0
  * @date    28-December-2012
  * @brief   This file includes the declarations for user routines for LCD 
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBH_USR_LCD_H
#define __USBH_USR_LCD_H

/* Includes ------------------------------------------------------------------*/
#include "usbh_hid_core.h"

/** @addtogroup USBH_USER
  * @{
  */
  
/** @defgroup USBH_USR_LCD 
  * @brief This file is the header file for user LCD routines
  * @{
  */ 

/** @defgroup USBH_USR_LCD_Exported_Types
  * @{
  */ 


/**
  * @}
  */ 

/** @defgroup USBH_USR_LCD_Exported_Defines
  * @{
  */ 



/**
  * @}
  */ 

/** @defgroup USBH_USR_LCD_Exported_Constants
  * @{
  */ 
/*Left Button : Report data :0x01*/
#define HID_MOUSE_BUTTON1                0x01 
/*Right Button : Report data :0x02*/
#define HID_MOUSE_BUTTON2                0x02 
/*Middle Button : Report data : 0x04*/
#define HID_MOUSE_BUTTON3                0x04 

/* Mouse directions */
#define MOUSE_TOP_DIR                   0x80
#define MOUSE_BOTTOM_DIR                0x00
#define MOUSE_LEFT_DIR                  0x80
#define MOUSE_RIGHT_DIR                 0x00

#define MOUSE_WINDOW_X                  92
#define MOUSE_WINDOW_Y                  100
#define MOUSE_WINDOW_X_MAX              181
#define MOUSE_WINDOW_Y_MIN              101
#define MOUSE_WINDOW_HEIGHT             90
#define MOUSE_WINDOW_WIDTH              128

#define HID_MOUSE_BUTTON_HEIGHT         10
#define HID_MOUSE_BUTTON_WIDTH          24

#define HID_MOUSE_BUTTON_YCHORD         201
#define HID_MOUSE_BUTTON3_XCHORD        182
#define HID_MOUSE_BUTTON2_XCHORD        144
#define HID_MOUSE_BUTTON1_XCHORD        106

#define MOUSE_LEFT_MOVE                  1
#define MOUSE_RIGHT_MOVE                 2
#define MOUSE_UP_MOVE                    3
#define MOUSE_DOWN_MOVE                  4

#define HID_MOUSE_HEIGHTLSB              2
#define HID_MOUSE_WIDTHLSB               2
#define HID_MOUSE_RES_X                  4  
#define HID_MOUSE_RES_Y                  4

#define SMALL_FONT_COLUMN_WIDTH          8
#define SMALL_FONT_LINE_WIDTH            12

/**
  * @}
  */ 

/** @defgroup USBH_USR_LCD_Exported_Variables
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup USBH_USR_LCD_Exported_Functions
  * @{
  */ 
void HID_MOUSE_UpdatePosition(int8_t x ,int8_t y);
void HID_MOUSE_ButtonReleased(uint8_t button_idx);
void HID_MOUSE_ButtonPressed (uint8_t button_idx);

/**
  * @}
  */ 

#endif /* __USBH_USR_LCD_H */

/**
  * @}
  */ 

/**
  * @}
  */ 
/********* Portions COPYRIGHT 2012 Embest Tech. Co., Ltd.*****END OF FILE******/
