/**
  @page USBH_MSC  USB Host MSC example
  
  @verbatim
  ****************** Portions COPYRIGHT 2012 STMicroelectronics ****************
  * @file    readme.txt 
  * @author  MCD Application Team
  * @version V2.1.0
  * @date    19-March-2012
  * @brief   Description of the USB Host MSC example
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
  @page USBH_MSC  USB Host MSC example
  
  @verbatim
  ************** Portions COPYRIGHT 2012 Embest Tech. Co., Ltd.** **************
  * @file    readme.txt 
  * @author  CMP Team
  * @version V1.0.0
  * @date    28-December-2012
  * @brief   Description of the USB Host MSC example
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

The STM32 behave as a mass storage Host that can enumerate, show content and display
the supported BMP image in the attached USB flash disk.
When the user press the User Button, the application explore the USB flash
disk content and display it on the LCD. The user has to press the User Button
to display the whole disk flash (recursion level 2).
Once the entire disk flash content is explored, the user has to press the User
Button to write a small file (less to 1 KB) on the disk.
After writing the file to the disk, the user can press the User Button and start 
the Image slide show (BMP file located in the USB Disk root).
Once all the images are displayed, the application explores again the disk flash.

 @note Only the BMP files with the following format are supported : 
         - width  : 320
         - Height : 240
         - BPP    :  16
         - Compression : RGB bitmap with RGB masks  
       To use the Image browser, copy the BMP files from "\Utilities\Binary\Media"
       to the root of the USB flash disk to be used. 
       
This example works
  - in full speed (FS) when the STM32F4DISCOVERY board and the USB OTG FS peripheral are used

@par Hardware and Software environment 

  - This example has been tested with the following environments:
     - STM32F4DISCOVERY board
     - STM32F4DIS-BB for the Base Board
     - STM32F4DIS-LCD for the LCD module
     - A USB type A (Female) to Micro AB (Male) cable
     - USB Flash Disk
     @Note:Kingston  1GB/2GB  and  SanDisk  4GB  USB  Flash  Disk  have  been  tested. It's 
           not guaranteed that all kind of USB Flash Disk work well on the board. 

   - Software development tools
    - EWARM V6.40
    - MDK-ARM V4.60
 
  - Hardware Set-up
    - Mount STM32F4DISCOVERY board onto STM32F4DIS-BB board through CON1 and CON2
    - Mount STM32F4DIS-LCD module onto STM32F4DIS-BB board through CON3
    - Connect the STM32F4DISCOVERY board to the USB flash disk with a 'USB type 
      A (Female) to Micro AB (Male)' cable through USB connector CN5
    - Connect the STM32F4DISCOVERY board to a PC with a 'USB type A to Mini-B' cable 
      through USB connector CN1 to power the board.



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
