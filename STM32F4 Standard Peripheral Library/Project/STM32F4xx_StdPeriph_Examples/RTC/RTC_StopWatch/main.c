/**
  ******************************************************************************
  * @file    RTC/RTC_StopWatch/main.c 
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
  
/** @addtogroup RTC_StopWatch
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define MESSAGE1   " **** STOPWATCH ****        " 
#define MESSAGE2   "Push Wakeup to Start        " 

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint32_t uwSecondfraction = 0;
__IO uint8_t  ubStartevent = 0;

RTC_InitTypeDef RTC_InitStructure;
RTC_TimeTypeDef RTC_TimeStruct; 

/* Private function prototypes -----------------------------------------------*/
static void RTC_Config(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
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
  
  /* Initialize the LCD */
  LCD_Init();
  LCD_Clear(White);
  
  /* Set the LCD Text Color */
  LCD_SetTextColor(Blue);
  
  /* Display message */
  LCD_DisplayStringLine(LCD_LINE_0, (uint8_t*)MESSAGE1);
  
  /* RTC configuration  */
  RTC_Config();

  /* Set the LCD Back Color */
  LCD_SetBackColor(White);
  
  /* Configure buttons on EVAL */
  STM_EVAL_PBInit(BUTTON_WAKEUP,BUTTON_MODE_EXTI);
  STM_EVAL_PBInit(BUTTON_TAMPER,BUTTON_MODE_EXTI);
  
  /* Set the LCD Text Color */
  LCD_SetTextColor(Blue);
  
  /* Display message */
  LCD_DisplayStringLine(LINE(5), (uint8_t *)MESSAGE2);
  
  /* Infinite loop */
  while (1)
  {
    /* Set The LCD font size */
    LCD_SetFont(&Font16x24);
    
    /* Set the LCD Back Color */
    LCD_SetBackColor(White);
    
    /* Check on the event 'start' */
    if(ubStartevent != 0)
    {
      /* Get the RTC sub seconds fraction */
      uwSecondfraction = 1000 - ((uint32_t)((uint32_t)RTC_GetSubSecond() * 1000) / (uint32_t)0x3FF);
    }
    else
    {
      /* Idle */
      uwSecondfraction = 0;
    }
    
    /* Get the Current time */
    RTC_GetTime(RTC_Format_BCD, &RTC_TimeStruct);
    
    /* Display the current time and the sub seconds on the LCD */
    RTC_Time_display(Line2, Black , RTC_Get_Time(uwSecondfraction , &RTC_TimeStruct));
  }
}

/**
  * @brief  Configures the RTC peripheral by selecting the clock source.
  * @param  None
  * @retval None
  */
static void RTC_Config(void)
{
  RTC_TimeTypeDef  RTC_TimeStructure;
  
  /* Enable the PWR clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

  /* Allow access to RTC */
  PWR_BackupAccessCmd(ENABLE);
  
  /* Reset RTC Domain */
  RCC_BackupResetCmd(ENABLE);
  RCC_BackupResetCmd(DISABLE);
  
  /* Enable the LSE OSC */
  RCC_LSEConfig(RCC_LSE_ON);

  /* Wait till LSE is ready */  
  while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {
  }

  /* Select the RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
  
  /* Configure the RTC data register and RTC prescaler */
  /* ck_spre(1Hz) = RTCCLK(LSI) /(AsynchPrediv + 1)*(SynchPrediv + 1)*/
  RTC_InitStructure.RTC_AsynchPrediv = 0x1F;
  RTC_InitStructure.RTC_SynchPrediv  = 0x3FF;
  RTC_InitStructure.RTC_HourFormat   = RTC_HourFormat_24;
  RTC_Init(&RTC_InitStructure);
  
  /* Set the time to 00h 00mn 00s AM */
  RTC_TimeStructure.RTC_H12     = RTC_H12_AM;
  RTC_TimeStructure.RTC_Hours   = 0;
  RTC_TimeStructure.RTC_Minutes = 0;
  RTC_TimeStructure.RTC_Seconds = 0;  
  RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure);
}

/**
  * @brief  Returns the current time and sub second.
  * @param  Secondfraction: the sub second fraction.
  * @param  RTC_TimeStructure : pointer to a RTC_TimeTypeDef structure that 
  *         contains the current time values. 
  * @retval table : return current time and sub second in a table form
  */
Table_TypeDef RTC_Get_Time(uint32_t Secondfraction , RTC_TimeTypeDef* RTC_TimeStructure)
{
  Table_TypeDef table2;

  /* Fill the table fields with the current Time*/
  table2.tab[0]   = (((uint8_t)(RTC_TimeStructure->RTC_Hours & 0xF0) >> 0x04) + 0x30);
  table2.tab[1]   = (((uint8_t)(RTC_TimeStructure->RTC_Hours & 0x0F))+ 0x30);
  table2.tab[2]   = 0x3A;
  
  table2.tab[3]   = (((uint8_t)(RTC_TimeStructure->RTC_Minutes & 0xF0) >> 0x04) + 0x30);
  table2.tab[4]   =(((uint8_t)(RTC_TimeStructure->RTC_Minutes & 0x0F))+ (uint8_t)0x30);
  table2.tab[5]   = 0x3A;

  table2.tab[6]   = (((uint8_t)(RTC_TimeStructure->RTC_Seconds & 0xF0) >> 0x04)+ 0x30);
  table2.tab[7]   = (((uint8_t)(RTC_TimeStructure->RTC_Seconds & 0x0F)) + 0x30);
  table2.tab[8]   = 0x2E;
  
  table2.tab[9]   = (uint8_t)((Secondfraction / 100) + 0x30);
  table2.tab[10]  = (uint8_t)(((Secondfraction % 100 ) / 10) + 0x30);
  table2.tab[11]  =  (uint8_t)((Secondfraction % 10) + 0x30);
  
  /* return table2 */
  return table2;
}

/**
  * @brief  Displays the current Time on the LCD.
  * @param  Line:  the Line where to display the Current time .
  *           This parameter can be one of the following values:
  *             @arg Linex: where x can be 0..9
  * @param  Color_x: specifies the Background Color.
  * @param  table: the Current time and sub second.
  * @retval None
  */
void RTC_Time_display(uint8_t Line,uint16_t Color_x, Table_TypeDef table )
{   
  uint8_t index = 0;

  /* Initialize table */
  LCD_SetTextColor(Color_x);
  
  for (index = 0; index < 12; index++)
  {
    /* Display char on the LCD */
    LCD_DisplayChar(Line, (290 - (20 *index)), table.tab[index]);
  }  
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
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
