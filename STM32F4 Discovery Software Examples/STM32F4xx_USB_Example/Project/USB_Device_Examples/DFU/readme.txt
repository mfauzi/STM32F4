/**
  @page USBD_DFU  USB Device DFU example
  
  @verbatim
  ********************Portions COPYRIGHT 2012 STMicroelectronics ***************
  * @file    readme.txt 
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    19-March-2012
  * @brief   Description of the USB Device DFU (Device Firmware Upgrade) example
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
  @page USBD_DFU  USB Device DFU example
  
  @verbatim
  ************** Portions COPYRIGHT 2012 Embest Tech. Co., Ltd.** **************
  * @file    readme.txt 
  * @author  CMP Team
  * @version V1.0.0
  * @date    28-December-2012
  * @brief   Description of the USB Device DFU (Device Firmware Upgrade) example
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

This Demo presents the implementation of a device firmware upgrade (DFU) capability
in the STM32F2xx, STM32F4xx and STM32F105/7 devices. 
It follows the DFU class specification defined by the USB Implementers Forum for 
reprogramming an application through USB-FS-Device. 
The DFU principle is particularly well suited to USB-FS-Device applications that 
need to be reprogrammed in the field.

To have an example of how to generate a DFU image, please refer to the project 
provided in folder "binary_template". This project allows to generate a binary
image (*.bin file) that can be used to generate a *.dfu image using the tool
provided by ST: "DFUse\DFU File Manager" available for download from www.st.com

Once the *.dfu image is generated, it can be downloaded into internal flash memory
using the ST tool "DFUse\DFUse Demonstration" or the tool "DFUse\STDFU Tester" both
available for download from www.st.com


The supported memories for this example are:
 - Internal Flash memory for STM32F2xx, STM32F4xx and STM32F105/7 devices
 - OTP memory for STM32F2xx and STM32F4xx devices.
 
To add a new memory interface you can use the template memory interface provided 
in folder \Libraries\STM32_USB_Device_Library\Class\dfu.

@note After each device reset, hold down the Key user-button on the STM32F4DISCOVERY boards

This example works
  - in full speed (FS) when the STM32F4DISCOVERY board and the USB OTG FS peripheral are used
      
@par Hardware and Software environment 

  - This example has been tested with the following environments:
    - STM32F4DISCOVERY board
    - STM32F4DIS-BB for the Base Board
    - STM32F4DIS-LCD for the LCD module
    - A USB type A (Male) to Micro AB (Male) cable
    - DfuSe_Demo_V3.0.2(The software is located in the folder:\Utilities\PC_Software\DfuSe_Demo_V3.0.2)

  - Software development tools
    - EWARM V6.40
    - MDK-ARM V4.60

  - Hardware Set-up
    - Mount STM32F4DISCOVERY board onto STM32F4DIS-BB board through CON1 and CON2
    - Mount STM32F4DIS-LCD module onto STM32F4DIS-BB board through CON3
    - Connect the STM32F4DISCOVERY board to a PC with a 'USB type A (Male) to Micro AB (Male)' 
      cable through USB connector CN5
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
