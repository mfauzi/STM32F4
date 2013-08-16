/**
  ******************************************************************************
  * @file    ADC/ADC_VBATMeasurement/main.c 
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

/** @addtogroup ADC_VBATMeasurement
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint16_t uhADCConvertedValue = 0;
__IO uint32_t uwVBATVoltage = 0;

/* Private function prototypes -----------------------------------------------*/
static void ADC_Config(void);

#ifdef USE_LCD
static void Display_Init(void);
static void Display(void);
static void delay(__IO uint32_t nCount);
#endif /* USE_LCD */

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
  
#ifdef USE_LCD
  /* LCD Display init  */
  Display_Init();
#endif /* USE_LCD */

  /* ADC1 Channel Vbat configuration */
  ADC_Config();
 
  /* Start ADC1 Software Conversion */ 
  ADC_SoftwareStartConv(ADC1);

  while (1)
  {
#ifdef USE_LCD
  /* Display ADC converted value on LCD */
    Display();
#endif /* USE_LCD */
  }
}

/**
  * @brief  ADC1 Channel Vbat configuration
  * @note   This function Configure the ADC peripheral  
            1) Enable peripheral clocks
            2) DMA2_Stream0 channel 0 configuration
            3) Configure ADC1 Channel18 (VBAT) 
  * @param  None
  * @retval None
  */
static void ADC_Config(void)
{
  ADC_InitTypeDef       ADC_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
  DMA_InitTypeDef       DMA_InitStructure;
    
  /* Enable peripheral clocks *************************************************/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
  RCC_APB2PeriphClockCmd(ADCx_CLK, ENABLE);

  /* DMA2_Stream0 channel0 configuration **************************************/
  DMA_DeInit(DMA2_Stream0);
  DMA_InitStructure.DMA_Channel = DMA_CHANNELx;  
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADCx_DR_ADDRESS;
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&uhADCConvertedValue;
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
  /* DMA2_Stream0 enable */
  DMA_Cmd(DMA_STREAMx, ENABLE);
    
  /* ADC Common Init **********************************************************/
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
  ADC_CommonInit(&ADC_CommonInitStructure);

  /* ADC1 Init ****************************************************************/
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;
  ADC_Init(ADCx, &ADC_InitStructure);

  /* Enable ADC1 DMA */
  ADC_DMACmd(ADCx, ENABLE);
  
  /* ADC1 regular channel18 (VBAT) configuration ******************************/
  ADC_RegularChannelConfig(ADCx, ADC_Channel_Vbat, 1, ADC_SampleTime_15Cycles);

  /* Enable VBAT channel */
  ADC_VBATCmd(ENABLE); 

  /* Enable DMA request after last transfer (Single-ADC mode) */
  ADC_DMARequestAfterLastTransferCmd(ADCx, ENABLE);

  /* Enable ADC1 **************************************************************/
  ADC_Cmd(ADCx, ENABLE);
}

#ifdef USE_LCD
/**
  * @brief  Display ADC converted value on LCD
  * @param  None
  * @retval None
  */
void Display(void)
{
   uint32_t uwVbatV=0;
   uint32_t uwVbatmV=0;
   uint8_t aTextBuffer[50];

  /*The VBAT pin is internally connected to a bridge divider by 2 */
  uwVBATVoltage = (uhADCConvertedValue*2)*3300/0xFFF;
  uwVbatV = uwVBATVoltage/1000;
  uwVbatmV = (uwVBATVoltage%1000)/100;
  sprintf((char*)aTextBuffer,"       %d,%d V   ", uwVbatV, uwVbatmV);
  LCD_DisplayStringLine(LCD_LINE_6, aTextBuffer);

  delay(100);
}

/**
  * @brief  Display Init (LCD)
  * @param  None
  * @retval None
  */
void Display_Init(void)
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

  LCD_DisplayStringLine(LCD_LINE_19, (uint8_t*)" STM32F4xx ADC1 VBAT Measurement example");

  /* Set the LCD Text size */
  LCD_SetFont(&Font16x24);

  LCD_DisplayStringLine(LCD_LINE_0, (uint8_t*)"**VBAT Measurement**");

  /* Set the LCD Back Color and Text Color*/
  LCD_SetBackColor(White);
  LCD_SetTextColor(Blue);

  LCD_DisplayStringLine(LCD_LINE_2, (uint8_t*)" Eval Board Instant ");
  LCD_DisplayStringLine(LCD_LINE_4, (uint8_t*)"  Battery Voltage   ");
}

/**
  * @brief  Inserts a delay time
  * @param  nCount: specifies the delay time length
  * @retval None
  */
static void delay(__IO uint32_t nCount)
{
  __IO uint32_t index = 0; 
  for(index = (100000 * nCount); index != 0; index--)
  {
  }
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
