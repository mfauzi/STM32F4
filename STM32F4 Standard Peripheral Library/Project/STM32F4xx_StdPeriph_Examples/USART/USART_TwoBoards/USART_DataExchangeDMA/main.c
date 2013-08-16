/**
  ******************************************************************************
  * @file    USART/USART_TwoBoards/USART_DataExchangeDMA/main.c
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

/** @addtogroup USART_DataExchangeDMA
* @{
*/

/* Private typedef -----------------------------------------------------------*/
DMA_InitTypeDef  DMA_InitStructure;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t aTxBuffer[BUFFERSIZE] = "USART DMA Example: Communication between two USART using DMA";
uint8_t aRxBuffer [BUFFERSIZE];
__IO uint32_t TimeOut = 0x0;   



/* Private function prototypes -----------------------------------------------*/
static void USART_Config(void);
static void SysTickConfig(void);

#ifdef USART_RECEIVER
static TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);
#endif /* USART_RECEIVER_MODE */

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
  
  /* USART configuration -----------------------------------------------------*/
  USART_Config();
  
  /* SysTick configuration ---------------------------------------------------*/
  SysTickConfig();
  
  /* LEDs configuration ------------------------------------------------------*/
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3);
  
#ifdef USART_TRANSMITTER
  
  /* Tamper Button Configuration ---------------------------------------------*/
  STM_EVAL_PBInit(BUTTON_TAMPER,BUTTON_MODE_GPIO);
  
  /* Enable DMA USART TX Stream */
  DMA_Cmd(USARTx_TX_DMA_STREAM,ENABLE);
  
  /* Wait until Tamper Button is pressed */
  while (STM_EVAL_PBGetState(BUTTON_TAMPER));  
  
  /* Enable USART DMA TX Requsts */
  USART_DMACmd(USARTx, USART_DMAReq_Tx, ENABLE);

  /* Waiting the end of Data transfer */
  while (USART_GetFlagStatus(USARTx,USART_FLAG_TC)==RESET);    
  while (DMA_GetFlagStatus(USARTx_TX_DMA_STREAM,USARTx_TX_DMA_FLAG_TCIF)==RESET);
  
  /* Clear DMA Transfer Complete Flags */
  DMA_ClearFlag(USARTx_TX_DMA_STREAM,USARTx_TX_DMA_FLAG_TCIF);
  /* Clear USART Transfer Complete Flags */
  USART_ClearFlag(USARTx,USART_FLAG_TC);  
  
#endif /* USART_TRANSMITTER */
  
#ifdef USART_RECEIVER
  
  /* Enable DMA USART RX Stream */
  DMA_Cmd(USARTx_RX_DMA_STREAM,ENABLE);
  
  /* Enable USART DMA RX Requsts */
  USART_DMACmd(USARTx, USART_DMAReq_Rx, ENABLE);
  
  /* Waiting the end of Data transfer */
  while (USART_GetFlagStatus(USARTx,USART_FLAG_TC)==RESET);    
  while (DMA_GetFlagStatus(USARTx_RX_DMA_STREAM,USARTx_RX_DMA_FLAG_TCIF)==RESET);
         
  /* Clear DMA Transfer Complete Flags */
  DMA_ClearFlag(USARTx_RX_DMA_STREAM,USARTx_RX_DMA_FLAG_TCIF);
  /* Clear USART Transfer Complete Flags */
  USART_ClearFlag(USARTx,USART_FLAG_TC);

  if (Buffercmp(aTxBuffer, aRxBuffer, BUFFERSIZE) != FAILED)
  {
    /* Turn ON LED2 */
    STM_EVAL_LEDOn(LED2);
  }
  else
  {
    /* Turn ON LED3 */
    STM_EVAL_LEDOn(LED3);
  }
  
#endif /* USART_RECEIVER */
         
  while (1)
  { 
  }
}

/**
  * @brief  Configures the USART Peripheral.
  * @param  None
  * @retval None
  */
