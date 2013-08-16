/**
  ******************************************************************************
  * @file    I2S/I2S_TwoBoards/I2S_DataExchangePolling/main.h
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
#if defined (USE_STM324xG_EVAL)
  #include "stm324xg_eval.h"

#elif defined (USE_STM324x7I_EVAL) 
  #include "stm324x7i_eval.h"

#else
 #error "Please select first the Evaluation board used in your application (in Project Options)"
#endif


/* Exported typedef -----------------------------------------------------------*/
#define countof(a)   (sizeof(a) / sizeof(*(a)))
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* Exported define ------------------------------------------------------------*/
/* Uncomment the line below if you will use the I2S peripheral as a Master */
#define I2S_MASTER 
/* Uncomment the line below if you will use the I2S peripheral as a Slave */
/* #define I2S_SLAVE */

/* I2Sx Communication boards Interface */
#define I2Sx                           SPI2
#define I2Sxext                        I2S2ext
#define I2Sx_CLK                       RCC_APB1Periph_SPI2
#define I2Sx_CLK_INIT                  RCC_APB1PeriphClockCmd

#define I2Sx_WS_PIN                    GPIO_Pin_12
#define I2Sx_WS_GPIO_PORT              GPIOB
#define I2Sx_WS_GPIO_CLK               RCC_AHB1Periph_GPIOB
#define I2Sx_WS_SOURCE                 GPIO_PinSource12
#define I2Sx_WS_AF                     GPIO_AF_SPI2

#define I2Sx_CK_PIN                    GPIO_Pin_13
#define I2Sx_CK_GPIO_PORT              GPIOB
#define I2Sx_CK_GPIO_CLK               RCC_AHB1Periph_GPIOB
#define I2Sx_CK_SOURCE                 GPIO_PinSource13
#define I2Sx_CK_AF                     GPIO_AF_SPI2

#define I2Sx_SD_PIN                    GPIO_Pin_3
#define I2Sx_SD_GPIO_PORT              GPIOC
#define I2Sx_SD_GPIO_CLK               RCC_AHB1Periph_GPIOC
#define I2Sx_SD_SOURCE                 GPIO_PinSource3
#define I2Sx_SD_AF                     GPIO_AF_SPI2

#define I2Sxext_SD_PIN                 GPIO_Pin_2
#define I2Sxext_SD_GPIO_PORT           GPIOI
#define I2Sxext_SD_GPIO_CLK            RCC_AHB1Periph_GPIOI
#define I2Sxext_SD_SOURCE              GPIO_PinSource2
#define I2Sxext_SD_AF                  GPIO_AF_SPI3

#define TX_MASTER_BUFFERSIZE           (countof(aTxMasterBuffer) - 1)
#define RX_MASTER_BUFFERSIZE           TX_MASTER_BUFFERSIZE

#define TX_SLAVE_BUFFERSIZE           (countof(aTxSlaveBuffer) - 1)
#define RX_SLAVE_BUFFERSIZE           TX_SLAVE_BUFFERSIZE

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
