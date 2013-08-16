/**
  @page I2C_DataExchangeDMA Two boards I2C Communication: Data Exchange using DMA example
  
  @verbatim
  ******************** (C) COPYRIGHT 2013 STMicroelectronics *******************
  * @file    I2C/I2C_TwoBoards/I2C_DataExchangeDMA/readme.txt 
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    18-January-2013
  * @brief   Description of the I2C Data Exchange between two boards using DMA 
  *          example.
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
  * limitations under the License.
  *
  ******************************************************************************
   @endverbatim

@par Example Description 

This is a typical example of how to use the I2C firmware library to ensure the
steps of an I2C communication between slave Receiver/Transmitter and master 
Transmitter/Receiver using DMA.

- Hardware Description

To use this example, you need to load it on two STM32 boards (let's call them 
Board A and Board B) then connect these two boards through I2C lines and GND.
In the firmware example uncomment the dedicated line in the main.h file to use
the I2C peripheral as STM32 Master device or as STM32 Slave.

@verbatim
*------------------------------------------------------------------------------*
|                BOARD A                                BOARD B                |
|         ____________________                   ____________________          |
|        |                    |                 |                    |         |
|        |                    |                 |                    |         | 
|        |     __________     |                 |     __________     |         |
|        |    |I2C Master|____|_______SDA_______|____|I2C Slave |    |         |
|        |    |  Device  |____|_______SCL_______|____|  Device  |    |         |
|        |    |__________|    |                 |    |__________|    |         |
|        |  O LD1             |                 |  O LD1             |         |
|        |  O LD2     RESET   |                 |  O LD2     RESET   |         |
|        |  O LD3        _    |                 |  O LD3        _    |         |
|        |  O LD4       |_|   |                 |  O LD4       |_|   |         |
|        |                    |                 |                    |         |
|        |             GND O--|-----------------|--O GND             |         |
|        |____________________|                 |____________________|         |
|                                                                              |
|                                                                              |
*------------------------------------------------------------------------------*
@endverbatim

- Software Description

This example requires two boards(Master and Slave)

  - Phase 1:
    In master board I2C peripheral is configured in Master Transmitter with DMA
    where in Slave board I2C peripheral is configured in Slave Receiver with DMA.
    The master will send Data to Slave according to the defined buffer size.
   
  - Phase 2: 
    In master board I2C peripheral is configured in Master Receiver with DMA,
    where in Slave board I2C peripheral is configured in Slave Transmitter with 
    DMA. 
    Once the master has initiated the communication (Start condition), the slave
    will send the defined data in his TxBuffer to the master.

At the end of the data transfer, a data consistency check will be performed 
in master and slave sides. In case of mismatch between sent and received data,
LED2, LED3 and LED4 are OFF. Otherwise LED2, LED3 and LED4 are ON. 
   
@note As the data transfers is ensured by DMA, the TxBuffer size must be equal 
  or greater than two bytes.
 
The File "I2C/I2C_TwoBoards/DataExchangeDMA/main.h" allows different possibilities
of configuration of this example:
  - Addressing mode : 7-bit or 10-bit
  - I2C speed: Fast mode or Standard mode

The SysTick is configured to generate an interrupt each 10ms. A dedicated counter 
inside the SysTick ISR is used to toggle the LED1 each 100ms indicating that the 
firmware is running.

A defined communication timeout is insuring that the application will not remain 
stuck in case of I2C communication is corrupted.
You can adjust this timeout through the USER_TIMEOUT  define inside main.h file
depending on CPU frequency and application conditions (interrupts routines, 
number of data to transfer, speed, CPU frequency...).
When a given operation is not finished within the defined timeout, a callback is 
made to the function "TimeOut_UserCallback()", this function is used to manage
timeout communication failure. In this example, this callback implements a simple 
infinite loop, but user can add his own implementation to manage timeout failure.


@par Directory contents 

  - I2C/I2C_TwoBoards/I2C_DataExchangeDMA/stm32f4xx_conf.h    Library Configuration file
  - I2C/I2C_TwoBoards/I2C_DataExchangeDMA/stm32f4xx_it.c      Interrupt handlers
  - I2C/I2C_TwoBoards/I2C_DataExchangeDMA/stm32f4xx_it.h      Interrupt handlers header file
  - I2C/I2C_TwoBoards/I2C_DataExchangeDMA/main.c              Main program
  - I2C/I2C_TwoBoards/I2C_DataExchangeDMA/main.h              Main Header file
  - I2C/I2C_TwoBoards/I2C_DataExchangeDMA/system_stm32f4xx.c  STM32F4xx system source file
  
@note The "system_stm32f4xx.c" is generated by an automatic clock configuration 
      tool and can be easily customized to your own configuration. 
      To select different clock setup, use the "STM32F4xx_Clock_Configuration_V1.1.0.xls" 
      provided with the AN3988 package available on <a href="http://www.st.com/internet/mcu/family/141.jsp">  ST Microcontrollers </a>


@par Hardware and Software environment  
  - This example runs on STM32F40xx/STM32F41xx, STM32F427x/STM32F437x Devices.
    
  - This example has been tested with STMicroelectronics STM324xG-EVAL (STM32F40xx/
    STM32F41xx Devices) and STM32437I-EVAL (STM32F427x/STM32F437x Devices) evaluation 
    boards and can be easily tailored to any other supported device and development 
    board.

  - STM324xG-EVAL and STM32437I-EVAL Set-up 
    - Connect I2C1 Master SCL pin (PB.06) to I2C1 Slave SCL (PB.06)
    - Connect I2C1 Master SDA pin (PB.09) to I2C1 Slave SDA (PB.09)

   @note  
    - Pull-up resistors should be connected to I2C SCL and SDA Pins in case of 
      other boards use.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Copy all source files from this example folder to the template folder under
   Project\STM32F4xx_StdPeriph_Template
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
   
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */


