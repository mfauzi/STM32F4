/**
  ******************************************************************************
  * <h2><center>&copy; COPYRIGHT 2012 Embest Tech. Co., Ltd.</center></h2>
  * @file    bsp.h 
  * @author  CMP Team
  * @version V1.0.0
  * @date    28-December-2012
  * @brief   board support package header file.                       
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef  __BSP_PRESENT__
#define  __BSP_PRESENT__

#ifdef   BSP_MODULE
#define  BSP_EXT
#else
#define  BSP_EXT  extern
#endif

/* Includes ------------------------------------------------------------------*/

#include  <cpu.h>
#include  <lib.h>


#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "ucos_ii.h"

/* Exported define ------------------------------------------------------------*/

#define  BSP_JOYSTICK_NONE                       DEF_BIT_NONE
#define  BSP_JOYSTICK_UP                         DEF_BIT_00
#define  BSP_JOYSTICK_DOWN                       DEF_BIT_01
#define  BSP_JOYSTICK_LEFT                       DEF_BIT_02
#define  BSP_JOYSTICK_RIGHT                      DEF_BIT_03
#define  BSP_JOYSTICK_CENTER                     DEF_BIT_04

#define  BSP_PB_ID_KEY                                     1
#define  BSP_PB_ID_WAKEUP                                  2
#define  BSP_PB_ID_TAMPER                                  3

