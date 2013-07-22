/**
  @page SPI_DataExchangeInterrupt SPI Communication Boards Data Exchange using interrupt example
  
  @verbatim
  ******************** (C) COPYRIGHT 2013 STMicroelectronics *******************
  * @file    SPI/SPI_TwoBoards/SPI_DataExchangeInterrupt/readme.txt 
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    18-January-2013
  * @brief   Description of the SPI Communication Boards Data Exchange using 
  *          interrupt example.
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

This example provides a small application in which joystick buttons are used 
to trigger SPI communication using interrupts and though using SPI firmware 
library.

- Hardware Description

To use this example, you need to load it on two STM32 boards (let's call them 
BoardA and BoardB) then connect these two boards through SPI lines and GND.
In the firmware example uncomment the dedicated line in the main.h file to use
the SPI peripheral as STM32 Master device or as STM32 Slave .

@verbatim
*------------------------------------------------------------------------------*
|                BoardA                                 BoardB                 |
|         ____________________                   ____________________          |
|        |                    |                 |                    |         |
|        |                    |                 |                    |         | 
|        |     __________     |                 |     __________     |         |
|        |    |SPI Master|____|_______SCK_______|____|SPI Slave |    |         |
|        |    |  Device  |____|_______MOSI______|____|  Device  |    |         |
|        |    |__________|____|_______MISO______|____|__________|    |         |
|        |                    |                 |                    |         |
|        |  O LD1             |                 |  O LD1             |         |
|        |  O LD2    Tamper   |                 |  O LD2    Tamper   |         |
|        |  O LD3        _    |                 |  O LD3        _    |         |
|        |  O LD4       |_|   |                 |  O LD4       |_|   |         |
|        |                    |                 |                    |         |
|        |             GND O--|-----------------|--O GND             |         |
|        |____________________|                 |____________________|         |
|                                                                              |
|                                                                              |
*------------------------------------------------------------------------------*
@endverbatim

@note The connection between the pins should use a short wires and a common Ground.

- Software Description
In master board, SPI peripheral is configured as Master full duplex with Interrupt,
whereas in Slave board, SPI peripheral is configured as Slave Full duplex with Interrupt. 

At the end of the data transfer, a data consistency check will be performed 
in master and slave sides. In case of mismatch between sent and received data,
LED2 and LED4 are OFF. Otherwise LED1 and LED3 are ON.

In both boards, the data transfers is managed using SPIx_IRQHandler in stm32f4xx_it.c 
file.
The SysTick is configured to generate interrupt each 10ms. A dedicated counter 
inside the SysTick ISR is used to toggle LD1 each 100ms indicating that the 
firmware is running.
These operations can be repeated infinitely.


A defined communication timeout is insuring that the application will not remain
stuck if the SPI communication is corrupted.
You can adjust this timeout through the USER_TIMEOUT define inside main.h file
depending on CPU frequency and application conditions (interrupts routines, 
number of data to transfer, baudrate, CPU frequency...).
When a given operation is not finished within the defined timeout, a callback is 
made to the function "TimeOut_UserCallback()", this function is used to manage
timeout communication failure. In this example, this callback implements a simple 
infinite loop, but user can add his own implementation to manage timeout failure.


@par Directory contents 

  - SPI/SPI_TwoBoards/SPI_DataExchangeInterrupt/stm32f4xx_conf.h    Library Configuration file
  - SPI/SPI_TwoBoards/SPI_DataExchangeInterrupt/stm32f4xx_it.c      Interrupt handlers
  - SPI/SPI_TwoBoards/SPI_DataExchangeInterrupt/stm32f4xx_it.h      Interrupt handlers header file
  - SPI/SPI_TwoBoards/SPI_DataExchangeInterrupt/main.c              Main program
  - SPI/SPI_TwoBoards/SPI_DataExchangeInterrupt/main.h              Main program header file
  - SPI/SPI_TwoBoards/SPI_DataExchangeInterrupt/system_stm32f4xx.c  STM32F4xx system source file
  
@note The "system_stm32f4xx.c" is generated by an automatic clock configuration 
      tool and can be easily customized to your own configuration. 
      To select different clock setup, use the "STM32F4xx_Clock_Configuration_V1.1.0.xls" tool. 

 
@par Hardware and Software environment  
  - This example runs on STM32F40xx/STM32F41xx, STM32F427x/STM32F437x Devices.
    
  - This example has been tested with STMicroelectronics STM324xG-EVAL (STM32F40xx/
    STM32F41xx Devices) and STM32437I-EVAL (STM32F427x/STM32F437x Devices) evaluation 
    boards and can be easily tailored to any other supported device and development 
    board.

  - STM324xG-EVAL and STM32437I-EVAL Set-up 
    - Use LED1, LED2, LED3 and LED4 connected respectively to PG.06, PG.08, PI.09
      and PC.07 pins
    - Use the Joystick Push Button 
    - Connect BoardA SPI2 SCK pin  (PI1) to BoardB SPI2 SCK pin  (PI1)
    - Connect BoardA SPI2 MOSI pin (PI3) to BoardB SPI2 MOSI pin (PI3)
    - Connect BoardA SPI2 MISO pin (PI2) to BoardB SPI2 MOSI pin (PI2)

@par How to use it ? 

In order to make the program work, you must do the following :
 - Copy all source files from this example folder to the template folder under
   Project\STM32F4xx_StdPeriph_Templates
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

    
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
