/**
  ******************************************************************************
  * @file    usbh_usr.c
  * @author  MCD Application Team
  * @version V2.1.0
  * @date    19-March-2012
  * @brief   This file includes the usb host library user callbacks
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
  * @file    usbh_usr.c
  * @author  CMP Team
  * @version V1.0.0
  * @date    28-December-2012
  * @brief   This file includes the usb host library user callbacks
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
#include <string.h>
#include "usbh_usr.h"
#include "lcd_log.h"
#include "ff.h"       /* FATFS */
#include "usbh_msc_core.h"
#include "usbh_msc_scsi.h"
#include "usbh_msc_bot.h"

/** @addtogroup USBH_USER
* @{
*/

/** @addtogroup USBH_MSC_DEMO_USER_CALLBACKS
* @{
*/

/** @defgroup USBH_USR 
* @brief    This file includes the usb host stack user callbacks
* @{
*/ 

/** @defgroup USBH_USR_Private_TypesDefinitions
* @{
*/ 
/**
* @}
*/ 


/** @defgroup USBH_USR_Private_Defines
* @{
*/ 
#define IMAGE_BUFFER_SIZE    512
/**
* @}
*/ 


/** @defgroup USBH_USR_Private_Macros
* @{
*/ 
extern USB_OTG_CORE_HANDLE          USB_OTG_Core;
/**
* @}
*/ 


/** @defgroup USBH_USR_Private_Variables
* @{
*/ 
uint8_t USBH_USR_ApplicationState = USH_USR_FS_INIT;
uint8_t filenameString[15]  = {0};

FATFS fatfs;
FIL file;
uint8_t Image_Buf[IMAGE_BUFFER_SIZE];
uint8_t line_idx = 0;   

/*  Points to the DEVICE_PROP structure of current device */
/*  The purpose of this register is to speed up the execution */

USBH_Usr_cb_TypeDef USR_cb =
{
  USBH_USR_Init,
  USBH_USR_DeInit,
  USBH_USR_DeviceAttached,
  USBH_USR_ResetDevice,
  USBH_USR_DeviceDisconnected,
  USBH_USR_OverCurrentDetected,
  USBH_USR_DeviceSpeedDetected,
  USBH_USR_Device_DescAvailable,
  USBH_USR_DeviceAddressAssigned,
  USBH_USR_Configuration_DescAvailable,
  USBH_USR_Manufacturer_String,
  USBH_USR_Product_String,
  USBH_USR_SerialNum_String,
  USBH_USR_EnumerationDone,
  USBH_USR_UserInput,
  USBH_USR_MSC_Application,
  USBH_USR_DeviceNotSupported,
  USBH_USR_UnrecoveredError
    
};

/**
* @}
*/

/** @defgroup USBH_USR_Private_Constants
* @{
*/ 
/*--------------- LCD Messages ---------------*/
const uint8_t MSG_HOST_INIT[]        = "> Host Library Initialized\n";
const uint8_t MSG_DEV_ATTACHED[]     = "> Device Attached \n";
const uint8_t MSG_DEV_DISCONNECTED[] = "> Device Disconnected\n";
const uint8_t MSG_DEV_ENUMERATED[]   = "> Enumeration completed \n";
const uint8_t MSG_DEV_HIGHSPEED[]    = "> High speed device detected\n";
const uint8_t MSG_DEV_FULLSPEED[]    = "> Full speed device detected\n";
const uint8_t MSG_DEV_LOWSPEED[]     = "> Low speed device detected\n";
const uint8_t MSG_DEV_ERROR[]        = "> Device fault \n";

const uint8_t MSG_MSC_CLASS[]        = "> Mass storage device connected\n";
const uint8_t MSG_HID_CLASS[]        = "> HID device connected\n";
const uint8_t MSG_DISK_SIZE[]        = "> Size of the disk in MBytes: \n";
const uint8_t MSG_LUN[]              = "> LUN Available in the device:\n";
const uint8_t MSG_ROOT_CONT[]        = "> Exploring disk flash ...\n";
const uint8_t MSG_WR_PROTECT[]       = "> The disk is write protected\n";
const uint8_t MSG_UNREC_ERROR[]      = "> UNRECOVERED ERROR STATE\n";

/**
* @}
*/


/** @defgroup USBH_USR_Private_FunctionPrototypes
* @{
*/
static uint8_t Explore_Disk (char* path , uint8_t recu_level);
static uint8_t Image_Browser (char* path);
static void     Show_Image(void);
static void     Toggle_Leds(void);
/**
* @}
*/ 