#define  BSP_INT_ID_WWDG                                   0    /* Window WatchDog Interrupt                            */
#define  BSP_INT_ID_PVD                                    1    /* PVD through EXTI Line detection Interrupt            */
#define  BSP_INT_ID_TAMPER                                 2    /* Tamper Interrupt                                     */
#define  BSP_INT_ID_RTC                                    3    /* RTC global Interrupt                                 */
#define  BSP_INT_ID_FLASH                                  4    /* FLASH global Interrupt                               */
#define  BSP_INT_ID_RCC                                    5    /* RCC global Interrupt                                 */
#define  BSP_INT_ID_EXTI0                                  6    /* EXTI Line0 Interrupt                                 */
#define  BSP_INT_ID_EXTI1                                  7    /* EXTI Line1 Interrupt                                 */
#define  BSP_INT_ID_EXTI2                                  8    /* EXTI Line2 Interrupt                                 */
#define  BSP_INT_ID_EXTI3                                  9    /* EXTI Line3 Interrupt                                 */
#define  BSP_INT_ID_EXTI4                                 10    /* EXTI Line4 Interrupt                                 */
#define  BSP_INT_ID_DMA1_CH1                              11    /* DMA1 Channel 1 global Interrupt                      */
#define  BSP_INT_ID_DMA1_CH2                              12    /* DMA1 Channel 2 global Interrupt                      */
#define  BSP_INT_ID_DMA1_CH3                              13    /* DMA1 Channel 3 global Interrupt                      */
#define  BSP_INT_ID_DMA1_CH4                              14    /* DMA1 Channel 4 global Interrupt                      */
#define  BSP_INT_ID_DMA1_CH5                              15    /* DMA1 Channel 5 global Interrupt                      */
#define  BSP_INT_ID_DMA1_CH6                              16    /* DMA1 Channel 6 global Interrupt                      */
#define  BSP_INT_ID_DMA1_CH7                              17    /* DMA1 Channel 7 global Interrupt                      */
#define  BSP_INT_ID_ADC1_2                                18    /* ADC1 et ADC2 global Interrupt                        */
#define  BSP_INT_ID_USB_HP_CAN_TX                         19    /* USB High Priority or CAN TX Interrupts               */
#define  BSP_INT_ID_USB_LP_CAN_RX0                        20    /* USB Low Priority or CAN RX0 Interrupts               */
#define  BSP_INT_ID_CAN_RX1                               21    /* CAN RX1 Interrupt                                    */
#define  BSP_INT_ID_CAN_SCE                               22    /* CAN SCE Interrupt                                    */
#define  BSP_INT_ID_EXTI9_5                               23    /* External Line[9:5] Interrupts                        */
#define  BSP_INT_ID_TIM1_BRK                              24    /* TIM1 Break Interrupt                                 */
#define  BSP_INT_ID_TIM1_UP                               25    /* TIM1 Update Interrupt                                */
#define  BSP_INT_ID_TIM1_TRG_COM                          26    /* TIM1 Trigger and Commutation Interrupt               */
#define  BSP_INT_ID_TIM1_CC                               27    /* TIM1 Capture Compare Interrupt                       */
#define  BSP_INT_ID_TIM2                                  28    /* TIM2 global Interrupt                                */
#define  BSP_INT_ID_TIM3                                  29    /* TIM3 global Interrupt                                */
#define  BSP_INT_ID_TIM4                                  30    /* TIM4 global Interrupt                                */
#define  BSP_INT_ID_I2C1_EV                               31    /* I2C1 Event Interrupt                                 */
#define  BSP_INT_ID_I2C1_ER                               32    /* I2C1 Error Interrupt                                 */
#define  BSP_INT_ID_I2C2_EV                               33    /* I2C2 Event Interrupt                                 */
#define  BSP_INT_ID_I2C2_ER                               34    /* I2C2 Error Interrupt                                 */
#define  BSP_INT_ID_SPI1                                  35    /* SPI1 global Interrupt                                */
#define  BSP_INT_ID_SPI2                                  36    /* SPI2 global Interrupt                                */
#define  BSP_INT_ID_USART1                                37    /* USART1 global Interrupt                              */
#define  BSP_INT_ID_USART2                                38    /* USART2 global Interrupt                              */
#define  BSP_INT_ID_USART3                                39    /* USART3 global Interrupt                              */
#define  BSP_INT_ID_EXTI15_10                             40    /* External Line[15:10] Interrupts                      */
#define  BSP_INT_ID_RTCAlarm                              41    /* RTC Alarm through EXTI Line Interrupt                */
#define  BSP_INT_ID_USBWakeUp                             42    /* USB WakeUp from suspend through EXTI Line Interrupt  */
#define  BSP_INT_ID_TIM8_BRK                              43    /* TIM8 Break Interrupt                                 */
#define  BSP_INT_ID_TIM8_UP                               44    /* TIM8 Update Interrupt                                */
#define  BSP_INT_ID_TIM8_TRG_COM                          45    /* TIM8 Trigger and Commutation Interrupt               */
#define  BSP_INT_ID_TIM8_CC                               46    /* TIM8 Capture Compare Interrupt                       */
#define  BSP_INT_ID_ADC3                                  47    /* ADC3 global Interrupt                                */
#define  BSP_INT_ID_FSMC                                  48    /* FSMC global Interrupt                                */
#define  BSP_INT_ID_SDIO                                  49    /* SDIO global Interrupt                                */
#define  BSP_INT_ID_TIM5                                  50    /* TIM5 global Interrupt                                */
#define  BSP_INT_ID_SPI3                                  51    /* SPI3 global Interrupt                                */
#define  BSP_INT_ID_UART4                                 52    /* UART4 global Interrupt                               */
#define  BSP_INT_ID_UART5                                 53    /* UART5 global Interrupt                               */
#define  BSP_INT_ID_TIM6                                  54    /* TIM6 global Interrupt                                */
#define  BSP_INT_ID_TIM7                                  55    /* TIM7 global Interrupt                                */
#define  BSP_INT_ID_DMA2_CH1                              56    /* DMA2 Channel 1 global Interrupt                      */
#define  BSP_INT_ID_DMA2_CH2                              57    /* DMA2 Channel 2 global Interrupt                      */
#define  BSP_INT_ID_DMA2_CH3                              58    /* DMA2 Channel 3 global Interrupt                      */
#define  BSP_INT_ID_DMA2_CH4_5                            59    /* DMA2 Channel 4 and DMA2 Channel 5 global Interrupt   */

