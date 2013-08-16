/**
  ******************************************************************************
  * <h2><center>&copy; COPYRIGHT 2012 Embest Tech. Co., Ltd.</center></h2>
  * @file    bsp.c 
  * @author  CMP Team
  * @version V1.0.0
  * @date    28-December-2012
  * @brief   board support package.                        
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
#define  BSP_MODULE
#include "bsp.h"
#include "stm32f4_discovery.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
                                                                /* -------------------- GPIOA PINS -------------------- */
#define  BSP_GPIOA_PB_WAKEUP                      DEF_BIT_00
#define  BSP_GPIOA_USART2_CTS                     DEF_BIT_00    /* USART2.                                              */
#define  BSP_GPIOA_USART2_RTS                     DEF_BIT_01    /* USART2.                                              */
#define  BSP_GPIOA_USART2_TX                      DEF_BIT_02    /* USART2.                                              */
#define  BSP_GPIOA_USART2_RX                      DEF_BIT_03    /* USART2.                                              */
#define  BSP_GPIOA_LCD_BL                         DEF_BIT_08    /* LCD.                                                 */
#define  BSP_GPIOA_USART1_TX                      DEF_BIT_09    /* USART1.                                              */
#define  BSP_GPIOA_USART1_RX                      DEF_BIT_10    /* USART1.                                              */
#define  BSP_GPIOA_USB_DM                         DEF_BIT_11    /* USB.                                                 */
#define  BSP_GPIOA_USB_DP                         DEF_BIT_12    /* USB.                                                 */

                                                                /* -------------------- GPIOB PINS -------------------- */
#define  BSP_GPIOB_SMARTCARD_PWR_SEL              DEF_BIT_00    /* Smartcard.                                           */
#define  BSP_GPIOB_SMARTCARD_IOUC                 DEF_BIT_10    /* Smartcard.                                           */
#define  BSP_GPIOB_SMARTCARD_RSTIN                DEF_BIT_11    /* Smartcard.                                           */
#define  BSP_GPIOB_SMARTCARD_XTAL1                DEF_BIT_12    /* Smartcard.                                           */
#define  BSP_GPIOB_USB_DISCONNECT                 DEF_BIT_14    /* USB.                                                 */

                                                                /* -------------------- GPIOC PINS -------------------- */
#define  BSP_GPIOC_ANALOG_CN5                     DEF_BIT_01    /* Analog Input.                                        */
#define  BSP_GPIOC_ANALOG_CN3                     DEF_BIT_02    /* Analog Input.                                        */
#define  BSP_GPIOC_ANALOG_CN2                     DEF_BIT_03    /* Analog Input.                                        */
#define  BSP_GPIOC_POT                            DEF_BIT_04    /* Potentiometer.                                       */
#define  BSP_GPIOC_SMARTCARD_CMDVCC               DEF_BIT_06    /* Smartcard.                                           */
#define  BSP_GPIOC_SMARTCARD_OFF                  DEF_BIT_07    /* Smartcard.                                           */
#define  BSP_GPIOC_MICROSD_D0                     DEF_BIT_08    /* MicroSD.                                             */
#define  BSP_GPIOC_MICROSD_D1                     DEF_BIT_09    /* MicroSD.                                             */
#define  BSP_GPIOC_MICROSD_D2                     DEF_BIT_10    /* MicroSD.                                             */
#define  BSP_GPIOC_MICROSD_D3                     DEF_BIT_11    /* MicroSD.                                             */
#define  BSP_GPIOC_MICROSD_CLK                    DEF_BIT_12    /* MicroSD.                                             */
#define  BSP_GPIOC_PB_TAMPER                      DEF_BIT_13

                                                                /* -------------------- GPIOD PINS -------------------- */
#define  BSP_GPIOD_MICROSD_CMD                    DEF_BIT_02    /* MicroSD.                                             */
#define  BSP_GPIOD_JOY_DOWN                       DEF_BIT_03    /* Joystick.                                            */


                                                                /* -------------------- GPIOF PINS -------------------- */
#define  BSP_GPIOF_LED1                           DEF_BIT_06    /* LED.                                                 */
#define  BSP_GPIOF_LED2                           DEF_BIT_07    /* LED.                                                 */
#define  BSP_GPIOF_LED3                           DEF_BIT_08    /* LED.                                                 */
#define  BSP_GPIOF_LED4                           DEF_BIT_09    /* LED.                                                 */

/* Private function prototypes -----------------------------------------------*/
static  void  BSP_PB_Init      (void);

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Initialize the Board Support Package (BSP).
  * @param  None
  * @retval None
  */

void  BSP_Init (void)
{
  BSP_IntInit();

  STM_EVAL_LEDInit(LED4);
  STM_EVAL_LEDInit(LED5);
  STM_EVAL_LEDInit(LED6);
  STM_EVAL_LEDInit(LED3);

  BSP_PB_Init();
}

/**
  * @brief  Read CPU registers to determine the CPU clock frequency of the chip.
  * @param  None
  * @retval The CPU clock frequency, in Hz.
  */
CPU_INT32U  BSP_CPU_ClkFreq (void)
{
  RCC_ClocksTypeDef  rcc_clocks;

  RCC_GetClocksFreq(&rcc_clocks);

  return ((CPU_INT32U)rcc_clocks.HCLK_Frequency);
}

/**
  * @brief  Get system tick clock frequency.
  * @param  None
  * @retval None
  */
INT32U  OS_CPU_SysTickClkFreq (void)
{
  INT32U  freq;

  freq = BSP_CPU_ClkFreq();
  return (freq);
}

/**
  * @brief  Initialize the board's PB.
  * @param  None
  * @retval None
  */
static  void  BSP_PB_Init (void)
{
  STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_GPIO);
}

/**
  * @brief  Get the status of a push button on the board.
  * @param  pb  The ID of the push button to probe
  *            1   probe the user push button
  * @retval DEF_FALSE if the push button is pressed.
  *         DEF_TRUE  if the push button is not pressed.
  */
CPU_BOOLEAN  BSP_PB_GetStatus (CPU_INT08U pb)
{
  CPU_BOOLEAN  status = 0;

  return (status);
}

/**
  * @brief  Initialize FSMC for NAND flash.
  * @param  None
  * @retval None
  */
void  BSP_NAND_Init (void)
{
                 
}

/**
  * @brief  Initialize FSMC for NOR flash.
  * @param  None
  * @retval None
  */
void  BSP_NOR_Init (void)
{
   
}

/******************** COPYRIGHT 2012 Embest Tech. Co., Ltd.*****END OF FILE****/
