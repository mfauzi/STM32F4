/**
  ******************************************************************************
  * @file    I2S/I2S_Audio/main.c 
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

/** @addtogroup I2S_Audio
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#if defined (USE_STM324xG_EVAL)
  #define MESSAGE1   "     STM32F40xx     "
  #define MESSAGE2   " Device running on  " 
  #define MESSAGE3   "   STM324xG-EVAL    "

#else /* USE_STM324x7I_EVAL */ 
  #define MESSAGE1   "     STM32F427x     "
  #define MESSAGE2   " Device running on  " 
  #define MESSAGE3   "  STM324x7I-EVAL    "
#endif  

/* Audio file size and start address are defined here since the audio file is 
   stored in Flash memory as a constant table of 16-bit data */
#define AUDIO_FILE_SZE          990000
#define AUIDO_START_ADDRESS     58 /* Offset relative to audio file header size */



/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

static __IO uint32_t uwTimingDelay;
RCC_ClocksTypeDef RCC_Clocks;

__IO uint32_t uwCommand = AUDIO_PAUSE;
__IO uint32_t uwVolume = 70;

/* Variable to indicate that push buttons will be used for switching between 
   Headphone and Speaker output modes. */
uint32_t uwSpHpSwitch = 0;

/* Private function prototypes -----------------------------------------------*/
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
       file (startup_stm32f40xx.s/startup_stm32f427x.s) before to branch to 
       application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f4xx.c file
     */
  
  /* Initialize LEDs, Key Button, LCD available on EVAL board *****************/
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);

  /* Initialize the Push buttons */
  /* Key button used for Pause/Resume */
  STM_EVAL_PBInit(BUTTON_KEY, BUTTON_MODE_GPIO); 
  /* Key button used for Volume High */    
  STM_EVAL_PBInit(BUTTON_WAKEUP, BUTTON_MODE_GPIO); 
  /* Key button used for Volume Low */ 
  STM_EVAL_PBInit(BUTTON_TAMPER, BUTTON_MODE_GPIO);  
    
  /* Initialize the LCD */
  LCD_Init();
  
  /* Display message on EVAL LCD **********************************************/
  /* Clear the LCD */ 
  LCD_Clear(LCD_COLOR_BLUE);  

  /* Set the LCD Back Color */
  LCD_SetBackColor(Blue);
  /* Set the LCD Text Color */
  LCD_SetTextColor(White);
  LCD_DisplayStringLine(Line0, (uint8_t *)MESSAGE1);
  LCD_DisplayStringLine(Line1, (uint8_t *)MESSAGE2);
  LCD_DisplayStringLine(Line2, (uint8_t *)MESSAGE3);

  /* Turn on leds available on EVAL *******************************************/
  STM_EVAL_LEDOn(LED1);
  STM_EVAL_LEDOn(LED2);
  STM_EVAL_LEDOn(LED3);
  STM_EVAL_LEDOn(LED4);

  /* SysTick end of count event each 10ms */
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 100);

  /* Initialize the Audio codec and all related peripherals (I2S, I2C, IOExpander, IOs...) */  
  if (EVAL_AUDIO_Init(OUTPUT_DEVICE_AUTO, uwVolume, I2S_AudioFreq_48k) == 0)
  {
    LCD_DisplayStringLine(Line3, (uint8_t *)"====================");
    LCD_DisplayStringLine(Line4, (uint8_t *)"Key   : Play/Pause ");
    LCD_DisplayStringLine(Line5, (uint8_t *)"Tamper: Vol+/Headph");
    LCD_DisplayStringLine(Line6, (uint8_t *)"Wakeup: Vol-/Spkr  ");
    LCD_DisplayStringLine(Line7, (uint8_t *)"====================");
    LCD_DisplayStringLine(Line8, (uint8_t *)"  AUDIO CODEC   OK  ");    
  }
  else
  {
    LCD_DisplayStringLine(Line4, (uint8_t *)"  AUDIO CODEC  FAIL ");
    LCD_DisplayStringLine(Line5, (uint8_t *)" Try to reset board ");
  }
  
  
  /* 
  Normal mode description:
      Start playing the audio file (using DMA stream) .
      Using this mode, the application can run other tasks in parallel since 
      the DMA is handling the Audio Transfer instead of the CPU.
      The only task remaining for the CPU will be the management of the DMA 
      Transfer Complete interrupt or the Half Transfer Complete interrupt in 
      order to load again the buffer and to calculate the remaining data.  
  Circular mode description:
     Start playing the file from a circular buffer, once the DMA is enabled it 
     always run. User has to fill periodically the buffer with the audio data 
     using Transfer complete and/or half transfer complete interrupts callbacks 
     (EVAL_AUDIO_TransferComplete_CallBack() or EVAL_AUDIO_HalfTransfer_CallBack()...
     In this case the audio data file is smaller than the DMA max buffer 
     size 65535 so there is no need to load buffer continuously or manage the 
     transfer complete or Half transfer interrupts callbacks. */
  EVAL_AUDIO_Play((uint16_t*)(AUDIO_SAMPLE + AUIDO_START_ADDRESS), (AUDIO_FILE_SZE - AUIDO_START_ADDRESS));
 
  /* Display the state on the screen */
  LCD_DisplayStringLine(Line8, (uint8_t *)"       PLAYING      ");
  
  /* Infinite loop */
  while (1)
  {    
    /* Check on the Pause/Resume button */
    if (STM_EVAL_PBGetState(BUTTON_KEY) != Bit_SET)
    {
      /* wait to avoid rebound */
      while (STM_EVAL_PBGetState(BUTTON_KEY) != Bit_SET);
      
      EVAL_AUDIO_PauseResume(uwCommand);
      if (uwCommand == AUDIO_PAUSE)
      {
        /* Display the current state of the player */
        LCD_DisplayStringLine(Line8, (uint8_t *)"       PAUSED       ");
        
        /* Next time Resume command should be processed */
        uwCommand = AUDIO_RESUME;
        
        /* Push buttons will be used to switch between Speaker and Headphone modes */
        uwSpHpSwitch = 1;
      }
      else
      {
        /* Display the current state of the player */
        LCD_DisplayStringLine(Line8, (uint8_t *)"       PLAYING      ");
        
        /* Next time Pause command should be processed */
        uwCommand = AUDIO_PAUSE;
        
        /* Push buttons will be used to control volume level */
        uwSpHpSwitch = 0;
      }
    }
    
    /* Check on the Volume high button */
    if (STM_EVAL_PBGetState(BUTTON_WAKEUP) == Bit_SET)
    {
      /* Check if the current state is paused (push buttons are used for volume control or for 
         speaker/headphone mode switching) */
      if (uwSpHpSwitch)
      {
        /* Set output to Speaker */
        Codec_SwitchOutput(OUTPUT_DEVICE_SPEAKER);
        
        /* Display the current state of the player */
        LCD_DisplayStringLine(Line9, (uint8_t *)"       SPEAKER      ");
      }
      else
      {
        /* wait to avoid rebound */
        while (STM_EVAL_PBGetState(BUTTON_WAKEUP) == Bit_SET);
        
        /* Decrease volume by 5% */
        if (uwVolume > 5)
          uwVolume -= 5; 
        else
          uwVolume = 0; 
        
        /* Apply the new volume to the codec */
        EVAL_AUDIO_VolumeCtl(uwVolume);
        LCD_DisplayStringLine(Line9, (uint8_t *)"       VOL:   -     ");
      }
    }    
    
    /* Check on the Volume high button */
    if (STM_EVAL_PBGetState(BUTTON_TAMPER) != Bit_SET)
    {
      /* Check if the current state is paused (push buttons are used for volume control or for 
         speaker/headphone mode switching) */
      if (uwSpHpSwitch)
      {
        /* Set output to Headphone */
        Codec_SwitchOutput(OUTPUT_DEVICE_HEADPHONE);
        
        /* Display the current state of the player */
        LCD_DisplayStringLine(Line9, (uint8_t *)"      HEADPHONE     ");
      }
      else
      {
        /* wait to avoid rebound */
        while (STM_EVAL_PBGetState(BUTTON_TAMPER) != Bit_SET);
        
        /* Increase volume by 5% */
        if (uwVolume < 95)
          uwVolume += 5; 
        else
          uwVolume = 100; 
        
        /* Apply the new volume to the codec */
        EVAL_AUDIO_VolumeCtl(uwVolume);
        LCD_DisplayStringLine(Line9, (uint8_t *)"       VOL:   +     ");  
      }
    }  
    
    /* Toggle LD4 */
    STM_EVAL_LEDToggle(LED3);

    /* Insert 50 ms delay */
    Delay(5);

    /* Toggle LD2 */
    STM_EVAL_LEDToggle(LED2);

    /* Insert 50 ms delay */
    Delay(5);
  }
}

