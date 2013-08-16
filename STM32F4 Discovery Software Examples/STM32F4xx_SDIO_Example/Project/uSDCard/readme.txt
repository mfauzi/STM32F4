/**
  @page SDIO_uSDCard  How to use the SDIO to drive an SD card memory
  
  @verbatim
  *************Portions COPYRIGHT 2011 STMicroelectronics **********************
  * @file    uSDCard/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    30-September-2011
  * @brief   Description of the SDIO Example.
  ******************************************************************************
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  ******************************************************************************
  @endverbatim
  */
/**
  @page SDIO_uSDCard  How to use the SDIO to drive an SD card memory
 
  @verbatim
  ************** Portions COPYRIGHT 2012 Embest Tech. Co., Ltd.** **************
  * @file    uSDCard/readme.txt 
  * @author  CMP Team
  * @version V1.0.0
  * @date    28-December-2012
  * @brief   Description of the SDIO Example.
  *          Modified to support the STM32F4DISCOVERY and STM32F4DIS-BB modules. 
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

This example provides a basic example of how to use the SDIO firmware library and
an associate driver to perform read/write operations on the SD Card memory (SD Card 
V1.0, V1.1, V2.0 and SDHC (High Capacity) protocol)that could be mounted on the 
STM32F4DIS-BB board.

The example provides different SD Card transfer states and operations and here
below a description of each step:
  - First the SDIO peripheral and SD Card are initialized using the SD_Init()
    function: 
            - Configure the SDIO according to the desired SDIO_CK clock frequency.
            - Reset the SD Card
            - Identify the SD Card
            - Initializes the SD Card
            - Get the SD Card Info
            - Select the SD Card
            - Enable the Wide Bus mode (4-bit data)

  - SD Card Erase Operation
            - Starts an Erase operation: Erase the corresponding blocks using 
              the SD_Erase() function 
            - Read the Erased blocks using the SD_ReadMultiBlocks() function
            - Test if the corresponding Blocks are well erased: check if the 
              EraseStatus variable is equal to PASSED. If the erase operation is
              passed LED1 is turned ON.
            
  - SD Card Single Block Operation
            - Starts a Write operation: Write a single Block using the SD_WriteBlock()
              function 
            - Read a single Block using the SD_ReadBlock() function
            - Compare the written Block and the read one: check if the TransferStatus1 
              variable is equal to PASSED. If the Single Block Read/write operation
              is passed LED2 is turned ON.

  - SD Card Multiple Block Operation
            - Starts a Multiple Write operation: Write a multi Blocks using the 
              SD_WriteMultiBlocks() function. 
            - Read a multiple Blocks using the SD_ReadMultiBlocks() function
            - Compare the written Blocks and the read one: check if the TransferStatus2 
              variable is equal to PASSED. If the Multiple Blocks Read/Write operation
              is passed LED3 is turned ON.

All data transfers are made by DMA.
At each operation, the SD Card presence and status is checked using the SD_GetStatus()
function and a global variable "Status" storing the results of the last operation.

SD Card Operations and Written/Read data correctness is signaled  by LED 
lightening and though as follow:
   - If the Erase operation is PASSED then LED3 ON else the LED6 is ON and LED3 is OFF 
   - If the Single Block Write/Read operation is PASSED then LED4 ON else the LED6 is ON and LED4 is OFF
   - If the Multi Blocks Write/Read operation is PASSED then LED5 ON else the LED6 is ON and LED5 is OFF
   - Any SD Card operation including the SD Initialization error is signaled by
     LED6 ON.

@par Directory contents 

  - "inc": contains the project firmware header files    
    - inc/stm32f4xx_conf.h     Library Configuration file
    - inc/stm32f4xx_it.h       Interrupt handlers header file

  - "src": contains the project firmware source files
    - src/system_stm32f4xx.c   STM32F4xx system clock configuration file
    - src/stm32f4xx_it.c       Interrupt handlers
    - src/main.c               Main program

  
@note The "system_stm32f4xx.c" is generated by an automatic clock configuration 
      tool and can be easily customized to your own configuration. 
      To select different clock setup, use the "STM32F4xx_Clock_Configuration_V1.0.0.xls" 
      provided with the AN3988 package available on <a href="http://www.st.com/internet/mcu/family/141.jsp">  ST Microcontrollers </a>

      
@par Hardware and Software environment 

  - This example has been tested with the following environment: 
     - STM32F4DISCOVERY board
     - STM32F4DIS-BB for the Base Board
     - TF card (2GB or less than 2GB Kingston or SanDisk TF Card is recommended. 
       It should be formatted before using)
	   
  - Software development tools
    - EWARM V6.40
    - MDK-ARM V4.60
	
  - Hardware Set-up 
    - Mount STM32F4DISCOVERY board onto STM32F4DIS-BB board through CON1 and CON2
    - Insert a TF card into microSD slot CON6
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

/*********** Portions COPYRIGHT 2012 Embest Tech. Co., Ltd.*****END OF FILE****/

