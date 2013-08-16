/**
  ******************************************************************************
  * @file    I2S/I2S_TwoBoards/I2S_DataExchangePolling/main.c
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

/** @addtogroup I2S_DataExchangePolling
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
I2S_InitTypeDef I2S_InitStructure;

const uint16_t aTxMasterBuffer[] =
  {
    0x1234, 0x5678, 0x1ABC, 0xDEF0, 0x5555,
    0x1748, 0x9027, 0x4424, 0x9623, 0x0932,
    0x9744, 0x7845, 0x0912, 0x1111, 0x1772,
    0x9736, 0x0933, 0x0187, 0x9567, 0x9977
  };

const uint16_t aTxSlaveBuffer[] =
  {
    0x1111, 0x7845, 0x0912, 0xDEF0, 0x9977,
    0x0187, 0x9736, 0x9567, 0xDEF0, 0x1772,
    0x5555, 0x1111, 0x0933, 0x1234, 0x9567,
    0x9736, 0x4444, 0x9ABC, 0x1111, 0x4424
  };

__IO uint16_t aRxMasterBuffer [RX_MASTER_BUFFERSIZE];
__IO uint16_t aRxSlaveBuffer [RX_SLAVE_BUFFERSIZE];
__IO uint8_t  ubBufferCounter = 0;

/* Private function prototypes -----------------------------------------------*/
static void I2S_Config(void);
static void SysTickConfig(void);
static TestStatus Buffercmp(uint8_t* pBuffer1, __IO uint8_t* pBuffer2, uint16_t BufferLength);
static void Fill_Buffer(__IO uint8_t *pBuffer, uint16_t BufferLength);


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
       file (startup_stm32f40xx.s/startup_stm32f427x.s) before to branch to 
       application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f4xx.c file
     */

  /* I2S configuration -------------------------------------------------------*/
  I2S_Config();

  /* SysTick configuration ---------------------------------------------------*/
  SysTickConfig();

  /* LEDs configuration ------------------------------------------------------*/
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);

#ifdef I2S_MASTER  
  /* Master full Duplex configuration ----------------------------------------*/
  /* Clear the Rx Master Buffer */
  Fill_Buffer((uint8_t*)aRxMasterBuffer, (TX_MASTER_BUFFERSIZE*2));

  /* Configure I2Sx in Master Transmitter Mode */
  I2S_InitStructure.I2S_Mode = I2S_Mode_MasterTx;
  I2S_Init(I2Sx, &I2S_InitStructure);
  
  /* Configure the I2Sx_ext (the second instance) in Slave Receiver Mode */
  I2S_FullDuplexConfig(I2Sxext, &I2S_InitStructure);
  
  /* Enable the I2Sx peripheral */
  I2S_Cmd(I2Sx, ENABLE);
  /* Enable the I2Sx_ext peripheral for Full Duplex mode */ 
  I2S_Cmd(I2Sxext, ENABLE);

  /* Master full Duplex Communication ----------------------------------------*/
  /* Communication Full Duplex Started */
  ubBufferCounter = 0;
  while ((ubBufferCounter != TX_MASTER_BUFFERSIZE))
  {
    /* Data to transmitted through I2Sx SD pin */
    while (SPI_I2S_GetFlagStatus(I2Sx, SPI_I2S_FLAG_TXE ) != SET);
    SPI_I2S_SendData(I2Sx, aTxMasterBuffer[ubBufferCounter]);
   
    /* Data Received through I2Sx_ext SD pin */ 
    while (SPI_I2S_GetFlagStatus(I2Sxext, SPI_I2S_FLAG_RXNE ) != SET);
    aRxMasterBuffer[ubBufferCounter] = SPI_I2S_ReceiveData(I2Sxext);
    ubBufferCounter++;
    
  }
  /* Communication Full Duplex Finished */
  I2S_Cmd(I2Sx, DISABLE);
  I2S_Cmd(I2Sxext, DISABLE);
 
  /* Check Communication Result ----------------------------------------------*/
  if (Buffercmp((uint8_t*)aRxMasterBuffer, (uint8_t*)aTxSlaveBuffer, (TX_SLAVE_BUFFERSIZE*2)) != FAILED) 
  {
    /* Turn ON LED2 and LED4 */
    STM_EVAL_LEDOn(LED2);
    STM_EVAL_LEDOn(LED4);
  }
#endif /* I2S_MASTER */

