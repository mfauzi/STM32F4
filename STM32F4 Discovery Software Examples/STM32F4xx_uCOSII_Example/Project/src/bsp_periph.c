/**
  ******************************************************************************
  * <h2><center>&copy; COPYRIGHT 2012 Embest Tech. Co., Ltd.</center></h2>
  * @file    bsp_periph.c 
  * @author  CMP Team
  * @version V1.0.0
  * @date    28-December-2012
  * @brief   board support package file.                        
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

#define  BSP_PERIPH_MODULE
#include <bsp.h>


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define  BSP_PERIPH_REG_RCC_BASE                 0x40021000
#define  BSP_PERIPH_REG_RCC_CFGR            (*(volatile  CPU_INT32U *)(BSP_PERIPH_REG_RCC_BASE + 0x004))
#define  BSP_PERIPH_REG_RCC_AHBENR          (*(volatile  CPU_INT32U *)(BSP_PERIPH_REG_RCC_BASE + 0x014))
#define  BSP_PERIPH_REG_RCC_APB2ENR         (*(volatile  CPU_INT32U *)(BSP_PERIPH_REG_RCC_BASE + 0x018))
#define  BSP_PERIPH_REG_RCC_APB1ENR         (*(volatile  CPU_INT32U *)(BSP_PERIPH_REG_RCC_BASE + 0x01C))

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Get clock frequency of a peripheral.
  * @param  pwr_clk_id      Power/clock ID.
  * @retval None
  */
CPU_INT32U  BSP_PeriphClkFreqGet (CPU_DATA  pwr_clk_id)
{
    CPU_INT32U  clk_freq;
    CPU_INT32U  clk_div;


    clk_freq = BSP_CPU_ClkFreq();

    switch (pwr_clk_id) {
        case BSP_PERIPH_ID_DMA1:
        case BSP_PERIPH_ID_DMA2:
        case BSP_PERIPH_ID_SRAM:
        case BSP_PERIPH_ID_FLITF:
        case BSP_PERIPH_ID_CRC:
        case BSP_PERIPH_ID_FSMC:
        case BSP_PERIPH_ID_SDIO:
             return (clk_freq);

        case BSP_PERIPH_ID_AFIO:
        case BSP_PERIPH_ID_IOPA:
        case BSP_PERIPH_ID_IOPB:
        case BSP_PERIPH_ID_IOPC:
        case BSP_PERIPH_ID_IOPD:
        case BSP_PERIPH_ID_IOPE:
        case BSP_PERIPH_ID_IOPF:
        case BSP_PERIPH_ID_IOPG:
        case BSP_PERIPH_ID_ADC1:
        case BSP_PERIPH_ID_ADC2:
        case BSP_PERIPH_ID_TIM1:
        case BSP_PERIPH_ID_SPI1:
        case BSP_PERIPH_ID_TIM8:
        case BSP_PERIPH_ID_USART1:
        case BSP_PERIPH_ID_ADC3:
             clk_div = (BSP_PERIPH_REG_RCC_CFGR & 0x00003800) >> 11;
             if (clk_div < 4) {
                 return (clk_freq);
             }
             clk_div   = (clk_div - 3) << 1;
             clk_freq /= clk_div;
             return (clk_freq);

        case BSP_PERIPH_ID_TIM2:
        case BSP_PERIPH_ID_TIM3:
        case BSP_PERIPH_ID_TIM4:
        case BSP_PERIPH_ID_TIM5:
        case BSP_PERIPH_ID_TIM6:
        case BSP_PERIPH_ID_TIM7:
        case BSP_PERIPH_ID_WWDG:
        case BSP_PERIPH_ID_SPI2:
        case BSP_PERIPH_ID_SPI3:
        case BSP_PERIPH_ID_USART2:
        case BSP_PERIPH_ID_USART3:
        case BSP_PERIPH_ID_USART4:
        case BSP_PERIPH_ID_USART5:
        case BSP_PERIPH_ID_I2C1:
        case BSP_PERIPH_ID_I2C2:
        case BSP_PERIPH_ID_USB:
        case BSP_PERIPH_ID_CAN:
        case BSP_PERIPH_ID_BKP:
        case BSP_PERIPH_ID_PWR:
        case BSP_PERIPH_ID_DAC:
             clk_div = (BSP_PERIPH_REG_RCC_CFGR & 0x00000700) >> 8;
             if (clk_div < 4) {
                 return (clk_freq);
             }
             clk_div   = (clk_div - 3) << 1;
             clk_freq /= clk_div;
             return (clk_freq);
    }

    return ((CPU_INT32U)0);
}

/**
  * @brief  Enable clock for peripheral.
  * @param  pwr_clk_id      Power/clock ID.
  * @retval None
  */