static void USART_Config(void)
{
  USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Peripheral Clock Enable -------------------------------------------------*/
  /* Enable GPIO clock */
  RCC_AHB1PeriphClockCmd(USARTx_TX_GPIO_CLK | USARTx_RX_GPIO_CLK, ENABLE);
  
  /* Enable USART clock */
  USARTx_CLK_INIT(USARTx_CLK, ENABLE);
  
  /* Enable the DMA clock */
  RCC_AHB1PeriphClockCmd(USARTx_DMAx_CLK, ENABLE);
  
  /* USARTx GPIO configuration -----------------------------------------------*/ 
  /* Connect USART pins to AF7 */
  GPIO_PinAFConfig(USARTx_TX_GPIO_PORT, USARTx_TX_SOURCE, USARTx_TX_AF);
  GPIO_PinAFConfig(USARTx_RX_GPIO_PORT, USARTx_RX_SOURCE, USARTx_RX_AF);
  
  /* Configure USART Tx and Rx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  
  GPIO_InitStructure.GPIO_Pin = USARTx_TX_PIN;
  GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = USARTx_RX_PIN;
  GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStructure);
 
  /* USARTx configuration ----------------------------------------------------*/
  /* Enable the USART OverSampling by 8 */
  USART_OverSampling8Cmd(USARTx, ENABLE); 
  
  /* USARTx configured as follow:
        - BaudRate = 5250000 baud
		   - Maximum BaudRate that can be achieved when using the Oversampling by 8
		     is: (USART APB Clock / 8) 
			 Example: 
			    - (USART3 APB1 Clock / 8) = (42 MHz / 8) = 5250000 baud
			    - (USART1 APB2 Clock / 8) = (84 MHz / 8) = 10500000 baud
		   - Maximum BaudRate that can be achieved when using the Oversampling by 16
		     is: (USART APB Clock / 16) 
			 Example: (USART3 APB1 Clock / 16) = (42 MHz / 16) = 2625000 baud
			 Example: (USART1 APB2 Clock / 16) = (84 MHz / 16) = 5250000 baud
        - Word Length = 8 Bits
        - one Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */ 
  USART_InitStructure.USART_BaudRate = 5250000;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  /* When using Parity the word length must be configured to 9 bits */
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USARTx, &USART_InitStructure);

  /* Configure DMA controller to manage USART TX and RX DMA request ----------*/ 
   
  /* Configure DMA Initialization Structure */
  DMA_InitStructure.DMA_BufferSize = BUFFERSIZE ;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable ;
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull ;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single ;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t) (&(USARTx->DR)) ;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  /* Configure TX DMA */
  DMA_InitStructure.DMA_Channel = USARTx_TX_DMA_CHANNEL ;
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral ;
  DMA_InitStructure.DMA_Memory0BaseAddr =(uint32_t)aTxBuffer ;
  DMA_Init(USARTx_TX_DMA_STREAM,&DMA_InitStructure);
  /* Configure RX DMA */
  DMA_InitStructure.DMA_Channel = USARTx_RX_DMA_CHANNEL ;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory ;
  DMA_InitStructure.DMA_Memory0BaseAddr =(uint32_t)aRxBuffer ; 
  DMA_Init(USARTx_RX_DMA_STREAM,&DMA_InitStructure);
         
  /* Enable USART */
  USART_Cmd(USARTx, ENABLE);
}

/**
  * @brief  Configures the SysTick Base time to 10 ms.
  * @param  None
  * @retval None
  */
static void SysTickConfig(void)
{
  /* Set SysTick Timer for 10ms interrupts  */
  if (SysTick_Config(SystemCoreClock / 100))
  {
    /* Capture error */
    while (1);
  }
  /* Configure the SysTick handler priority */
  NVIC_SetPriority(SysTick_IRQn, 0x0);
}

#ifdef USART_RECEIVER

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval PASSED: pBuffer1 identical to pBuffer2
  *         FAILED: pBuffer1 differs from pBuffer2
  */

static TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
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

#endif /* USART_RECEIVER */

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