#define  BSP_PERIPH_ID_DMA1                                0
#define  BSP_PERIPH_ID_DMA2                                1
#define  BSP_PERIPH_ID_SRAM                                2
#define  BSP_PERIPH_ID_FLITF                               4
#define  BSP_PERIPH_ID_CRC                                 6
#define  BSP_PERIPH_ID_FSMC                                8
#define  BSP_PERIPH_ID_SDIO                               10

#define  BSP_PERIPH_ID_AFIO                               32
#define  BSP_PERIPH_ID_IOPA                               34
#define  BSP_PERIPH_ID_IOPB                               35
#define  BSP_PERIPH_ID_IOPC                               36
#define  BSP_PERIPH_ID_IOPD                               37
#define  BSP_PERIPH_ID_IOPE                               38
#define  BSP_PERIPH_ID_IOPF                               39
#define  BSP_PERIPH_ID_IOPG                               40
#define  BSP_PERIPH_ID_ADC1                               41
#define  BSP_PERIPH_ID_ADC2                               42
#define  BSP_PERIPH_ID_TIM1                               43
#define  BSP_PERIPH_ID_SPI1                               44
#define  BSP_PERIPH_ID_TIM8                               45
#define  BSP_PERIPH_ID_USART1                             46
#define  BSP_PERIPH_ID_ADC3                               47

#define  BSP_PERIPH_ID_TIM2                               64
#define  BSP_PERIPH_ID_TIM3                               65
#define  BSP_PERIPH_ID_TIM4                               66
#define  BSP_PERIPH_ID_TIM5                               67
#define  BSP_PERIPH_ID_TIM6                               68
#define  BSP_PERIPH_ID_TIM7                               69
#define  BSP_PERIPH_ID_WWDG                               75
#define  BSP_PERIPH_ID_SPI2                               78
#define  BSP_PERIPH_ID_SPI3                               79
#define  BSP_PERIPH_ID_USART2                             81
#define  BSP_PERIPH_ID_USART3                             82
#define  BSP_PERIPH_ID_USART4                             83
#define  BSP_PERIPH_ID_USART5                             84
#define  BSP_PERIPH_ID_I2C1                               85
#define  BSP_PERIPH_ID_I2C2                               86
#define  BSP_PERIPH_ID_USB                                87
#define  BSP_PERIPH_ID_CAN                                89
#define  BSP_PERIPH_ID_BKP                                91
#define  BSP_PERIPH_ID_PWR                                92
#define  BSP_PERIPH_ID_DAC                                93

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */


void         BSP_Init                    (void);
void         BSP_IntDisAll               (void);
CPU_INT32U   BSP_CPU_ClkFreq             (void);
void         BSP_IntInit                 (void);
void         BSP_IntEn                   (CPU_DATA       int_id);
void         BSP_IntDis                  (CPU_DATA       int_id);
void         BSP_IntClr                  (CPU_DATA       int_id);
void         BSP_IntVectSet              (CPU_DATA       int_id,
                                          CPU_FNCT_VOID  isr);
void         BSP_IntPrioSet              (CPU_DATA       int_id,
                                          CPU_INT08U     prio);
void         BSP_IntHandlerWWDG          (void);
void         BSP_IntHandlerPVD           (void);
void         BSP_IntHandlerTAMPER        (void);
void         BSP_IntHandlerRTC           (void);
void         BSP_IntHandlerFLASH         (void);
void         BSP_IntHandlerRCC           (void);
void         BSP_IntHandlerEXTI0         (void);
void         BSP_IntHandlerEXTI1         (void);
void         BSP_IntHandlerEXTI2         (void);
void         BSP_IntHandlerEXTI3         (void);
void         BSP_IntHandlerEXTI4         (void);
void         BSP_IntHandlerDMA1_CH1      (void);
void         BSP_IntHandlerDMA1_CH2      (void);
void         BSP_IntHandlerDMA1_CH3      (void);
void         BSP_IntHandlerDMA1_CH4      (void);
void         BSP_IntHandlerDMA1_CH5      (void);

