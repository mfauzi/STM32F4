/**
  ******************************************************************************
  * @file    SPI/SPI_TwoBoards/SPI_DataExchangeInterrupt/main.h
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    18-January-2013
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
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
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

#if defined (USE_STM324xG_EVAL)
  #include "stm324xg_eval.h"

#elif defined (USE_STM324x7I_EVAL) 
  #include "stm324x7i_eval.h"

#elif defined (USE_STM324x9I_EVAL) 
  #include "stm324x9i_eval.h"
   
#else
 #error "Please select first the Evaluation board used in your application (in Project Options)"
#endif

/* Exported typedef -----------------------------------------------------------*/
#define countof(a)   (sizeof(a) / sizeof(*(a)))
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* Exported define ------------------------------------------------------------*/
/* Uncomment the line below if you will use the SPI peripheral as a Master */
/* #define SPI_MASTER */
/* Uncomment the line below if you will use the SPI peripheral as a Slave */
 #define SPI_SLAVE  

/* USER_TIMEOUT value for waiting loops. This timeout is just guarantee that the
   application will not remain stuck if the USART communication is corrupted. 
   You may modify this timeout value depending on CPU frequency and application
   conditions (interrupts routines, number of data to transfer, baudrate, CPU
   frequency...). */ 
#define USER_TIMEOUT                    ((uint32_t)0x64) /* Waiting 1s */

/* SPIx Communication boards Interface */
#if defined (USE_STM324xG_EVAL)
  #define SPIx                           SPI2
  #define SPIx_CLK                       RCC_APB1Periph_SPI2
  #define SPIx_CLK_INIT                  RCC_APB1PeriphClockCmd
  #define SPIx_IRQn                      SPI2_IRQn
  #define SPIx_IRQHANDLER                SPI2_IRQHandler

  #define SPIx_SCK_PIN                   GPIO_Pin_1
  #define SPIx_SCK_GPIO_PORT             GPIOI
  #define SPIx_SCK_GPIO_CLK              RCC_AHB1Periph_GPIOI
  #define SPIx_SCK_SOURCE                GPIO_PinSource1
  #define SPIx_SCK_AF                    GPIO_AF_SPI2

  #define SPIx_MISO_PIN                  GPIO_Pin_2
  #define SPIx_MISO_GPIO_PORT            GPIOI
  #define SPIx_MISO_GPIO_CLK             RCC_AHB1Periph_GPIOI
  #define SPIx_MISO_SOURCE               GPIO_PinSource2
  #define SPIx_MISO_AF                   GPIO_AF_SPI2

  #define SPIx_MOSI_PIN                  GPIO_Pin_3
  #define SPIx_MOSI_GPIO_PORT            GPIOI
  #define SPIx_MOSI_GPIO_CLK             RCC_AHB1Periph_GPIOI
  #define SPIx_MOSI_SOURCE               GPIO_PinSource3
  #define SPIx_MOSI_AF                   GPIO_AF_SPI2
	
#endif  /* USE_STM324xG_EVAL */	

#if defined (USE_STM324x7I_EVAL)
  #define SPIx                           SPI2
  #define SPIx_CLK                       RCC_APB1Periph_SPI2
  #define SPIx_CLK_INIT                  RCC_APB1PeriphClockCmd
  #define SPIx_IRQn                      SPI2_IRQn
  #define SPIx_IRQHANDLER                SPI2_IRQHandler

  #define SPIx_SCK_PIN                   GPIO_Pin_1
  #define SPIx_SCK_GPIO_PORT             GPIOI
  #define SPIx_SCK_GPIO_CLK              RCC_AHB1Periph_GPIOI
  #define SPIx_SCK_SOURCE                GPIO_PinSource1
  #define SPIx_SCK_AF                    GPIO_AF_SPI2

  #define SPIx_MISO_PIN                  GPIO_Pin_2
  #define SPIx_MISO_GPIO_PORT            GPIOI
  #define SPIx_MISO_GPIO_CLK             RCC_AHB1Periph_GPIOI
  #define SPIx_MISO_SOURCE               GPIO_PinSource2
  #define SPIx_MISO_AF                   GPIO_AF_SPI2

  #define SPIx_MOSI_PIN                  GPIO_Pin_3
  #define SPIx_MOSI_GPIO_PORT            GPIOI
  #define SPIx_MOSI_GPIO_CLK             RCC_AHB1Periph_GPIOI
  #define SPIx_MOSI_SOURCE               GPIO_PinSource3
  #define SPIx_MOSI_AF                   GPIO_AF_SPI2
  
#endif  /* USE_STM324x7I_EVAL */

#define BUFFERSIZE                       100



/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