/** @defgroup USBH_USR_Private_Functions
* @{
*/ 


/**
* @brief  USBH_USR_Init 
*         Displays the message on LCD for host lib initialization
* @param  None
* @retval None
*/
void USBH_USR_Init(void)
{
  static uint8_t startup = 0;  
  
  if (startup == 0 ) {
    startup = 1;
    /* Configure the LEDs */ 
    STM_EVAL_LEDInit(LED4); 
    
    STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_GPIO);
    
    STM32f4_Discovery_LCD_Init();

    LCD_LOG_Init();
      
#ifdef USE_USB_OTG_HS 
    LCD_LOG_SetHeader(" USB OTG HS MSC Host");
#else
    LCD_LOG_SetHeader(" USB OTG FS MSC Host");
#endif
    LCD_UsrLog("> USB Host library started.\n"); 
    LCD_LOG_SetFooter ("     USB Host Library v2.1.0" );
  }
}

/**
* @brief  USBH_USR_DeviceAttached 
*         Displays the message on LCD on device attached
* @param  None
* @retval None
*/
void USBH_USR_DeviceAttached(void)
{
  LCD_UsrLog((void *)MSG_DEV_ATTACHED);
}


/**
* @brief  USBH_USR_UnrecoveredError
* @param  None
* @retval None
*/
void USBH_USR_UnrecoveredError (void)
{
  
  /* Set default screen color*/ 
  LCD_ErrLog((void *)MSG_UNREC_ERROR); 
}


/**
* @brief  USBH_DisconnectEvent
*         Device disconnect event
* @param  None
* @retval Staus
*/
void USBH_USR_DeviceDisconnected (void)
{
  
  LCD_LOG_ClearTextZone();
  
  LCD_DisplayStringLine( LCD_PIXEL_HEIGHT - 42, "                                      ");
  LCD_DisplayStringLine( LCD_PIXEL_HEIGHT - 30, "                                      ");  
  
  /* Set default screen color*/
  LCD_ErrLog((void *)MSG_DEV_DISCONNECTED);
}
/**
* @brief  USBH_USR_ResetUSBDevice 
* @param  None
* @retval None
*/
void USBH_USR_ResetDevice(void)
{
  /* callback for USB-Reset */
}


/**
* @brief  USBH_USR_DeviceSpeedDetected 
*         Displays the message on LCD for device speed
* @param  Device speed
* @retval None
*/
void USBH_USR_DeviceSpeedDetected(uint8_t DeviceSpeed)
{
  if(DeviceSpeed == HPRT0_PRTSPD_HIGH_SPEED) {
    LCD_UsrLog((void *)MSG_DEV_HIGHSPEED);
  } else if(DeviceSpeed == HPRT0_PRTSPD_FULL_SPEED) {
    LCD_UsrLog((void *)MSG_DEV_FULLSPEED);
  } else if(DeviceSpeed == HPRT0_PRTSPD_LOW_SPEED) {
    LCD_UsrLog((void *)MSG_DEV_LOWSPEED);
  } else {
    LCD_UsrLog((void *)MSG_DEV_ERROR);
  }
}

/**
* @brief  USBH_USR_Device_DescAvailable 
*         Displays the message on LCD for device descriptor
* @param  device descriptor
* @retval None
*/
void USBH_USR_Device_DescAvailable(void *DeviceDesc)
{ 
  USBH_DevDesc_TypeDef *hs;
  hs = DeviceDesc;  
  
  
  LCD_UsrLog("VID : %04Xh\n" , (uint32_t)(*hs).idVendor); 
  LCD_UsrLog("PID : %04Xh\n" , (uint32_t)(*hs).idProduct); 
}

/**
* @brief  USBH_USR_DeviceAddressAssigned 
*         USB device is successfully assigned the Address 
* @param  None
* @retval None
*/
void USBH_USR_DeviceAddressAssigned(void)
{
  
}