void         BSP_IntHandlerDMA1_CH6      (void);
void         BSP_IntHandlerDMA1_CH7      (void);
void         BSP_IntHandlerADC1_2        (void);
void         BSP_IntHandlerUSB_HP_CAN_TX (void);
void         BSP_IntHandlerUSB_LP_CAN_RX0(void);
void         BSP_IntHandlerCAN_RX1       (void);
void         BSP_IntHandlerCAN_SCE       (void);
void         BSP_IntHandlerEXTI9_5       (void);
void         BSP_IntHandlerTIM1_BRK      (void);
void         BSP_IntHandlerTIM1_UP       (void);
void         BSP_IntHandlerTIM1_TRG_COM  (void);
void         BSP_IntHandlerTIM1_CC       (void);
void         BSP_IntHandlerTIM2          (void);
void         BSP_IntHandlerTIM3          (void);
void         BSP_IntHandlerTIM4          (void);
void         BSP_IntHandlerI2C1_EV       (void);

void         BSP_IntHandlerI2C1_ER       (void);
void         BSP_IntHandlerI2C2_EV       (void);
void         BSP_IntHandlerI2C2_ER       (void);
void         BSP_IntHandlerSPI1          (void);
void         BSP_IntHandlerSPI2          (void);
void         BSP_IntHandlerUSART1        (void);
void         BSP_IntHandlerUSART2        (void);
void         BSP_IntHandlerUSART3        (void);
void         BSP_IntHandlerEXTI15_10     (void);
void         BSP_IntHandlerRTCAlarm      (void);
void         BSP_IntHandlerUSBWakeUp     (void);
void         BSP_IntHandlerTIM8_BRK      (void);
void         BSP_IntHandlerTIM8_UP       (void);
void         BSP_IntHandlerTIM8_TRG_COM  (void);
void         BSP_IntHandlerTIM8_CC       (void);
void         BSP_IntHandlerADC3          (void);

void         BSP_IntHandlerFSMC          (void);
void         BSP_IntHandlerSDIO          (void);
void         BSP_IntHandlerTIM5          (void);
void         BSP_IntHandlerSPI3          (void);
void         BSP_IntHandlerUART4         (void);
void         BSP_IntHandlerUART5         (void);
void         BSP_IntHandlerTIM6          (void);
void         BSP_IntHandlerTIM7          (void);
void         BSP_IntHandlerDMA2_CH1      (void);
void         BSP_IntHandlerDMA2_CH2      (void);
void         BSP_IntHandlerDMA2_CH3      (void);
void         BSP_IntHandlerDMA2_CH4_5    (void);

CPU_INT32U   BSP_PeriphClkFreqGet        (CPU_DATA       pwr_clk_id);
void         BSP_PeriphEn                (CPU_DATA       pwr_clk_id);
void         BSP_PeriphDis               (CPU_DATA       pwr_clk_id);

CPU_INT16U   BSP_ADC_GetStatus           (CPU_INT08U     led);
CPU_INT32U   BSP_Joystick_GetStatus      (void);

void         BSP_LED_On                  (CPU_INT08U     led);
void         BSP_LED_Off                 (CPU_INT08U     led);
void         BSP_LED_Toggle              (CPU_INT08U     led);
CPU_BOOLEAN  BSP_PB_GetStatus            (CPU_INT08U     pb);
void         BSP_NAND_Init               (void);
void         BSP_NOR_Init                (void);


#endif /*__BSP_PRESENT__*/

/******************** COPYRIGHT 2012 Embest Tech. Co., Ltd.*****END OF FILE****/
