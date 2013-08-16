/**
  @page http server socket demonstration Readme file
 
  @verbatim
  ***************** Portions COPYRIGHT 2011 STMicroelectronics *****************
  * @file    httpserver socket/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    31-October-2011
  * @brief   Description of the STM32F4x7 http server socket demonstration.
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
  @page http server socket demonstration Readme file
 
  @verbatim
  ******************* Portions COPYRIGHT 2012 Embest Tech. Co., Ltd.** *********
  * @file    readme.txt 
  * @author  CMP Team
  * @version V1.0.0
  * @date    28-December-2012
  * @brief   Description of the STM32F4x7 http server socket demonstration.
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
   
@par Description

This directory contains a set of sources files that implement a web server
application, based on the socket API, for STM32F4x7 devices.

Please note that for http server socket demonstration, LwIP v1.3.2 is used a 
the TCP/IP stack and FreeRTOS v6.1.0 is used as the Real Time Kernel.

@par Project Directory contents

 - "inc": contains the demonstration firmware header files
    - inc/main.h               main config file
    - inc/stm32f4x7_eth_bsp.h  header for stm32f4x7_eth_bsp.c
    - inc/netconf.h            header for netconf.c
    - inc/lwipopts.h           LwIP stack configuration options
    - inc/FreeRTOSConfig.h     FreeRTOS configuration options
    - inc/fsdata.h             header for fsdata.c
    - inc/stm32f4xx_conf.h     Library Configuration file
    - inc/stm32f4x7_eth_conf.h STM32 Ethernet driver Configuration file
    - inc/stm32f4xx_it.h       Header for stm32f4xx_it.c
    - inc/fs.h                 header for fs.c
    - inc/httpserver-socket.h header for httpserver-socket.c
    - inc/serial_debug.h       header for serial_debug.c

  - "src": contains the demonstration firmware source files
    - src/main.c               main program file
    - src/stm32f4x7_eth_bsp.c  STM32F4x7 Ethernet hardware configuration
    - src/netconf.c            LwIP stack initializations
    - src/fsdata.c             ROM filesystem data (html pages)
    - src/system_stm32f4xx.c   STM32 system clock configuration file
    - src/stm32f4xx_it.c       STM32 Interrupt handlers
    - src/httpserver-socket.c httpserver socket main task
    - src/fs.c                 file system functions
    - src/serial_debug.c       retarget the printf function to the USART

@par Hardware and Software environment
  
  - This example has been tested with the following environments:
    - STM32F4DISCOVERY board
    - STM32F4DIS-BB for the Base Board
    - STM32F4DIS-LCD for the LCD module
    - Http clients: Firefox Mozilla (v3.6 and later) or Microsoft Internet Explorer (v8 and later)
    - DHCP server: PC utility TFTPD32 (http://tftpd32.jounin.net/) is used as a DHCP server

  - Software development tools
    - EWARM V6.40
    - MDK-ARM V4.60

  - Hardware Set-up
    - Mount STM32F4DISCOVERY board onto STM32F4DIS-BB board through CON1 and CON2
    - Mount STM32F4DIS-LCD module onto STM32F4DIS-BB board through CON3
    - Connect the STM32F4DIS-BB board to a PC with a crossover Ethernet cable through RJ45 connector J1
    - Connect the STM32F4DISCOVERY board to a PC with a 'USB type A to Mini-B' cable 
      through USB connector CN1 to power the board.



@par How to use it ?

In order to make the program work, you must do the following:
  1. Load the demonstration code in the STM32F4x7 Flash memory (see below)
  2. Refer to "AN3966 LwIP TCP/IP stack demonstration for STM32F4x7xx microcontrollers"
     to know how to use the demonstration

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