/**
* @brief  USBH_USR_Conf_Desc 
*         Displays the message on LCD for configuration descriptor
* @param  Configuration descriptor
* @retval None
*/
void USBH_USR_Configuration_DescAvailable(USBH_CfgDesc_TypeDef * cfgDesc,
                                          USBH_InterfaceDesc_TypeDef *itfDesc,
                                          USBH_EpDesc_TypeDef *epDesc)
{
  USBH_InterfaceDesc_TypeDef *id;
  
  id = itfDesc;  
  
  if((*id).bInterfaceClass  == 0x08) {
    LCD_UsrLog((void *)MSG_MSC_CLASS);
  } else if((*id).bInterfaceClass  == 0x03) {
    LCD_UsrLog((void *)MSG_HID_CLASS);
  }    
}

/**
* @brief  USBH_USR_Manufacturer_String 
*         Displays the message on LCD for Manufacturer String 
* @param  Manufacturer String 
* @retval None
*/
void USBH_USR_Manufacturer_String(void *ManufacturerString)
{
  LCD_UsrLog("Manufacturer : %s\n", (char *)ManufacturerString);
}

/**
* @brief  USBH_USR_Product_String 
*         Displays the message on LCD for Product String
* @param  Product String
* @retval None
*/
void USBH_USR_Product_String(void *ProductString)
{
  LCD_UsrLog("Product : %s\n", (char *)ProductString);  
}

/**
* @brief  USBH_USR_SerialNum_String 
*         Displays the message on LCD for SerialNum_String 
* @param  SerialNum_String 
* @retval None
*/
void USBH_USR_SerialNum_String(void *SerialNumString)
{
  LCD_UsrLog( "Serial Number : %s\n", (char *)SerialNumString);    
} 



/**
* @brief  EnumerationDone 
*         User response request is displayed to ask application jump to class
* @param  None
* @retval None
*/
void USBH_USR_EnumerationDone(void)
{
  
  /* Enumeration complete */
  LCD_UsrLog((void *)MSG_DEV_ENUMERATED);
  
  LCD_SetTextColor(Green);
  LCD_DisplayStringLine( LCD_PIXEL_HEIGHT - 42, "To see the root content of the disk : " );
  LCD_DisplayStringLine( LCD_PIXEL_HEIGHT - 30, "Press Key...                       ");
  LCD_SetTextColor(LCD_LOG_DEFAULT_COLOR); 
  
} 


/**
* @brief  USBH_USR_DeviceNotSupported
*         Device is not supported
* @param  None
* @retval None
*/
void USBH_USR_DeviceNotSupported(void)
{
  LCD_ErrLog ("> Device not supported."); 
}  


/**
* @brief  USBH_USR_UserInput
*         User Action for application state entry
* @param  None
* @retval USBH_USR_Status : User response for key button
*/
USBH_USR_Status USBH_USR_UserInput(void)
{
  USBH_USR_Status usbh_usr_status;
  
  usbh_usr_status = USBH_USR_NO_RESP;  
  
  /*Key B3 is in polling mode to detect user action */
  if(STM_EVAL_PBGetState(BUTTON_USER) == SET) {
    
    usbh_usr_status = USBH_USR_RESP_OK;
    
  } 
  return usbh_usr_status;
}  

/**
* @brief  USBH_USR_OverCurrentDetected
*         Over Current Detected on VBUS
* @param  None
* @retval Staus
*/
void USBH_USR_OverCurrentDetected (void)
{
  LCD_ErrLog ("Overcurrent detected.");
}


