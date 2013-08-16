/**
  @page USBD_MSC  USB Device Mass Storage example
  
  @verbatim
  **************** Portions COPYRIGHT 2012 STMicroelectronics ******************
  * @file    readme.txt 
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    19-March-2012
  * @brief   Description of the USB Device Mass Storage example
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
  @page USBD_MSC  USB Device Mass Storage example
  
  @verbatim
  ************** Portions COPYRIGHT 2012 Embest Tech. Co., Ltd.** **************
  * @file    readme.txt 
  * @author  CMP Team
  * @version V1.0.0
  * @date    28-December-2012
  * @brief   Description of the USB Device Mass Storage example
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

The Mass Storage example gives a typical example of how to use the STM32F2xx, STM32F4xx
and STM32F105/7 USB OTG Device peripheral to communicate with a PC Host using the bulk
transfer while the microSD card is used as storage media.
This example supports the BOT (bulk only transfer) protocol and all needed SCSI
(small computer system interface) commands, and is compatible with both Windows
XP (SP1/SP2/SP3) and Windows 2000 (SP4).
After running the example, the user just has to plug the USB cable into a PC host
and the device is automatically detected. A new removable drive appears in the
system window and write/read/format operations can be performed as with any other
removable drive.

@note The USB Device library uses the multi-packet feature, this functionality increase 
        the transfer performance when using big amount of data. In   the Mass storage demo, 
        an intermediary buffer is used to receive/send data from/to USB  and used also to 
        write/read from the physical media (microSD). The bigger is the buffer, the faster
        is the IO operations in USB and also in media; in order to use a balanced buffer size
        with the project footprint; the buffer size was defined in the usbd_conf.h file to 4K,
        but it can be modified by the user from 512 Bytes to 64KBytes.
        
This example works
  - in full speed (FS) when the STM32F4DISCOVERY board and the USB OTG FS peripheral are used.

@par Hardware and Software environment 

  - This example has been tested with the following environments:
     - STM32F4DISCOVERY board
     - STM32F4DIS-BB for the Base Board
     - STM32F4DIS-LCD for the LCD module
     - USB A type (Male) to Micro AB (Male) cable
     - MicroSD card
     
     @Note : Kingston 1GB/2GB and SanDisk 2GB MicroSD Card have been tested . It¡¯s not 
             guaranteed that all kind of MicroSD card work well on the board

  - Software development tools
    - EWARM V6.40
    - MDK-ARM V4.60

  - Hardware Set-up
    - Mount STM32F4DISCOVERY board onto STM32F4DIS-BB board through CON1 and CON2
    - Mount STM32F4DIS-LCD module onto STM32F4DIS-BB board through CON3
    - Insert the MicroSD card into MicroSD slot CON6
    - Connect the STM32F4DISCOVERY board to a PC with a 'USB type A (Male) to Micro AB (Male)' 
      cable through USB connector CN5
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