/*------------------------------------------------------------------------------
       Callbacks implementation:
           the callbacks prototypes are defined in the stm324xg_eval_audio_codec.h /
           stm324x7i_eval_audio_codec.h file and their implementation should be done
           the user coed if they are needed.
           Below some examples of callback implementations.
  ----------------------------------------------------------------------------*/
/**
  * @brief  Calculates the remaining file size and new position of the pointer.
  * @param  None
  * @retval None
  */
void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size)
{
  /* Calculate the remaining audio data in the file and the new size 
     for the DMA transfer. If the Audio files size is less than the DMA max 
     data transfer size, so there is no calculation to be done, just restart 
     from the beginning of the file ... */
  /* Check if the end of file has been reached */

#ifdef AUDIO_MAL_MODE_NORMAL  

  /* Replay from the beginning */
  EVAL_AUDIO_Play((uint16_t*)(AUDIO_SAMPLE + AUIDO_START_ADDRESS), (AUDIO_FILE_SZE - AUIDO_START_ADDRESS));
  
#else /* #ifdef AUDIO_MAL_MODE_CIRCULAR */

  /* Display message on the LCD screen */
  LCD_DisplayStringLine(Line8, " All Buffer Reached ");   

#endif /* AUDIO_MAL_MODE_CIRCULAR */
}

