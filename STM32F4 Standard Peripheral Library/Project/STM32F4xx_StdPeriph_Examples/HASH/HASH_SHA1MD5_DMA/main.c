/**
  ******************************************************************************
  * @file    HASH/HASH_SHA1MD5_DMA/main.c
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

/** @addtogroup HASH_SHA1MD5_DMA
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
HASH_MsgDigest MsgDigest;
static uint8_t MessageToHash[SIZE_MSG_TO_HASH_IN_BYTES]= 
"The STM32 F4 series is the result of a perfect symbiosis of the real-time control capabilities of an MCU and the signal processing performance of a DSP, and thus complements the STM32 portfolio with a new class of devices, digital signal controllers (DSC).";

/* Private function prototypes -----------------------------------------------*/
static void HASH_SHA1_DMA(void);
static void HASH_MD5_DMA(void);
static void Display_MainMessage(void);
static void Display_SHA1Digest(void);
static void Display_MD5Digest(void);
static void USART_Config(void);

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

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
       file (startup_stm32f4xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f4xx.c file
     */

  /* USART configuration */
  USART_Config();

  /* Enable HASH and DMA clocks */
  RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_HASH, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

  /* Display the original message */
  Display_MainMessage();

/*=============================================================================
  SHA-1 Digest Computation
==============================================================================*/
  /* HASH SHA-1 Digest computation (using DMA for data transfer) */
  HASH_SHA1_DMA();

  /* Display the SHA1 digest */
  Display_SHA1Digest();

/*=============================================================================
  MD5 Digest Computation
==============================================================================*/
  /* HASH MD5 Digest computation (using DMA for data transfer) */
  HASH_MD5_DMA();

  /*  Display the MD5 digest */
  Display_MD5Digest();

  while(1);
}

/**
  * @brief  HASH SHA-1 Digest computation (using DMA for data transfer) 
  * @note   DMA2 stream7 channel2 is used to transfer data from memory 
  *         (MessageToHash[] array) to HASH Peripheral (the INPUT data register).
  * @param  None
  * @retval None
  */
static void HASH_SHA1_DMA(void)
{
  HASH_InitTypeDef HASH_InitStructure;
  DMA_InitTypeDef DMA_InitStructure;

  /* HASH Configuration */
  HASH_StructInit(&HASH_InitStructure);
  HASH_InitStructure.HASH_AlgoSelection = HASH_AlgoSelection_SHA1;
  HASH_InitStructure.HASH_AlgoMode = HASH_AlgoMode_HASH;
  HASH_InitStructure.HASH_DataType = HASH_DataType_8b;
  HASH_Init(&HASH_InitStructure);

  /*DMA Configuration*/
  DMA_DeInit(DMA2_Stream7);
  DMA_InitStructure.DMA_Channel = DMA_Channel_2;
  DMA_InitStructure.DMA_PeripheralBaseAddr = HASH_DIN_REG_ADDR;
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)MessageToHash;
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
  DMA_InitStructure.DMA_BufferSize = SIZE_MSG_TO_HASH_IN_WORDS;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

  /* Configure the DMA Stream */
  DMA_Init(DMA2_Stream7, &DMA_InitStructure);

  /* Enable HASH DMA */
  HASH_DMACmd(ENABLE);

  /* Enable DMA2 Transfer */
  DMA_Cmd(DMA2_Stream7, ENABLE);

  /* Note :  When the DMA is enabled, it provides the information to the hash 
             processor when it is transferring the last data word. Then the 
             padding and digest computation are performed automatically as if 
             DCAL had been written to 1.*/

  /* Wait until DMA Transfer completed */
  while (DMA_GetFlagStatus(DMA2_Stream7, DMA_FLAG_TCIF7) == RESET);

  /* Wait until the Busy flag is RESET */
  while (HASH_GetFlagStatus(HASH_FLAG_BUSY) != RESET);

  /* Get the SHA-1 Digest */
  HASH_GetDigest(&MsgDigest);
}


/**
  * @brief  HASH MD5 Digest computation (using DMA for data transfer) 
  * @note   DMA2 stream7 channel2 is used to transfer data from memory
  *         (MessageToHash[] array) to HASH Peripheral (the INPUT data register).
  * @param  None
  * @retval None
  */
