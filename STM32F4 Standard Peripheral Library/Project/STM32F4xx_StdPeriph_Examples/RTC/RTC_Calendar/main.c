/**
  ******************************************************************************
  * @file    RTC/RTC_Calendar/main.c 
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

/** @addtogroup RTC_Calendar
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Uncomment the corresponding line to select the RTC Clock source */
#define RTC_CLOCK_SOURCE_LSE          /* LSE used as RTC source clock */
/* #define RTC_CLOCK_SOURCE_LSI */    /* LSI used as RTC source clock. The RTC Clock
                                         may varies due to LSI frequency dispersion. */ 

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
RTC_TimeTypeDef  RTC_TimeStructure;
RTC_InitTypeDef  RTC_InitStructure;
RTC_AlarmTypeDef RTC_AlarmStructure;

__IO uint32_t uwAsynchPrediv = 0;
__IO uint32_t uwSynchPrediv = 0;

/* Private function prototypes -----------------------------------------------*/
static void RTC_Config(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  NVIC_InitTypeDef  NVIC_InitStructure;
  EXTI_InitTypeDef  EXTI_InitStructure;
  
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       files (startup_stm32f40xx.s/startup_stm32f427x.s) before to branch to 
       application main. 
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f4xx.c file
     */ 

  /* Configure the external interrupt "WAKEUP" and "TAMPER" buttons */
  STM_EVAL_PBInit(BUTTON_TAMPER , BUTTON_MODE_EXTI);
  STM_EVAL_PBInit(BUTTON_WAKEUP , BUTTON_MODE_EXTI);

  /* Configure LEDx */
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);
    
  /* Turn LED2 ON */
  STM_EVAL_LEDOn(LED2);
   
  if (RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x32F2)
  {  
    /* RTC configuration  */
    RTC_Config();
    /* Display the RTC Time and Alarm */
    RTC_TimeShow();
    RTC_AlarmShow();
  }
  else
  {
    /* Check if the Power On Reset flag is set */
    if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
    {
      /* Power On Reset occurred     */
      STM_EVAL_LEDOn(LED3);
    }
    /* Check if the Pin Reset flag is set */
    else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
    {
      /* External Reset occurred */
      STM_EVAL_LEDOn(LED4);
    }
    
    /* Enable the PWR clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    /* Allow access to RTC */
    PWR_BackupAccessCmd(ENABLE);

    /* Wait for RTC APB registers synchronisation */
    RTC_WaitForSynchro();

    /* Clear the RTC Alarm Flag */
    RTC_ClearFlag(RTC_FLAG_ALRAF);

    /* Clear the EXTI Line 17 Pending bit (Connected internally to RTC Alarm) */
    EXTI_ClearITPendingBit(EXTI_Line17);

    /* Display the RTC Time and Alarm */
    RTC_TimeShow();
    RTC_AlarmShow();
  }
   
  /* RTC Alarm A Interrupt Configuration */
  /* EXTI configuration *********************************************************/
  EXTI_ClearITPendingBit(EXTI_Line17);
  EXTI_InitStructure.EXTI_Line = EXTI_Line17;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
  /* Enable the RTC Alarm Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  while (1)
  {
  }
}

/**
  * @brief  Configure the RTC peripheral by selecting the clock source.
  * @param  None
  * @retval None
  */
