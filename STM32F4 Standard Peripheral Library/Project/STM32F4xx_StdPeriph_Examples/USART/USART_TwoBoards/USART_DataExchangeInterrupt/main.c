/**
  ******************************************************************************
  * @file    USART/USART_TwoBoards/USART_DataExchangeInterrupt/main.c
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

/** @addtogroup USART_DataExchangeInterrupt
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t aTxBuffer[BUFFERSIZE] = "USART Interrupt Example: Communication between two USART using Interrupt";
uint8_t aRxBuffer [BUFFERSIZE];
__IO uint8_t ubRxIndex = 0x00;
__IO uint8_t ubTxIndex = 0x00;

__IO uint32_t TimeOut = 0x00;  
/* Private function prototypes -----------------------------------------------*/
static void USART_Config(void);
static void SysTickConfig(void);
#ifdef USART_RECEIVER
static TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);
#endif
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
  
  /* Wait until Tamper Button is pressed */
  while (STM_EVAL_PBGetState(BUTTON_TAMPER));  
  
  /* Enable the Tx buffer empty interrupt */
  USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);
  
#endif /* USART_TRANSMITTER */
  
#ifdef USART_RECEIVER
  
  /* Enable the Tx buffer empty interrupt */
  USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);

  /* Waiting the end of Data transfer */
  while ((ubTxIndex < BUFFERSIZE) && (ubRxIndex < BUFFERSIZE))
  {
  } 
  
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
  NVIC_InitTypeDef NVIC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* Enable GPIO clock */
  RCC_AHB1PeriphClockCmd(USARTx_TX_GPIO_CLK | USARTx_RX_GPIO_CLK, ENABLE);
  
  /* Enable USART clock */
  USARTx_CLK_INIT(USARTx_CLK, ENABLE);
  
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

  /* Enable the USART OverSampling by 8 */
  USART_OverSampling8Cmd(USARTx, ENABLE);  

  /* USARTx configuration ----------------------------------------------------*/
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
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USARTx, &USART_InitStructure);
  
  /* NVIC configuration */
  /* Configure the Priority Group to 2 bits */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* Enable the USARTx Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USARTx_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
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
  /* Setup SysTick Timer for 10ms interrupts  */
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
#endif

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