/**
* @brief  USBH_USR_MSC_Application 
*         Demo application for mass storage
* @param  None
* @retval Staus
*/
int USBH_USR_MSC_Application(void)
{
  FRESULT res;
  uint8_t writeTextBuff[] = "STM32 Connectivity line Host Demo application using FAT_FS   ";
  uint16_t bytesWritten, bytesToWrite;
  
  switch(USBH_USR_ApplicationState)
  {
  case USH_USR_FS_INIT: 
    
    /* Initialises the File System*/
    if ( f_mount( 0, &fatfs ) != FR_OK ) {
      /* efs initialisation fails*/
      LCD_ErrLog("> Cannot initialize File System.\n");
      return(-1);
    }
    LCD_UsrLog("> File System initialized.\n");
    LCD_UsrLog("> Disk capacity : %d Bytes\n", USBH_MSC_Param.MSCapacity * \
      USBH_MSC_Param.MSPageLength); 
    
    if(USBH_MSC_Param.MSWriteProtect == DISK_WRITE_PROTECTED) {
      LCD_ErrLog((void *)MSG_WR_PROTECT);
    }
    
    USBH_USR_ApplicationState = USH_USR_FS_READLIST;
    break;
    
  case USH_USR_FS_READLIST:
    
    LCD_UsrLog((void *)MSG_ROOT_CONT);
    Explore_Disk("0:/", 1);
    line_idx = 0;   
    USBH_USR_ApplicationState = USH_USR_FS_WRITEFILE;
    
    break;
    
  case USH_USR_FS_WRITEFILE:
    
    LCD_SetTextColor(Green);
    LCD_DisplayStringLine( LCD_PIXEL_HEIGHT - 42, "                                              ");
    LCD_DisplayStringLine( LCD_PIXEL_HEIGHT - 30, "Press Key to write file");
    LCD_SetTextColor(LCD_LOG_DEFAULT_COLOR); 
    USB_OTG_BSP_mDelay(100);
    
    /*Key B3 in polling*/
    while((HCD_IsDeviceConnected(&USB_OTG_Core)) && \
      (STM_EVAL_PBGetState (BUTTON_USER) == RESET))          
    {
      Toggle_Leds();
    }
    /* Writes a text file, STM32.TXT in the disk*/
    LCD_UsrLog("> Writing File to disk flash ...\n");
    if (USBH_MSC_Param.MSWriteProtect == DISK_WRITE_PROTECTED) {
      
      LCD_ErrLog ( "> Disk flash is write protected \n");
      USBH_USR_ApplicationState = USH_USR_FS_DRAW;
      break;
    }
    
    /* Register work area for logical drives */
    f_mount(0, &fatfs);
    
    if (f_open(&file, "0:STM32.TXT",FA_CREATE_ALWAYS | FA_WRITE) == FR_OK) { 
      /* Write buffer to file */
      bytesToWrite = sizeof(writeTextBuff); 
      res= f_write (&file, writeTextBuff, bytesToWrite, (void *)&bytesWritten);   
      
      if((bytesWritten == 0) || (res != FR_OK)) /*EOF or Error*/ {
        LCD_ErrLog("> STM32.TXT CANNOT be writen.\n");
      } else {
        LCD_UsrLog("> 'STM32.TXT' file created\n");
      }
      
      /*close file and filesystem*/
      f_close(&file);
      f_mount(0, NULL); 
    } else {
      LCD_UsrLog ("> STM32.TXT created in the disk\n");
    }

    USBH_USR_ApplicationState = USH_USR_FS_DRAW; 
    
    LCD_SetTextColor(Green);
    LCD_DisplayStringLine( LCD_PIXEL_HEIGHT - 42, "                                              ");
    LCD_DisplayStringLine( LCD_PIXEL_HEIGHT - 30, "To start Image slide show Press Key.");
    LCD_SetTextColor(LCD_LOG_DEFAULT_COLOR); 
  
    break;
    
  case USH_USR_FS_DRAW:
    
    /*Key B3 in polling*/
    while((HCD_IsDeviceConnected(&USB_OTG_Core)) && \
      (STM_EVAL_PBGetState (BUTTON_USER) == RESET))
    {
      Toggle_Leds();
    }
  
    while(HCD_IsDeviceConnected(&USB_OTG_Core))
    {
      if ( f_mount( 0, &fatfs ) != FR_OK ) {
        /* fat_fs initialisation fails*/
        return(-1);
      }
      return Image_Browser("0:/");
    }
    break;
  default: break;
  }
  return(0);
}

/**
* @brief  Explore_Disk 
*         Displays disk content
* @param  path: pointer to root path
* @retval None
*/
static uint8_t Explore_Disk (char* path , uint8_t recu_level)
{

  FRESULT res;
  FILINFO fno;
  DIR dir;
  char *fn;
  char tmp[14];
  
  res = f_opendir(&dir, path);
  if (res == FR_OK) {
    while(HCD_IsDeviceConnected(&USB_OTG_Core)) 
    {
      res = f_readdir(&dir, &fno);
      if (res != FR_OK || fno.fname[0] == 0) 
      {
        break;
      }
      if (fno.fname[0] == '.') {
        continue;
      }

      fn = fno.fname;
      strcpy(tmp, fn); 

      line_idx++;
      if (line_idx > 9) {
        line_idx = 0;
        LCD_SetTextColor(Green);
        LCD_DisplayStringLine( LCD_PIXEL_HEIGHT - 42, "                                              ");
        LCD_DisplayStringLine( LCD_PIXEL_HEIGHT - 30, "Press Key to continue...");
        LCD_SetTextColor(LCD_LOG_DEFAULT_COLOR); 
        
        /*Key B3 in polling*/
        while((HCD_IsDeviceConnected(&USB_OTG_Core)) && \
          (STM_EVAL_PBGetState (BUTTON_USER) == RESET)) {
          Toggle_Leds();
          
        }
      } 
      
      if (recu_level == 1) {
        LCD_DbgLog("   |__");
      } else if(recu_level == 2) {
        LCD_DbgLog("   |   |__");
      }
      if ((fno.fattrib & AM_MASK) == AM_DIR) {
        strcat(tmp, "\n"); 
        LCD_UsrLog((void *)tmp);
      } else {
        strcat(tmp, "\n"); 
        LCD_DbgLog((void *)tmp);
      }

      if (((fno.fattrib & AM_MASK) == AM_DIR)&&(recu_level == 1)) {
        Explore_Disk(fn, 2);
      }
    }
  }
  return res;
}