static void RTC_Config(void)
{
  RTC_DateTypeDef RTC_DateStructure;
  
  /* Enable the PWR clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

  /* Allow access to RTC */
  PWR_BackupAccessCmd(ENABLE);
    
#if defined (RTC_CLOCK_SOURCE_LSI)  /* LSI used as RTC source clock*/
  /* The RTC Clock may varies due to LSI frequency dispersion */   
  /* Enable the LSI OSC */ 
  RCC_LSICmd(ENABLE);

  /* Wait till LSI is ready */  
  while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
  {
  }

  /* Select the RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
  
  /* ck_spre(1Hz) = RTCCLK(LSI) /(uwAsynchPrediv + 1)*(uwSynchPrediv + 1)*/
  uwSynchPrediv = 0xFF;
  uwAsynchPrediv = 0x7F;

#elif defined (RTC_CLOCK_SOURCE_LSE) /* LSE used as RTC source clock */
  /* Enable the LSE OSC */
  RCC_LSEConfig(RCC_LSE_ON);

  /* Wait till LSE is ready */  
  while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {
  }

  /* Select the RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
  /* ck_spre(1Hz) = RTCCLK(LSE) /(uwAsynchPrediv + 1)*(uwSynchPrediv + 1)*/
  uwSynchPrediv = 0xFF;
  uwAsynchPrediv = 0x7F;

#else
  #error Please select the RTC Clock source inside the main.c file
#endif /* RTC_CLOCK_SOURCE_LSI */
  
  /* Enable the RTC Clock */
  RCC_RTCCLKCmd(ENABLE);

  /* Wait for RTC APB registers synchronisation */
  RTC_WaitForSynchro();
  
  /* Configure the RTC data register and RTC prescaler */
  RTC_InitStructure.RTC_AsynchPrediv = uwAsynchPrediv;
  RTC_InitStructure.RTC_SynchPrediv = uwSynchPrediv;
  RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
  RTC_Init(&RTC_InitStructure);
  
  /* Set the alarm 05h:20min:30s */
  RTC_AlarmStructure.RTC_AlarmTime.RTC_H12     = RTC_H12_AM;
  RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours   = 0x05;
  RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = 0x20;
  RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = 0x30;
  RTC_AlarmStructure.RTC_AlarmDateWeekDay = 0x31;
  RTC_AlarmStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date;
  RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;
  
  /* Configure the RTC Alarm A register */
  RTC_SetAlarm(RTC_Format_BCD, RTC_Alarm_A, &RTC_AlarmStructure);
  
  /* Enable RTC Alarm A Interrupt */
  RTC_ITConfig(RTC_IT_ALRA, ENABLE);
  
  /* Enable the alarm */
  RTC_AlarmCmd(RTC_Alarm_A, ENABLE);
  
  RTC_ClearFlag(RTC_FLAG_ALRAF);
  
  /* Set the date: Friday January 11th 2013 */
  RTC_DateStructure.RTC_Year = 0x13;
  RTC_DateStructure.RTC_Month = RTC_Month_January;
  RTC_DateStructure.RTC_Date = 0x11;
  RTC_DateStructure.RTC_WeekDay = RTC_Weekday_Saturday;
  RTC_SetDate(RTC_Format_BCD, &RTC_DateStructure);
  
  /* Set the time to 05h 20mn 00s AM */
  RTC_TimeStructure.RTC_H12     = RTC_H12_AM;
  RTC_TimeStructure.RTC_Hours   = 0x05;
  RTC_TimeStructure.RTC_Minutes = 0x20;
  RTC_TimeStructure.RTC_Seconds = 0x00; 
  
  RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure);   
  
  /* Indicator for the RTC configuration */
  RTC_WriteBackupRegister(RTC_BKP_DR0, 0x32F2);
}

/**
  * @brief  Display the current time.
  * @param  None
  * @retval None
  */
void RTC_TimeShow(void)
{
  uint8_t showtime[50] = {0};
  /* Get the current Time */
  RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
  /* Display time Format : hh:mm:ss */
  sprintf((char*)showtime,"%0.2d:%0.2d:%0.2d",RTC_TimeStructure.RTC_Hours, RTC_TimeStructure.RTC_Minutes, RTC_TimeStructure.RTC_Seconds);
}

/**
  * @brief  Display the current Alarm.
  * @param  None
  * @retval None
  */
void RTC_AlarmShow(void)
{
  uint8_t showalarm[50] = {0};
  /* Get the current Alarm */
  RTC_GetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_AlarmStructure);
  sprintf((char*)showalarm,"%0.2d:%0.2d:%0.2d", RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours, RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes, RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds);
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