static void HASH_MD5_DMA(void)
{
  HASH_InitTypeDef HASH_InitStructure;
  DMA_InitTypeDef DMA_InitStructure;

  /* HASH Configuration */
  HASH_InitStructure.HASH_AlgoSelection = HASH_AlgoSelection_MD5;
  HASH_InitStructure.HASH_AlgoMode = HASH_AlgoMode_HASH;
  HASH_InitStructure.HASH_DataType = HASH_DataType_8b;
  HASH_InitStructure.HASH_HMACKeyType = HASH_HMACKeyType_ShortKey;
  HASH_Init(&HASH_InitStructure);

  /*DMA Configuration*/
  DMA_DeInit(DMA2_Stream7);
  DMA_InitStructure.DMA_Channel = DMA_Channel_2;
  DMA_InitStructure.DMA_PeripheralBaseAddr = HASH_DIN_REG_ADDR;
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)MessageToHash;
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
  DMA_InitStructure.DMA_BufferSize = SIZE_MSG_TO_HASH_IN_WORDS;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

  /* Configure the DMA Stream */
  DMA_Init(DMA2_Stream7, &DMA_InitStructure);

  /* Enable HASH DMA */
  HASH_DMACmd(ENABLE);

  /* Enable DMA2 Transfer */
  DMA_Cmd(DMA2_Stream7, ENABLE);

  /* Note :  When the DMA is enabled, it provides the information to the hash 
             processor when it is transferring the last data word. Then the 
             padding and digest computation are performed automatically as if 
             DCAL had been written to 1.*/

  /* wait until DMA Transfer completed */
  while (DMA_GetFlagStatus(DMA2_Stream7, DMA_FLAG_TCIF7) == RESET);

  /* wait until the Busy flag is RESET */
  while (HASH_GetFlagStatus(HASH_FLAG_BUSY) != RESET);

  /* Get the MD5 Digest */
  HASH_GetDigest(&MsgDigest);
}

/**
  * @brief  Display the original message.
  * @param  None
  * @retval None
  */
static void Display_MainMessage(void)
{
  uint32_t BufferCounter=0;
  
  printf("\n\r ======================================\n\r");
  printf(" ====    HASH Using DMA Example    ====\n\r");
  printf(" ======================================\n\r");
  printf(" ---------------------------------------\n\r");
  printf(" Text to be Hashed (%d bits):\n\r", SIZE_MSG_TO_HASH_IN_BYTES*8);
  printf(" ---------------------------------------\n\r");

  for(BufferCounter = 0; BufferCounter <SIZE_MSG_TO_HASH_IN_BYTES; BufferCounter++)
  {
    printf("%c", MessageToHash[BufferCounter]);
    
    if(((BufferCounter%31) == 0) && (BufferCounter != 0))
    {
      printf("\n\r");
    }
  }
}

/**
  * @brief  Display the SHA-1 digest.
  * @param  None
  * @retval None
  */
static void Display_SHA1Digest(void)
{
  printf("\n\r ---------------------------------------\n\r");
  printf(" SHA1 Message Digest (160 bits):\n\r");
  printf(" ---------------------------------------\n\r");
  printf(" H0 = [0x%08X]  \n\r", MsgDigest.Data[0]);
  printf(" H1 = [0x%08X]  \n\r", MsgDigest.Data[1]);
  printf(" H2 = [0x%08X]  \n\r", MsgDigest.Data[2]);
  printf(" H3 = [0x%08X]  \n\r", MsgDigest.Data[3]);
  printf(" H4 = [0x%08X]  \n\r", MsgDigest.Data[4]);
}

/**
  * @brief  Display the MD5 digest.
  * @param  None
  * @retval None
  */
static void Display_MD5Digest(void)
{
  printf("\n\r ---------------------------------------\n\r");
  printf(" MD5 Message Digest (128 bits):\n\r");
  printf(" ---------------------------------------\n\r");
  printf(" A = [0x%08X]  \n\r", MsgDigest.Data[0]);
  printf(" B = [0x%08X]  \n\r", MsgDigest.Data[1]);
  printf(" C = [0x%08X]  \n\r", MsgDigest.Data[2]);
  printf(" D = [0x%08X]  \n\r", MsgDigest.Data[3]);
}

/**
  * @brief  USART configuration 
  * @param  None
  * @retval None
  */
static void USART_Config(void)
{
  /* USARTx configured as follow:
        - BaudRate = 115200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  USART_InitTypeDef USART_InitStructure;

  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  STM_EVAL_COMInit(COM1, &USART_InitStructure);
}

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(EVAL_COM1, (uint8_t) ch);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TC) == RESET)
  {}

  return ch;
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
  {
  }
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
