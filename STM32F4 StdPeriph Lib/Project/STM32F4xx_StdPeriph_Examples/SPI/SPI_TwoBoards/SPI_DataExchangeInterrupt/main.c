/**
  ******************************************************************************
  * @file    SPI/SPI_TwoBoards/SPI_DataExchangeInterrupt/main.c
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32F4xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup SPI_DataExchangeInterrupt
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
    uint8_t aTxBuffer[BUFFERSIZE] = "SPI Master/Slave : Communication between two SPI using Interrupts";
__IO uint8_t aRxBuffer [BUFFERSIZE];
__IO uint8_t ubRxIndex = 0;
__IO uint8_t ubTxIndex = 0;
__IO uint32_t TimeOut = 0;

SPI_InitTypeDef  SPI_InitStructure;

/* Private function prototypes -----------------------------------------------*/
static void SPI_Config(void);
static void SysTickConfig(void);
static TestStatus Buffercmp(uint8_t* pBuffer1, __IO uint8_t* pBuffer2, uint16_t BufferLength);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       files (startup_stm32f40xx.s/startup_stm32f427x.s) before to branch to 
       application main. 
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f4xx.c file
     */
  
  /* SPI configuration */
  SPI_Config();
  
  /* SysTick configuration */
  SysTickConfig();
  
  /* LEDs configuration */
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);
  
#ifdef SPI_MASTER
  /* Master board configuration */    
  /* Initializes the SPI communication */
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_Init(SPIx, &SPI_InitStructure);
  
  /* The Data transfer is performed in the SPI interrupt routine */
  /* Configure the Tamper Button */
  STM_EVAL_PBInit(BUTTON_TAMPER,BUTTON_MODE_GPIO);
  
  /* Wait until Tamper Button is pressed */
  while (STM_EVAL_PBGetState(BUTTON_TAMPER));
  
  /* Enable the SPI peripheral */
  SPI_Cmd(SPIx, ENABLE);
  
  /* Initialize Buffer counters */
  ubTxIndex = 0;
  ubRxIndex = 0;
  
  /* Enable the Rx buffer not empty interrupt */
  SPI_I2S_ITConfig(SPIx, SPI_I2S_IT_RXNE, ENABLE);
  
  /* Enable the Tx buffer empty interrupt */
  SPI_I2S_ITConfig(SPIx, SPI_I2S_IT_TXE, ENABLE);
  
#endif /* SPI_MASTER */
  
#ifdef SPI_SLAVE
  /* Slave board configuration */
  /* Initializes the SPI communication */
  SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;
  SPI_Init(SPIx, &SPI_InitStructure);
  
  /* The Data transfer is performed in the SPI interrupt routine */
  /* Initialize Buffer counters */
  ubTxIndex = 0;
  ubRxIndex = 0;
  
  /* Enable the Rx buffer not empty interrupt */
  SPI_I2S_ITConfig(SPIx, SPI_I2S_IT_RXNE, ENABLE);
  
  /* Enable the Tx empty interrupt */
  SPI_I2S_ITConfig(SPIx, SPI_I2S_IT_TXE, ENABLE);
  
  /* Enable the SPI peripheral */
  SPI_Cmd(SPIx, ENABLE);
  
