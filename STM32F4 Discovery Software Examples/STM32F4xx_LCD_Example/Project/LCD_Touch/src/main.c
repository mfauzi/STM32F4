/**
  ******************************************************************************
  * <h2><center>&copy; COPYRIGHT 2012 Embest Tech. Co., Ltd.</center></h2>
  * @file    main.c
  * @author  CMP Team
  * @version V1.0.0
  * @date    28-December-2012
  * @brief   LCD touch Demo      
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, Embest SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
  * OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
  * OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
  * CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "stm32f4_discovery_lcd.h"
#include "stmpe811qtr.h"
#include "LCD_Touch_Calibration.h"


void delay(__IO uint32_t nCount);

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  IOE_Config();

  /* Initialize the LCD */
  STM32f4_Discovery_LCD_Init();
	
  /* Clear the LCD */ 
  LCD_Clear(White);
	
  /* Set the LCD Text size */
  LCD_SetFont(&Font8x12);

  Lcd_Touch_Calibration();

  while (1) {
    Calibration_Test_Dispose();
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
  while (1);
}
#endif

/**
  * @}
  */ 


/**
  * @brief  Inserts a delay time.
  * @param  nCount: specifies the delay time length.
  * @retval None
  */
void delay(__IO uint32_t nCount)
{
  __IO uint32_t index = 0; 
  for (index = (100000 * nCount); index != 0; index--);
}


/******************** COPYRIGHT 2012 Embest Tech. Co., Ltd.*****END OF FILE****/
