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

GPIO_InitTypeDef  GPIO_InitStructure;
void Delay(__IO uint32_t nCount);

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
	while(1)
	{
		GPIO_ToggleBits(GPIOE,GPIO_Pin_3);
		Delay(0xFFFFFF);
	}

}

void Delay(__IO uint32_t nCount)
{
  while(nCount--)
  {
  }
}
