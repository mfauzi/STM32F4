#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_usart.h>

GPIO_InitTypeDef GPIO_InitStructure;
USART_InitTypeDef USART_InitStructure;

void PrintChar(char c);
void msDelay(unsigned long msTime);

int main(void)
{
	  SystemInit();

	  /* Enable GPIO clock */
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	  /* Enable UART clock */
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	  /* Connect PXx to USARTx_Tx*/
	  GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);

	  /* Connect PXx to USARTx_Rx*/
	  GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);

	  /* Configure USART Tx as alternate function  */
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	  GPIO_Init(GPIOB, &GPIO_InitStructure);

	  /* Configure USART Rx as alternate function  */
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	  GPIO_Init(GPIOB, &GPIO_InitStructure);

	  USART_InitStructure.USART_BaudRate = 921600;
	  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	  USART_InitStructure.USART_StopBits = USART_StopBits_1;
	  USART_InitStructure.USART_Parity = USART_Parity_No;
	  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	  /* USART configuration */
	  USART_Init(USART3, &USART_InitStructure);

	  /* Enable USART */
	  USART_Cmd(USART3, ENABLE);


	  while(1)
	  {
		  PrintChar(0x55);
		  msDelay(1000);
	  }
}

void PrintChar(char c)
{
	uint8_t ch;
	ch = c;
	USART_SendData(USART3, (uint8_t) ch);

	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
}

void msDelay(unsigned long msTime)
{
	unsigned long i,j, nCount=0x1310;
	for(i=0;i<msTime;i++)
		for(j=0;j<nCount;j++);
}
