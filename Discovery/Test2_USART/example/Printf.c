#include "stm32f4xx_usart.h"

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
void PrintChar(char c)
{
	uint8_t ch;
  ch = c;
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USART3, (uint8_t) ch);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
  {}

}