/**
  * @brief  Manages the DMA Half Transfer complete interrupt.
  * @param  None
  * @retval None
  */
void EVAL_AUDIO_HalfTransfer_CallBack(uint32_t pBuffer, uint32_t Size)
{  
#ifdef AUDIO_MAL_MODE_CIRCULAR
  
  /* Display message on the LCD screen */
  LCD_DisplayStringLine(Line8, " 1/2 Buffer Reached "); 
  
#endif /* AUDIO_MAL_MODE_CIRCULAR */
  
  /* Generally this interrupt routine is used to load the buffer when 
  a streaming scheme is used: When first Half buffer is already transferred load 
  the new data to the first half of buffer while DMA is transferring data from 
  the second half. And when Transfer complete occurs, load the second half of 
  the buffer while the DMA is transferring from the first half ... */
  /* 
    ...........
                   */
}

/**
  * @brief  Manages the DMA FIFO error interrupt.
  * @param  None
  * @retval None
  */
void EVAL_AUDIO_Error_CallBack(void* pData)
{
  /* Display message on the LCD screen */
  LCD_SetBackColor(Red);
  LCD_DisplayStringLine(Line8, (uint8_t *)"     DMA  ERROR     ");
  
  /* Stop the program with an infinite loop */
  while (1)
  {}
  
  /* could also generate a system reset to recover from the error */
  /* .... */
}

#ifndef USE_DEFAULT_TIMEOUT_CALLBACK
/**
  * @brief  Basic management of the timeout situation.
  * @param  None.
  * @retval None.
  */
uint32_t Codec_TIMEOUT_UserCallback(void)
{
  /* Display message on the LCD screen */
  LCD_DisplayStringLine(Line8, (uint8_t *)"  CODEC I2C  ERROR  ");  
    
  /* Block communication and all processes */
  while (1)
  {   
  }
}
#endif /* USE_DEFAULT_TIMEOUT_CALLBACK */
/*----------------------------------------------------------------------------*/

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in 10 ms.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
{
  uwTimingDelay = nTime;
  
  while(uwTimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (uwTimingDelay != 0x00)
  { 
    uwTimingDelay--;
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
