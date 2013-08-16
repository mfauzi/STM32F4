/**
  @page USBD_VCP  USB Device VCP example
  
  @verbatim
  ***************** Portions COPYRIGHT 2012 STMicroelectronics *****************
  * @file    readme.txt 
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    19-March-2012
  * @brief   Description of the USB Device VCP (Virtual Com Port) example
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
  @page USBD_VCP  USB Device VCP example
  
  @verbatim
  ************** Portions COPYRIGHT 2012 Embest Tech. Co., Ltd.** **************
  * @file    readme.txt 
  * @author  CMP Team
  * @version V1.0.0
  * @date    28-December-2012
  * @brief   Description of the USB Device VCP (Virtual Com Port) example
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

This Demo presents the implementation of a Virtual Com Port (VCP) capability in 
the STM32F2xx, STM32F4xx and STM32F105/7 devices. 

It illustrates an implementation of the CDC class following the PSTN subprotocol.
The VCP example allows the STM32 device to behave as a USB-to-RS232 bridge.
 - On one side, the STM32 communicates with host (PC) through USB interface in Device mode.
 - On the other side, the STM32 communicates with other devices (same host, other host,
   other devices? through the USART interface (RS232).

The support of the VCP interface is managed through the ST Virtual Com Port driver
available for download from www.st.com.
This example can be customized to communicate with interfaces other than USART.
The VCP example works in High and Full speed modes.

When the VCP application starts, the USB device is enumerated as serial communication
port and can be configured in the same way (baudrate, data format, parity, stop bit
length?.
To test this example, you can use one of the following configurations:
 - Configuration 1: Connect USB cable to host and USART (RS232) to a different host
   (PC or other device) or to the same host. In this case, you can open two hyperterminal-like
   terminals to send/receive data to/from host to/from device.
 - Configuration 2: Connect USB cable to Host and connect USART TX pin to USART
   RX pin on the evaluation board (Loopback mode). In this case, you can open one
   terminal (relative to USB com port or USART com port) and all data sent from this
   terminal will be received by the same terminal in loopback mode. This mode is useful
   for test and performance measurements.

To use a different CDC communication interface you can use the template CDC 
interface provided in folder \Libraries\STM32_USB_Device_Library\Class\cdc.

@note When transferring a big file (USB OUT transfer) user have to adapt the size
      of IN buffer, for more details refer to usbd_conf.h file (APP_RX_DATA_SIZE constant).


This example works
   - in full speed (FS) when the STM32F4DISCOVERY board and the USB OTG FS peripheral are used

@par Hardware and Software environment 

  - This example has been tested with the following environments:
     - STM32F4DISCOVERY board
     - STM32F4DIS-BB for the Base Board
     - STM32F4DIS-LCD for the LCD module
     - A USB type A (Male) to Micro AB (Male) 
     - VCP_V1.3.1_Setup(The software is located in the folder:\Utilities\PC_Software\stm32_vcp)

  - Software development tools
    - EWARM V6.40
    - MDK-ARM V4.60

  - Hardware Set-up
    - Mount STM32F4DISCOVERY board onto STM32F4DIS-BB board through CON1 and CON2
    - Mount STM32F4DIS-LCD module onto STM32F4DIS-BB board through CON3
    - Connect the STM32F4DISCOVERY board to a PC with a 'USB type A (Male) to Micro 
      AB (Male)' cable through USB connector CN5
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
