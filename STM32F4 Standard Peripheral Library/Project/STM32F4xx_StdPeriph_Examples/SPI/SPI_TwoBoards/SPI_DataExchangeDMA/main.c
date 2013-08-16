/**
  ******************************************************************************
  * @file    SPI/SPI_TwoBoards/SPI_DataExchangeDMA/main.c
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

/** @addtogroup SPI_DataExchangeDMA
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
    uint8_t aTxBuffer[BUFFERSIZE] = "SPI Master/Slave : Communication between two SPI using DMA";
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
  
  /* The Data transfer is performed in the SPI using Direct Memory Access */

  /* Enable DMA SPI TX Stream */
  DMA_Cmd(SPIx_TX_DMA_STREAM,ENABLE);

  /* Enable DMA SPI RX Stream */
  DMA_Cmd(SPIx_RX_DMA_STREAM,ENABLE);

  /* Enable SPI DMA TX Requsts */
  SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Tx, ENABLE);

  /* Enable SPI DMA RX Requsts */
  SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Rx, ENABLE);
  
  /* Configure the Tamper Button */
  STM_EVAL_PBInit(BUTTON_TAMPER,BUTTON_MODE_GPIO);
  
  /* Wait until Tamper Button is pressed */
  while (STM_EVAL_PBGetState(BUTTON_TAMPER));
  
  /* Enable the SPI peripheral */
  SPI_Cmd(SPIx, ENABLE);
  
#endif /* SPI_MASTER */
  
#ifdef SPI_SLAVE
  /* Slave board configuration */
  /* Initializes the SPI communication */
  SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;
  SPI_Init(SPIx, &SPI_InitStructure);

  /* Enable DMA SPI TX Stream */
  DMA_Cmd(SPIx_TX_DMA_STREAM,ENABLE);

  /* Enable DMA SPI RX Stream */
  DMA_Cmd(SPIx_RX_DMA_STREAM,ENABLE);  
  
  /* Enable SPI DMA TX Requsts */
  SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Tx, ENABLE);

  /* Enable SPI DMA RX Requsts */
  SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Rx, ENABLE);

  /* Enable the SPI peripheral */
  SPI_Cmd(SPIx, ENABLE);
 
#endif /* SPI_SLAVE */

  /* Waiting the end of Data transfer */
  while (DMA_GetFlagStatus(SPIx_TX_DMA_STREAM,SPIx_TX_DMA_FLAG_TCIF)==RESET);
  while (DMA_GetFlagStatus(SPIx_RX_DMA_STREAM,SPIx_RX_DMA_FLAG_TCIF)==RESET);
  
  /* Clear DMA Transfer Complete Flags */
  DMA_ClearFlag(SPIx_TX_DMA_STREAM,SPIx_TX_DMA_FLAG_TCIF);
  DMA_ClearFlag(SPIx_RX_DMA_STREAM,SPIx_RX_DMA_FLAG_TCIF);  
  
  /* Disable DMA SPI TX Stream */
  DMA_Cmd(SPIx_TX_DMA_STREAM,DISABLE);

  /* Disable DMA SPI RX Stream */
  DMA_Cmd(SPIx_RX_DMA_STREAM,DISABLE);  
  
  /* Disable SPI DMA TX Requsts */
  SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Tx, DISABLE);

  /* Disable SPI DMA RX Requsts */
  SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Rx, DISABLE);

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
  DMA_InitTypeDef DMA_InitStructure;

  /* Peripheral Clock Enable -------------------------------------------------*/
  /* Enable the SPI clock */
  SPIx_CLK_INIT(SPIx_CLK, ENABLE);
  
  /* Enable GPIO clocks */
  RCC_AHB1PeriphClockCmd(SPIx_SCK_GPIO_CLK | SPIx_MISO_GPIO_CLK | SPIx_MOSI_GPIO_CLK, ENABLE);
  
  /* Enable DMA clock */
  RCC_AHB1PeriphClockCmd(SPIx_DMA_CLK, ENABLE);

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
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  
  /* DMA configuration -------------------------------------------------------*/
  /* Deinitialize DMA Streams */
  DMA_DeInit(SPIx_TX_DMA_STREAM);
  DMA_DeInit(SPIx_RX_DMA_STREAM);
  
  /* Configure DMA Initialization Structure */
  DMA_InitStructure.DMA_BufferSize = BUFFERSIZE ;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable ;
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull ;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single ;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t) (&(SPIx->DR)) ;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  /* Configure TX DMA */
  DMA_InitStructure.DMA_Channel = SPIx_TX_DMA_CHANNEL ;
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral ;
  DMA_InitStructure.DMA_Memory0BaseAddr =(uint32_t)aTxBuffer ;
  DMA_Init(SPIx_TX_DMA_STREAM, &DMA_InitStructure);
  /* Configure RX DMA */
  DMA_InitStructure.DMA_Channel = SPIx_RX_DMA_CHANNEL ;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory ;
  DMA_InitStructure.DMA_Memory0BaseAddr =(uint32_t)aRxBuffer ; 
  DMA_Init(SPIx_RX_DMA_STREAM, &DMA_InitStructure);
  
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
