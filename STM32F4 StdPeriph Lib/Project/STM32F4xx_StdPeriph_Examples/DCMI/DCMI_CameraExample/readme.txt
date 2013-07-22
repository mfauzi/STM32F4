/**
  @page DCMI_CameraExample Camera example Readme file
  
  @verbatim
  ******************** (C) COPYRIGHT 2013 STMicroelectronics *******************
  * @file    DCMI/DCMI_CameraExample/readme.txt 
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    18-January-2013
  * @brief   Description of the DCMI Camera example.
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

This example shows how to use the DCMI to control the OV9655 or OV2640 Camera module
mounted on STM324xG-EVAL or STM32437I-EVAL evaluation boards. 

Both cameras have an image array capable of operating at up to 15 frames per second 
in SXGA resolution with complete user control over image quality, formatting and
output data transfer. All required image processing functions are also programmable
through the SCCB interface (I2C like protocol).

In this example the DCMI is configured to interface with this 8 bit data camera
in continuous mode. The I2C1 is used to configure the Camera in 8 bit RGB 5:6:5 mode. 
The user can select between two resolutions QQVGA(160x120) or QVGA(320x240) in order 
to display the captured image on the LCD(320x240), this selection is performed 
at application run time using the Joystick UP, DOWN and SEL buttons.
All camera data received by the DCMI are transferred through the DMA and displayed
on the LCD (connected to FSMC). As a result the CPU is free to execute other tasks.

You can use the potentiometer RV1 to fine tune on the fly the camera's brightness.
       

@par Directory contents

    - DCMI/DCMI_CameraExample/main.h               Main config file
    - DCMI/DCMI_CameraExample/camera_api.h         Header for camera_api.c
    - DCMI/DCMI_CameraExample/dcmi_ov2640.h        Header for dcmi_ov2640.c
    - DCMI/DCMI_CameraExample/dcmi_ov9655.h        Header for dcmi_ov9655.c
    - DCMI/DCMI_CameraExample/lcd_lo_conf.h        Lcd_log configuration file
    - DCMI/DCMI_CameraExample/stm32f4xx_it.h       Header for stm32f4xx_it.c
    - DCMI/DCMI_CameraExample/stm32f4xx_conf.h     library configuration file 
    - DCMI/DCMI_CameraExample/main.c               Main program  
    - DCMI/DCMI_CameraExample/camera_api.c         High level driver for OV9655/OV2640 Cameras
    - DCMI/DCMI_CameraExample/dcmi_ov2640.c        Driver for OV2640 Camera module
    - DCMI/DCMI_CameraExample/dcmi_ov9655.c        Driver for OV9655 Camera module
    - DCMI/DCMI_CameraExample/stm32f4xx_it.c       Interrupt handlers
    - DCMI/DCMI_CameraExample/system_stm32f4xx.c   STM32F4xx system clock configuration file


@par Hardware and Software environment  
  - This example runs on STM32F40xx/STM32F41xx, STM32F427x/STM32F437x Devices.
    
  - This example has been tested with STMicroelectronics STM324xG-EVAL (STM32F40xx/
    STM32F41xx Devices) and STM32437I-EVAL (STM32F427x/STM32F437x Devices) evaluation 
    boards and can be easily tailored to any other supported device and development 
    board.


@par How to use it ?
In order to make the program work, you must do the following :
 - Copy all source files from this example folder to the template folder under
   Project\STM32F4xx_StdPeriph_Templates
 - Open your preferred toolchain
 - Add the following files the project source list
     - camera_api.c
     - dcmi_ov2640.c
     - dcmi_ov9655.c
     - Utilities\STM32_EVAL\Common\lcd_log.c
 - Rebuild all files and load your image into target memory
 - Run the example

  
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