void  BSP_PeriphEn (CPU_DATA  pwr_clk_id)
{
    switch (pwr_clk_id) {
        case BSP_PERIPH_ID_DMA1:
        case BSP_PERIPH_ID_DMA2:
        case BSP_PERIPH_ID_SRAM:
        case BSP_PERIPH_ID_FLITF:
        case BSP_PERIPH_ID_CRC:
        case BSP_PERIPH_ID_FSMC:
        case BSP_PERIPH_ID_SDIO:
             BSP_PERIPH_REG_RCC_AHBENR |= DEF_BIT(pwr_clk_id);
             break;

        case BSP_PERIPH_ID_AFIO:
        case BSP_PERIPH_ID_IOPA:
        case BSP_PERIPH_ID_IOPB:
        case BSP_PERIPH_ID_IOPC:
        case BSP_PERIPH_ID_IOPD:
        case BSP_PERIPH_ID_IOPE:
        case BSP_PERIPH_ID_IOPF:
        case BSP_PERIPH_ID_IOPG:
        case BSP_PERIPH_ID_ADC1:
        case BSP_PERIPH_ID_ADC2:
        case BSP_PERIPH_ID_TIM1:
        case BSP_PERIPH_ID_SPI1:
        case BSP_PERIPH_ID_TIM8:
        case BSP_PERIPH_ID_USART1:
        case BSP_PERIPH_ID_ADC3:
             BSP_PERIPH_REG_RCC_APB2ENR |= DEF_BIT(pwr_clk_id - 32);
             break;

        case BSP_PERIPH_ID_TIM2:
        case BSP_PERIPH_ID_TIM3:
        case BSP_PERIPH_ID_TIM4:
        case BSP_PERIPH_ID_TIM5:
        case BSP_PERIPH_ID_TIM6:
        case BSP_PERIPH_ID_TIM7:
        case BSP_PERIPH_ID_WWDG:
        case BSP_PERIPH_ID_SPI2:
        case BSP_PERIPH_ID_SPI3:
        case BSP_PERIPH_ID_USART2:
        case BSP_PERIPH_ID_USART3:
        case BSP_PERIPH_ID_USART4:
        case BSP_PERIPH_ID_USART5:
        case BSP_PERIPH_ID_I2C1:
        case BSP_PERIPH_ID_I2C2:
        case BSP_PERIPH_ID_USB:
        case BSP_PERIPH_ID_CAN:
        case BSP_PERIPH_ID_BKP:
        case BSP_PERIPH_ID_PWR:
        case BSP_PERIPH_ID_DAC:
             BSP_PERIPH_REG_RCC_APB1ENR |= DEF_BIT(pwr_clk_id - 64);
             break;
    }
}

/**
  * @brief  Disable clock for peripheral.
  * @param  pwr_clk_id      Power/clock ID.
  * @retval None
  */
void  BSP_PeriphDis (CPU_DATA  pwr_clk_id)
{
    switch (pwr_clk_id) {
        case BSP_PERIPH_ID_DMA1:
        case BSP_PERIPH_ID_DMA2:
        case BSP_PERIPH_ID_SRAM:
        case BSP_PERIPH_ID_FLITF:
        case BSP_PERIPH_ID_CRC:
        case BSP_PERIPH_ID_FSMC:
        case BSP_PERIPH_ID_SDIO:
             BSP_PERIPH_REG_RCC_AHBENR &= ~DEF_BIT(pwr_clk_id);
             break;

        case BSP_PERIPH_ID_AFIO:
        case BSP_PERIPH_ID_IOPA:
        case BSP_PERIPH_ID_IOPB:
        case BSP_PERIPH_ID_IOPC:
        case BSP_PERIPH_ID_IOPD:
        case BSP_PERIPH_ID_IOPE:
        case BSP_PERIPH_ID_IOPF:
        case BSP_PERIPH_ID_IOPG:
        case BSP_PERIPH_ID_ADC1:
        case BSP_PERIPH_ID_ADC2:
        case BSP_PERIPH_ID_TIM1:
        case BSP_PERIPH_ID_SPI1:
        case BSP_PERIPH_ID_TIM8:
        case BSP_PERIPH_ID_USART1:
        case BSP_PERIPH_ID_ADC3:
             BSP_PERIPH_REG_RCC_APB2ENR &= ~DEF_BIT(pwr_clk_id - 32);
             break;

        case BSP_PERIPH_ID_TIM2:
        case BSP_PERIPH_ID_TIM3:
        case BSP_PERIPH_ID_TIM4:
        case BSP_PERIPH_ID_TIM5:
        case BSP_PERIPH_ID_TIM6:
        case BSP_PERIPH_ID_TIM7:
        case BSP_PERIPH_ID_WWDG:
        case BSP_PERIPH_ID_SPI2:
        case BSP_PERIPH_ID_SPI3:
        case BSP_PERIPH_ID_USART2:
        case BSP_PERIPH_ID_USART3:
        case BSP_PERIPH_ID_USART4:
        case BSP_PERIPH_ID_USART5:
        case BSP_PERIPH_ID_I2C1:
        case BSP_PERIPH_ID_I2C2:
        case BSP_PERIPH_ID_USB:
        case BSP_PERIPH_ID_CAN:
        case BSP_PERIPH_ID_BKP:
        case BSP_PERIPH_ID_PWR:
        case BSP_PERIPH_ID_DAC:
             BSP_PERIPH_REG_RCC_APB1ENR &= ~DEF_BIT(pwr_clk_id - 64);
             break;
    }
}

/******************** COPYRIGHT 2012 Embest Tech. Co., Ltd.*****END OF FILE****/
