/**
  ******************************************************************************
  * @file    ADC/ADC_DualModeInterleaved/main.c
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

/** @addtogroup ADC_DualModeInterleaved
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint16_t uhADCDualConvertedValue;

/* Private function prototypes -----------------------------------------------*/
static void ADC_Config(void);

#ifdef USE_LCD
static void Display_Init(void);
static void Display(void);
#endif /* USE_LCD */

/* Private functions ---------------------------------------------------------*/

/**
  * @brief   Main program
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
#ifdef USE_LCD
  /* LCD Display init  */
  Display_Init();
#endif /* USE_LCD */

  /* ADC configuration */
  ADC_Config();

  /* Start ADC1 Software Conversion */
  ADC_SoftwareStartConv(ADC1);

  while (1)
  {
#ifdef USE_LCD
  /* Display ADCs converted values on LCD */
    Display();
#endif /* USE_LCD */
  }
}

/**
  * @brief  ADC configuration
  * @note   This function Configure the ADC peripheral  
            1) Enable peripheral clocks
            2) Configure ADC Channel 12 pin as analog input
            3) DMA2_Stream0 channel2 configuration
            4) Configure ADC1 Channel 12
            5) Configure ADC2 Channel 12             
  * @param  None
  * @retval None
  */
static void ADC_Config(void)
{

  ADC_InitTypeDef ADC_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
  DMA_InitTypeDef DMA_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable peripheral clocks */
  RCC_AHB1PeriphClockCmd( ADC1_2_CHANNEL_GPIO_CLK , ENABLE);
  RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_DMA2 , ENABLE);
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_ADC1 , ENABLE);
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_ADC2 , ENABLE);
  
  /* Configure ADC Channel 12 pin as analog input */
  GPIO_InitStructure.GPIO_Pin = GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIO_PORT, &GPIO_InitStructure);

  /* DMA2 Stream0 channel 0 configuration */
  DMA_InitStructure.DMA_Channel = DMA_CHANNELx;  
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC_CDR_ADDRESS;
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&uhADCDualConvertedValue;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize = 1;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA_STREAMx, &DMA_InitStructure);

  /* DMA2_Stream 0 enable */
  DMA_Cmd(DMA_STREAMx, ENABLE);

/******************************************************************************/
/*  ADCs configuration: double interleaved with 6cycles delay to reach 5Msps  */
/******************************************************************************/

  /* ADC Common configuration *************************************************/
  ADC_CommonInitStructure.ADC_Mode = ADC_DualMode_Interl;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_6Cycles;
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_3;
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
  ADC_CommonInit(&ADC_CommonInitStructure);  
  
  /* DMA mode 3 is used in interleaved mode in 6-bit or 8-bit resolutions */
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_8b;

  /* ADC1 regular channel 12 configuration ************************************/
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;	
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;
  ADC_Init(ADC1, &ADC_InitStructure);
  /* ADC1 regular channel 12 configuration */ 
  ADC_RegularChannelConfig(ADC1, ADC_CHANNEL, 1, ADC_SampleTime_3Cycles); 

  /* ADC2 regular channel 12 configuration ************************************/
  ADC_Init(ADC2, &ADC_InitStructure);
  /* ADC2 regular channel 12 configuration */
  ADC_RegularChannelConfig(ADC2, ADC_CHANNEL, 1, ADC_SampleTime_3Cycles);

  /* Enable DMA request after last transfer (multi-ADC mode) ******************/
  ADC_MultiModeDMARequestAfterLastTransferCmd(ENABLE);

  /* Enable ADC1 **************************************************************/
  ADC_Cmd(ADC1, ENABLE);

  /* Enable ADC2 **************************************************************/
  ADC_Cmd(ADC2, ENABLE);
} 
  

#ifdef USE_LCD
/**
  * @brief  Display ADCs converted values on LCD
  * @param  None
  * @retval None
  */
static void Display(void)
{
  uint32_t uwVoltage =0, uwMVoltage=0;
  uint8_t aTextBuffer[50];
  __IO uint32_t uwADC1ConvertedVoltage;
  __IO uint32_t uwADC2ConvertedVoltage;
  __IO uint8_t uwADC1ConvertedValue;
  __IO uint8_t uwADC2ConvertedValue;

  uwADC1ConvertedValue = (uhADCDualConvertedValue & 0x00FF);
  uwADC1ConvertedVoltage = uwADC1ConvertedValue *3300/0xFF;

  uwVoltage = (uwADC1ConvertedVoltage)/1000;
  uwMVoltage = (uwADC1ConvertedVoltage%1000)/100;
  sprintf((char*)aTextBuffer,"   ADC1 = %d,%d V   ",uwVoltage, uwMVoltage);
  LCD_DisplayStringLine(LCD_LINE_6, (uint8_t*)aTextBuffer);


  uwADC2ConvertedValue = (uhADCDualConvertedValue >>8);
  uwADC2ConvertedVoltage = uwADC2ConvertedValue *3300/0xFF;

  uwVoltage=uwADC2ConvertedVoltage/1000;
  uwMVoltage = (uwADC2ConvertedVoltage%1000)/100;
  sprintf((char*)aTextBuffer,"   ADC2 = %d,%d V   ",uwVoltage, uwMVoltage);
  LCD_DisplayStringLine(LCD_LINE_7, (uint8_t*)aTextBuffer);
}

/**
  * @brief  Display Init (LCD)
  * @param  None
  * @retval None
  */
static void Display_Init(void)
{

/* Initialize the LCD */
  LCD_Init();
  
  /* Clear the LCD */ 
  LCD_Clear(White);

  /* Set the LCD Text size */
  LCD_SetFont(&Font8x12);

  /* Set the LCD Back Color and Text Color*/
  LCD_SetBackColor(Blue);
  LCD_SetTextColor(White);

  LCD_DisplayStringLine(LCD_LINE_19, (uint8_t*)FOOTER_MESSAGE );


  /* Set the LCD Text size */
  LCD_SetFont(&Font16x24);

  LCD_DisplayStringLine(LCD_LINE_0, (uint8_t*)CONFIG1_MESSAGE);


  /* Set the LCD Back Color and Text Color*/
  LCD_SetBackColor(White);
  LCD_SetTextColor(Blue); 

  LCD_DisplayStringLine(LCD_LINE_2, (uint8_t*)CONFIG2_MESSAGE);
  LCD_DisplayStringLine(LCD_LINE_3, (uint8_t*)CONFIG3_MESSAGE);
}
#endif /* USE_LCD */


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