static uint8_t Image_Browser (char* path)
{
  FRESULT res;
  uint8_t ret = 1;
  FILINFO fno;
  DIR dir;
  char *fn;
  
  res = f_opendir(&dir, path);
  if (res == FR_OK) {
    
    for (;;) {
      res = f_readdir(&dir, &fno);
      if (res != FR_OK || fno.fname[0] == 0) break;
      if (fno.fname[0] == '.') continue;

      fn = fno.fname;
 
      if (fno.fattrib & AM_DIR) {
        continue;
      } else {
        if ((strstr(fn, "bmp")) || (strstr(fn, "BMP"))) {
          res = f_open(&file, fn, FA_OPEN_EXISTING | FA_READ);
          Show_Image();
          USB_OTG_BSP_mDelay(100);
          ret = 0;
          while((HCD_IsDeviceConnected(&USB_OTG_Core)) && \
            (STM_EVAL_PBGetState (BUTTON_USER) == RESET))
          {
            Toggle_Leds();
          }
          f_close(&file);
          
        }
      }
    }  
  }
  
  #ifdef USE_USB_OTG_HS 
  LCD_LOG_SetHeader(" USB OTG HS MSC Host");
#else
  LCD_LOG_SetHeader(" USB OTG FS MSC Host");
#endif
  LCD_LOG_SetFooter ("     USB Host Library v2.1.0" );
  LCD_UsrLog("> Disk capacity : %d Bytes\n", USBH_MSC_Param.MSCapacity * \
  USBH_MSC_Param.MSPageLength); 
  USBH_USR_ApplicationState = USH_USR_FS_READLIST;
  return ret;
}

/**
* @brief  Show_Image 
*         Displays BMP image
* @param  None
* @retval None
*/
/**
* @brief  Show_Image 
*         Displays BMP image
* @param  None
* @retval None
*/
static void Show_Image(void)
{
  uint16_t i = 0;
  uint16_t numOfReadBytes = 0;
  FRESULT res; 

  LCD_SetDisplayWindow(0, 0, 320, 240);
  LCD_WriteReg(SSD2119_ENTRY_MODE_REG, ENTRY_MODE_BMP);
  LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */

  /* Bypass Bitmap header */ 
  f_lseek (&file, 54);
  
  while (HCD_IsDeviceConnected(&USB_OTG_Core))
  {
    res = f_read(&file, Image_Buf, IMAGE_BUFFER_SIZE, (void *)&numOfReadBytes);
    if ((numOfReadBytes == 0) || (res != FR_OK)) {
      /*EOF or Error*/ 
      LCD_WriteReg(SSD2119_ENTRY_MODE_REG, ENTRY_MODE_DEFAULT);		
      break; 
    }
    for (i = 0 ; i < IMAGE_BUFFER_SIZE; i+= 2) {
      LCD_WriteRAM(Image_Buf[i+1] << 8 | Image_Buf[i]); 
    } 
  }
}

/**
* @brief  Toggle_Leds
*         Toggle leds to shows user input state
* @param  None
* @retval None
*/
static void Toggle_Leds(void)
{
  static uint32_t i;
  if (i++ == 0x10000) {
    STM_EVAL_LEDToggle(LED4);
    i = 0;
  }  
}
/**
* @brief  USBH_USR_DeInit
*         Deint User state and associated variables
* @param  None
* @retval None
*/
void USBH_USR_DeInit(void)
{
  USBH_USR_ApplicationState = USH_USR_FS_INIT;
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