#endif /* SPI_SLAVE */
  
  /* Waiting the end of Data transfer */
  while ((ubTxIndex < BUFFERSIZE) && (ubRxIndex < BUFFERSIZE))
  {
  }
  
  /* Disable the Rx buffer not empty interrupt */
  SPI_I2S_ITConfig(SPIx, SPI_I2S_IT_RXNE, DISABLE);
  
  /* Disable the Tx empty interrupt */
  SPI_I2S_ITConfig(SPIx, SPI_I2S_IT_TXE, DISABLE);
  
  /* Disable the SPI peripheral */
  SPI_Cmd(SPIx, DISABLE);
  
  if (Buffercmp(aTxBuffer, aRxBuffer, BUFFERSIZE) != FAILED) 
  {
    /* Turn ON LED1 and LED3 */
    STM_EVAL_LEDOn(LED1);
    STM_EVAL_LEDOn(LED3);
    /* Turn OFF LED2 and LED4 */
    STM_EVAL_LEDOff(LED2);
    STM_EVAL_LEDOff(LED4);
  }
  else 
  {
    /* Turn OFF LED1 and LED3 */
    STM_EVAL_LEDOff(LED1);
    STM_EVAL_LEDOff(LED3);
    /* Turn ON LED2 and LED4 */
    STM_EVAL_LEDOn(LED2);
    STM_EVAL_LEDOn(LED4);    
  }
  
  /* Infinite Loop */
  while (1)
  { 
  }  
}

/**
  * @brief  Configures the SPI Peripheral.
  * @param  None
  * @retval None
  */
static void SPI_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Peripheral Clock Enable -------------------------------------------------*/
  /* Enable the SPI clock */
  SPIx_CLK_INIT(SPIx_CLK, ENABLE);
  
  /* Enable GPIO clocks */
  RCC_AHB1PeriphClockCmd(SPIx_SCK_GPIO_CLK | SPIx_MISO_GPIO_CLK | SPIx_MOSI_GPIO_CLK, ENABLE);

  /* SPI GPIO Configuration --------------------------------------------------*/
  /* GPIO Deinitialisation */
  GPIO_DeInit(SPIx_SCK_GPIO_PORT);
  GPIO_DeInit(SPIx_MISO_GPIO_PORT);
  GPIO_DeInit(SPIx_MOSI_GPIO_PORT);
  
  /* Connect SPI pins to AF5 */  
  GPIO_PinAFConfig(SPIx_SCK_GPIO_PORT, SPIx_SCK_SOURCE, SPIx_SCK_AF);
  GPIO_PinAFConfig(SPIx_MISO_GPIO_PORT, SPIx_MISO_SOURCE, SPIx_MISO_AF);    
  GPIO_PinAFConfig(SPIx_MOSI_GPIO_PORT, SPIx_MOSI_SOURCE, SPIx_MOSI_AF);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;

  /* SPI SCK pin configuration */
  GPIO_InitStructure.GPIO_Pin = SPIx_SCK_PIN;
  GPIO_Init(SPIx_SCK_GPIO_PORT, &GPIO_InitStructure);
  
  /* SPI  MISO pin configuration */
  GPIO_InitStructure.GPIO_Pin =  SPIx_MISO_PIN;
  GPIO_Init(SPIx_MISO_GPIO_PORT, &GPIO_InitStructure);  

  /* SPI  MOSI pin configuration */
  GPIO_InitStructure.GPIO_Pin =  SPIx_MOSI_PIN;
  GPIO_Init(SPIx_MOSI_GPIO_PORT, &GPIO_InitStructure);
 
  /* SPI configuration -------------------------------------------------------*/
  SPI_I2S_DeInit(SPIx);
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  
  /* Configure the Priority Group to 1 bit */                
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* Configure the SPI interrupt priority */
  NVIC_InitStructure.NVIC_IRQChannel = SPIx_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  Configure a SysTick Base time to 10 ms.
  * @param  None
  * @retval None
  */
static void SysTickConfig(void)
{
  /* Setup SysTick Timer for 10ms interrupts  */
  if (SysTick_Config(SystemCoreClock / 100))
  {
    /* Capture error */
    while (1);
  }

  /* Configure the SysTick handler priority */
  NVIC_SetPriority(SysTick_IRQn, 0x0);
}


/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval PASSED: pBuffer1 identical to pBuffer2
  *         FAILED: pBuffer1 differs from pBuffer2
  */
static TestStatus Buffercmp(uint8_t* pBuffer1, __IO uint8_t* pBuffer2, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer1 != *pBuffer2)
    {
      return FAILED;
    }
    pBuffer1++;
    pBuffer2++;
  }

  return PASSED;
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
