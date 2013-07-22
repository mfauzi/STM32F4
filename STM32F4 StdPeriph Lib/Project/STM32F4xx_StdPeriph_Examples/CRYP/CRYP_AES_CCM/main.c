/**
  ******************************************************************************
  * @file    CRYP/CRYP_AES_CCM/main.c 
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

/** @addtogroup CRYP_AES_CCM
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* AES Key size is 128-bit (16 bytes) */
uint8_t AES128key[KEY_SIZE/8] = {0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
                                 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f};
uint8_t Nonce[NONCE_SIZE] = {0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17};
uint32_t NonceSize = NONCE_SIZE;
uint8_t HeaderMessage[HEADER_SIZE] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                                      0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
uint8_t HBuffer[HEADER_SIZE + 21]; /* Temporary buffer used to append the header.
                                      HBuffer size must be equal to HEADER_SIZE + 21 */
uint8_t PlainText[PLAINTEXT_SIZE] = {0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
                                     0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f};
uint8_t ExpectedCypherText[PLAINTEXT_SIZE] = {0xd2, 0xa1, 0xf0, 0xe0, 0x51, 0xea, 0x5f, 0x62,
                                              0x08, 0x1a, 0x77, 0x92, 0x07, 0x3d, 0x59, 0x3d};
uint8_t OutputText[PLAINTEXT_SIZE]; /* used for storing either encrypted or decrypted text */
uint8_t MAC[MAC_SIZE];/* used for storing computed MAC (TAG) */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

static void USART_Config(void);
static void Display_PlainData(uint32_t datalength);
static void Display_EncryptedData(uint8_t mode, uint16_t keysize, uint32_t datalength);
static void Display_DecryptedData(uint8_t mode, uint16_t keysize, uint32_t datalength);
static void Display_MAC(uint8_t* MAC, uint32_t MACSize);
static char PressToContinue(void);
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
       
  /* USARTx configured as follow:
        - BaudRate = 115200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  USART_Config();
  
  /* Enable CRYP clock */
  RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_CRYP, ENABLE);
  
  while(1)
  { 
    /* Display Plain Text */
    Display_PlainData(PLAINTEXT_SIZE);

    /********************************************************************/
    /* !!!! This example runs only on STM32F437x Devices !!!            */
    /********************************************************************/
    
    /* Encrypt the plaintext message */
    if(CRYP_AES_CCM(MODE_ENCRYPT, Nonce, NonceSize, AES128key, KEY_SIZE, PlainText, PLAINTEXT_SIZE, HeaderMessage, HEADER_SIZE, HBuffer, OutputText, MAC, MAC_SIZE) == SUCCESS)
    {
      /* Display encrypted Data */
      Display_EncryptedData(AES_MODE_CCM, KEY_SIZE, PLAINTEXT_SIZE);
      /* Display computed MAC */
      Display_MAC(MAC, MAC_SIZE);
    }
    
    /* Decrypt the cyphertext message */
    if(CRYP_AES_CCM(MODE_DECRYPT, Nonce, NonceSize, AES128key, KEY_SIZE, ExpectedCypherText, PLAINTEXT_SIZE, HeaderMessage, HEADER_SIZE, HBuffer, OutputText, MAC, MAC_SIZE) == SUCCESS)
    {
      /* Display encrypted Data */
      Display_DecryptedData(AES_MODE_CCM, KEY_SIZE, PLAINTEXT_SIZE);
      /* Display computed MAC */
      Display_MAC(MAC, MAC_SIZE);
    }
    
    PressToContinue();
    printf("\n\r Example restarted...\n ");
  }
}

/**
  * @brief  Display Plain Data 
  * @param  datalength: length of the data to display
  * @retval None
  */
static void Display_PlainData(uint32_t datalength)
{
  uint32_t buffercounter =0;
  uint32_t count = 0;
  
  printf("\n\r =============================================================\n\r");
  printf(" ================= AES using CCM mode ===================\n\r");
  printf(" ============================================================\n\r");
  printf(" ---------------------------------------\n\r");
  printf(" Plain Data :\n\r");
  printf(" ---------------------------------------\n\r");
  
  for(buffercounter = 0; buffercounter < datalength; buffercounter++)
  {
    printf("[0x%02X]", PlainText[buffercounter]);
    count++;

    if(count == 16)
    { 
      count = 0;
      printf("  Block %d \n\r", buffercounter/16);
    }
  }
}

/**
  * @brief  Display Encrypted Data 
  * @param  mode: chaining mode
  * @param  keysize: AES key size used
  * @param  datalength: length of the data to display
  * @retval None
  */
static void Display_EncryptedData(uint8_t mode, uint16_t keysize, uint32_t datalength)
{
  uint32_t buffercounter = 0;
  uint32_t count = 0;

  printf("\n\r =======================================\n\r");
  printf(" Encrypted Data with AES %d  Mode  ",keysize );
  if(mode == AES_MODE_CCM)
  {
    printf("CCM\n\r");
  }
  printf(" ---------------------------------------\n\r");
  
  for(buffercounter = 0; buffercounter < datalength; buffercounter++)
  {
    printf("[0x%02X]", OutputText[buffercounter]);

    count++;
    if(count == 16)
    { 
      count = 0;
      printf(" Block %d \n\r", buffercounter/16);
    }
  }
}

/**
  * @brief  Display Decrypted Data 
  * @param  mode: chaining mode
  * @param  keysize: AES key size used
  * @param  datalength: length of the data to display
  * @retval None
  */
static void Display_DecryptedData(uint8_t mode, uint16_t keysize, uint32_t datalength)
{
  uint32_t buffercounter = 0;
  uint32_t count = 0;

  printf("\n\r =======================================\n\r");
  printf(" Decrypted Data with AES %d  Mode  ",keysize ); 
  if(mode == AES_MODE_CCM)
  {
    printf("CCM\n\r");
  }
  printf(" ---------------------------------------\n\r");
  
  for(buffercounter = 0; buffercounter < datalength; buffercounter++)
  {
    printf("[0x%02X]", OutputText[buffercounter]);
    count++;

    if(count == 16)
    { 
      count = 0;
      printf(" Block %d \n\r", buffercounter/16);
    }
  }
}

/**
  * @brief  Display Message Authentication Code (MAC)
  * @param  MAC: the computed MAC
  * @param  MACSize: the size of the MAC
  * @retval None
  */
static void Display_MAC(uint8_t* MAC, uint32_t MACSize)
{
  uint32_t buffercounter = 0;
  uint32_t count = 0;

  printf("\n\r =======================================\n\r");
  printf(" Message Authentication Code (MAC):\n\r  "); 
  printf("---------------------------------------\n\r");
  
  for(buffercounter = 0; buffercounter < MACSize; buffercounter++)
  {
    printf("[0x%02X]", MAC[buffercounter]);
    count++;

    if(count == 16)
    { 
      count = 0;
      printf(" Block %d \n\r", MACSize/16);
    }
  }
}

/**
  * @brief  USART configuration 
  * @param  None
  * @retval None
  */
static void USART_Config(void)
{
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
  * @brief  Wait till a character is received by the USART
  * @param  None
  * @retval Keyboard pressed character
  */
static char PressToContinue(void)
{
  char c; 
  printf("\n\r Press any key to continue...\n\r ");

  while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_RXNE) == RESET)
  {
  }

  c = USART_ReceiveData(EVAL_COM1);

  return(c);
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