#ifdef I2S_SLAVE
  /* Slave full Duplex configuration ----------------------------------------*/ 
  /* Clear the RxBuffer */
  Fill_Buffer((uint8_t*)aRxSlaveBuffer, (TX_SLAVE_BUFFERSIZE*2));
    
  /* Configure I2Sx in Slave Receiver Mode */
  I2S_InitStructure.I2S_Mode = I2S_Mode_SlaveRx;
  I2S_Init(I2Sx, &I2S_InitStructure);
  
  /* Configure the I2Sx_ext (the second instance) in Slave Transmitter Mode */
  I2S_FullDuplexConfig(I2Sxext, &I2S_InitStructure);
  
  /* Enable the I2Sx_ext peripheral for Full Duplex mode */ 
  I2S_Cmd(I2Sxext, ENABLE);
  /* Enable the I2Sx peripheral */
  I2S_Cmd(I2Sx, ENABLE);
 
  /* Slave full Duplex Communication -----------------------------------------*/
  /* Communication Full Duplex Started */
  ubBufferCounter = 0;
  while ((ubBufferCounter != TX_SLAVE_BUFFERSIZE) )
  {
    /* Data to transmitted through I2Sx_ext SD pin */
    while (SPI_I2S_GetFlagStatus(I2Sxext, SPI_I2S_FLAG_TXE ) != SET);
    SPI_I2S_SendData(I2Sxext, aTxSlaveBuffer[ubBufferCounter]);
    
    /* Data Received through I2Sx SD pin */ 
    while (SPI_I2S_GetFlagStatus(I2Sx, SPI_I2S_FLAG_RXNE ) != SET);
    aRxSlaveBuffer[ubBufferCounter] = SPI_I2S_ReceiveData(I2Sx);
    ubBufferCounter++;
    
  }
  /* Communication full duplex Finished */
  I2S_Cmd(I2Sx, DISABLE);
  I2S_Cmd(I2Sxext, DISABLE);

  /* Check Communication Results ---------------------------------------------*/
  if (Buffercmp((uint8_t*)aRxSlaveBuffer, (uint8_t*)aTxMasterBuffer, (TX_SLAVE_BUFFERSIZE*2)) != FAILED) 
  {
    /* Turn ON LED3 */
    STM_EVAL_LEDOn(LED3);
  }
#endif /* I2S_SLAVE */
        
  while(1)
  {
  }
}

/**
  * @brief  Configures the I2S Peripheral.
  * @param  None
  * @retval None
  */
static void I2S_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Peripheral Clock Enable -------------------------------------------------*/
  /* Enable the I2Sx/I2Sx_ext clock */
  I2Sx_CLK_INIT(I2Sx_CLK, ENABLE);
  
  /* Enable GPIO clocks */
  RCC_AHB1PeriphClockCmd(I2Sx_WS_GPIO_CLK | I2Sx_CK_GPIO_CLK | \
                         I2Sx_SD_GPIO_CLK | I2Sxext_SD_GPIO_CLK, ENABLE);

  /* I2S GPIO Configuration --------------------------------------------------*/
  /* Connect I2S pins to Alternate functions */  
  GPIO_PinAFConfig(I2Sx_WS_GPIO_PORT, I2Sx_WS_SOURCE, I2Sx_WS_AF);
  GPIO_PinAFConfig(I2Sx_CK_GPIO_PORT, I2Sx_CK_SOURCE, I2Sx_CK_AF);
  GPIO_PinAFConfig(I2Sx_SD_GPIO_PORT, I2Sx_SD_SOURCE, I2Sx_SD_AF);
  GPIO_PinAFConfig(I2Sxext_SD_GPIO_PORT, I2Sxext_SD_SOURCE, I2Sxext_SD_AF);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;

  /* I2S WS pin configuration */
  GPIO_InitStructure.GPIO_Pin = I2Sx_WS_PIN;
  GPIO_Init(I2Sx_WS_GPIO_PORT, &GPIO_InitStructure);

  /* I2S CK pin configuration */
  GPIO_InitStructure.GPIO_Pin =  I2Sx_CK_PIN;
  GPIO_Init(I2Sx_CK_GPIO_PORT, &GPIO_InitStructure);
  
  /* I2S SD pin configuration */
  GPIO_InitStructure.GPIO_Pin = I2Sx_SD_PIN;
  GPIO_Init(I2Sx_SD_GPIO_PORT, &GPIO_InitStructure);

  /* I2S Extended SD pin configuration */
  GPIO_InitStructure.GPIO_Pin =  I2Sxext_SD_PIN;
  GPIO_Init(I2Sxext_SD_GPIO_PORT, &GPIO_InitStructure);
 
  /* I2S configuration -------------------------------------------------------*/
  /* Initialize  I2Sx and I2Sxext peripherals */
  SPI_I2S_DeInit(I2Sx);
  /* Configure the Audio Frequency, Standard and the data format */
  I2S_InitStructure.I2S_AudioFreq = I2S_AudioFreq_8k;
  I2S_InitStructure.I2S_Standard = I2S_Standard_Phillips;
  I2S_InitStructure.I2S_MCLKOutput = I2S_MCLKOutput_Disable;
  I2S_InitStructure.I2S_CPOL = I2S_CPOL_Low;
  I2S_InitStructure.I2S_DataFormat = I2S_DataFormat_16b;
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

/**
  * @brief  Fills buffer.
  * @param  pBuffer: pointer on the Buffer to fill
  * @param  BufferLength: size of the buffer to fill
  * @retval None
  */
static void Fill_Buffer(__IO uint8_t *pBuffer, uint16_t BufferLength)
{
  uint32_t index = 0;

  /* Put in global buffer same values */
  for (index = 0; index < BufferLength; index++ )
  {
    pBuffer[index] = 0x00;
  }
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
