/**
  @page USBH_HID  USB Host HID example
  
  @verbatim
  ***************** Portions COPYRIGHT 2012 STMicroelectronics *****************
  * @file    readme.txt 
  * @author  MCD Application Team
  * @version V2.1.0
  * @date    19-March-2012
  * @brief   Description of the USB Host HID example
  ******************************************************************************
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
  * limitations under the Licens
  *   
  ******************************************************************************
   @endverbatim
  */
/**
  @page USBH_HID  USB Host HID example
  
  @verbatim
  ************** Portions COPYRIGHT 2012 Embest Tech. Co., Ltd.** **************
  * @file    readme.txt 
  * @author  CMP Team
  * @version V1.0.0
  * @date    28-December-2012
  * @brief   Description of the USB Host HID example
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
  @endverbatim
  */

@par Example Description 

This example provides a description of how to use the USB OTG host peripheral 
on the STM32F2xx, STM32F4xx and STM32F105/7 devices.

When an USB Device is attached to the Host port, the device is enumerated and checked
whether it can support HID device or not, if the attached device is HID one, when the user
press the User Button, the mouse or the keyboard application is launched.

Moving the mouse will move the pointer in the display rectangle and if User Button is pressed,
the corresponding rectangle will be highlighted in green
If a keyboard has been attached, the display show the following messages and the taped
characters are displayed in green on the display.

This example works:
  - in full speed (FS) when the STM32F4DISCOVERY board and the USB OTG FS peripheral are used

@note In the USB Host HID class, two layouts are defined in the Project_keybd.h file
      and could be used (Azerty and Querty)
        //#define QWERTY_KEYBOARD
        #define AZERTY_KEYBOARD
      The User can eventually add his own layout by editing the HID_KEYBRD_Key array
      in the Project_keybd.c file
    
  
@par Hardware and Software environment 

  - This example has been tested with the following environments:
     - STM32F4DISCOVERY board
     - STM32F4DIS-BB for the Base Board
     - STM32F4DIS-LCD for the LCD module
     - A USB type A (Female) to Micro AB (Male)
     - A mouse or A Keyboard

  - Software development tools
    - EWARM V6.40
    - MDK-ARM V4.60

  - Hardware Set-up
    - Mount STM32F4DISCOVERY board onto STM32F4DIS-BB board through CON1 and CON2
    - Mount STM32F4DIS-LCD module onto STM32F4DIS-BB board through CON3
    - Connect the STM32F4DISCOVERY board to a mouse(or Keyboard) with a 'USB type 
      A (Female) to Micro AB (Male)' cable through USB connector CN5
    - Connect the STM32F4DISCOVERY board to a PC with a 'USB type A to Mini-B' cable 
      through USB connector CN1 to power the board

@par How to use it ?
  In order to load the Project code, you have do the following:
   - EWARM
      - Open the Project.eww workspace
      - Rebuild all files: Project->Rebuild all
      - Load project image: Project->Debug
      - Run program: Debug->Go(F5)


   - MDK-ARM
      - Open the Project.uvproj project
      - Rebuild all files: Project->Rebuild all target files
      - Load project image: Debug->Start/Stop Debug Session
      - Run program: Debug->Run (F5)

    
@note Known Limitations
      - This example retargets the C library printf() function to the EVAL board LCD
        screen (C library I/O redirected to LCD) to display some Library and user debug
        messages.As consequence, when using this toolchain no debug messages will be
        displayed on the LCD (only some control messages in green will be displayed in
        bottom of the LCD). 
    
/********* Portions COPYRIGHT 2012 Embest Tech. Co., Ltd.*****END OF FILE******/
