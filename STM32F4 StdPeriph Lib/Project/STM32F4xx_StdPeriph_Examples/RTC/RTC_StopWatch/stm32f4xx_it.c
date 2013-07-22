/**
  ******************************************************************************
  * @file    RTC/RTC_StopWatch/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    18-January-2013
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
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
#include "stm32f4xx_it.h"
#include "main.h"

/** @addtogroup STM32F4xx_StdPeriph_Examples
  * @{
  */
  
/** @addtogroup RTC_StopWatch
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define MESSAGE1   "Push Wakeup to start"

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint32_t uwBackupindex = 0;
__IO uint8_t  ubRTCCount = 0;
__IO uint8_t  ubSSecondfraction = 0;

extern __IO uint8_t ubStartevent;

RTC_TimeTypeDef RTC_StampTimeStruct;
RTC_TimeTypeDef RTC_TimeStructure;

/* Define the backup register */
uint32_t aBKPDataReg[20] = { RTC_BKP_DR0, RTC_BKP_DR1, RTC_BKP_DR3, RTC_BKP_DR4, 
                             RTC_BKP_DR5, RTC_BKP_DR6, RTC_BKP_DR7, RTC_BKP_DR8, 
                             RTC_BKP_DR9, RTC_BKP_DR10, RTC_BKP_DR11, RTC_BKP_DR12, 
                             RTC_BKP_DR13, RTC_BKP_DR14, RTC_BKP_DR15, RTC_BKP_DR16, 
                             RTC_BKP_DR17, RTC_BKP_DR18, RTC_BKP_DR19};

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f40xx.s/startup_stm32f427x.s).                         */
/******************************************************************************/

/**
  * @brief  This function handles External line 0 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI0_IRQHandler(void)
{
  if((EXTI_GetITStatus(WAKEUP_BUTTON_EXTI_LINE) != RESET) && (ubStartevent == 0))
  {
    /* Set the LCD Back Color */
    LCD_SetBackColor(White);

    /* Clear the LCD line 5 */
    LCD_ClearLine(Line5);
  
    /* Enable the RTC Clock */
    RCC_RTCCLKCmd(ENABLE);

    /* Wait for RTC APB registers synchronisation */
    RTC_WaitForSynchro();

    /* start count */
    ubStartevent = 1;
  }
  /* Clear the Wakeup EXTI pending bit */
  EXTI_ClearITPendingBit(WAKEUP_BUTTON_EXTI_LINE);  
}  

/**
  * @brief  This function handles External lines 15 to 10 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI15_10_IRQHandler(void)
{
  if((EXTI_GetITStatus(TAMPER_BUTTON_EXTI_LINE) != RESET) && (ubStartevent !=0) && (uwBackupindex < 11))
  {
    uint16_t Colorx;
    uint8_t index = 0;
    
    if (uwBackupindex < 10)
    {
      /* Increment counter */
      ubRTCCount++;
      
      /* Set LCD background color*/
      if((uint8_t)(ubRTCCount % 2) != 0)
      { 
        LCD_SetBackColor(Blue2);
        Colorx = White;
      }
      else
      {
        LCD_SetBackColor(Cyan);
        Colorx = Black;
      }
      
      /* Get the Current sub seconds and time */
      ubSSecondfraction = 1000 - ((uint32_t)((uint32_t)RTC_GetSubSecond() * 1000) / (uint32_t)0x3FF);
      RTC_GetTime(RTC_Format_BCD, &RTC_StampTimeStruct);
      
      LCD_SetFont(&Font12x12);
      
      /* Display result on the LCD */
      RTC_Time_display( LINE(7 + uwBackupindex), Colorx, RTC_Get_Time(ubSSecondfraction , &RTC_StampTimeStruct) ); 
      
      /* Save time register  to Backup register (the first 10 registers are reserved for time) */
      RTC_WriteBackupRegister(aBKPDataReg[uwBackupindex],(uint32_t)RTC->TR);
      
      /* Save sub second time stamp register (the latest 10 registers are reserved for sub second) */
      RTC_WriteBackupRegister(aBKPDataReg[uwBackupindex + 10], ubSSecondfraction);
    }
    else
    {      
      /* Set the LCD Background Color */
      LCD_SetBackColor(White);
      LCD_SetFont(&Font12x12);
      
      /* Clear all LCD lines from 7 to 19 */
      for (index = 0; index < 13; index++)
      {
        LCD_ClearLine(LINE(7 + index));
      }
      
      /* Reset Counters */
      ubRTCCount = 0;
      uwBackupindex = 0 ;
      
      /* Enter to idle */
      ubStartevent =0;
      
      /* Set the time to 00h 00mn 00s AM */
      RTC_TimeStructure.RTC_H12     = RTC_H12_AM;
      RTC_TimeStructure.RTC_Hours   = 0;
      RTC_TimeStructure.RTC_Minutes = 0;
      RTC_TimeStructure.RTC_Seconds = 0;  
      RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure);
      
      /* Disable the RTC Clock */
      RCC_RTCCLKCmd(DISABLE);
      LCD_SetFont(&Font16x24);
      
      /* Set the LCD Text Color */
      LCD_SetTextColor(Blue);
      
      /* Display message to the LCD */
      LCD_DisplayStringLine(LINE(5), (uint8_t *)MESSAGE1);
      
      /* Clear EXTI line 21 */
      EXTI_ClearITPendingBit(EXTI_Line21);
      
      /* Clear Tamper pin interrupt pending bit */
      RTC_ClearITPendingBit(RTC_IT_TAMP1);
    }  
    
    uwBackupindex++;
  }
  /* Clear the TAMPER EXTI pending bit */
  EXTI_ClearITPendingBit(TAMPER_BUTTON_EXTI_LINE);     
}

/**
  * @}
  */ 

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
