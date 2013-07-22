/**
  *****************************************************************************
  * @title   main.c
  * @author  Fauzi
  * @date    12 July 2013
  * @brief   Turn on all LEDs in PX4Flow
  *
  *          PE3 (Blue), PE2 (Amber) and PE7 (Red) are configured in output push pull mode, turn ON.
  *           - Set PD12, PD13, PD14 and PD15 by setting corresponding bits in BSRRL register
  *           - Reset PD12, PD13, PD14 and PD15  by setting corresponding bits in BSRRH register
  *
  *******************************************************************************
  **/


#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_usart.h>

GPIO_InitTypeDef GPIO_InitStructure;
USART_InitTypeDef USART_InitStructure;

void usart_init(void);
void PrintChar(char c);
void msDelay(unsigned long msTime);

int main(void)
{

	SystemInit();

	/* GPIOE Peripheral clock enable */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	/* Configure PE3, PE2 and PE7 in output push-pull mode */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_2| GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_SetBits(GPIOE, GPIO_Pin_3 | GPIO_Pin_2| GPIO_Pin_7); //all OFF

	usart_init();

	while(1)
	{
		PrintChar('m');
		PrintChar('f');
		PrintChar('\n');
		GPIO_ToggleBits(GPIOE,GPIO_Pin_3);
		msDelay(500);
	}

}

void usart_init(void)
{
	  /* Enable GPIO clock */
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	  /* Enable UART clock */
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	  /* Connect PXx to USARTx_Tx*/
	  GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_USART3);

	  /* Connect PXx to USARTx_Rx*/
	  GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_USART3);

	  /* Configure USART Tx as alternate function  */
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_Init(GPIOD, &GPIO_InitStructure);

	  /* Configure USART Rx as alternate function  */
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	  GPIO_Init(GPIOD, &GPIO_InitStructure);

	  USART_InitStructure.USART_BaudRate = 115200;
	  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	  USART_InitStructure.USART_StopBits = USART_StopBits_1;
	  USART_InitStructure.USART_Parity = USART_Parity_No;
	  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	  /* USART configuration */
	  USART_Init(USART3, &USART_InitStructure);

	  /* Enable USART */
	  USART_Cmd(USART3, ENABLE);
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
