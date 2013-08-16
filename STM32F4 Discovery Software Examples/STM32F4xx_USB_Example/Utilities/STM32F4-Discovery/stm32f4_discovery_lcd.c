/**
  ******************************************************************************
  * @file    stm324xg_discovery_lcd.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    30-September-2011
  * @brief   This file includes the LCD driver for AM-240320L8TNQW00H (LCD_ILI9320)
  *          and AM240320D5TOQW01H (LCD_ILI9325) Liquid Crystal Display Modules
  *          of STM324xG-EVAL evaluation board(MB786) RevB.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; Portions COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************  
  */ 
/**
  ******************************************************************************
  * <h2><center>&copy; Portions COPYRIGHT 2012 Embest Tech. Co., Ltd.</center></h2>
  * @file    stm32f4_discovery_lcd.c
  * @author  CMP Team
  * @version V1.0.0
  * @date    28-December-2012
  * @brief   LCD LOW_LEVEL Drive 
  *          Modified to support the STM32F4DISCOVERY, STM32F4DIS-BB, STM32F4DIS-CAM
  *          and STM32F4DIS-LCD modules.     
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
#include "fonts.c"

/** @addtogroup Utilities
  * @{
  */ 

/** @addtogroup STM32F4_DISCOVERY
  * @{
  */
    
/** @defgroup stm32f4_discovery_LCD 
  * @brief This file includes the LCD driver for (LCDSSD2119)
  * @{
  */ 

/** @defgroup stm32f4_discovery_LCD_Private_TypesDef
  * @{
  */

/** @defgroup stm32f4_discovery_LCD_Private define
  * @{
  */
#define LCD_RST_PIN                  (GPIO_Pin_3)
#define LCD_RST_PORT                 (GPIOD)

#define LCD_PWM_PIN                  (GPIO_Pin_13)
#define LCD_PWM_PORT                 (GPIOD)

/* Note: LCD /CS is NE1 - Bank 1 of NOR/SRAM Bank 1~4 */
#define  LCD_BASE_Data               ((u32)(0x60000000|0x00100000))
#define  LCD_BASE_Addr               ((u32)(0x60000000|0x00000000))
#define  LCD_CMD                     (*(vu16 *)LCD_BASE_Addr)
#define  LCD_Data                    (*(vu16 *)LCD_BASE_Data)

#define MAX_POLY_CORNERS             200
#define POLY_Y(Z)                    ((int32_t)((Points + Z)->X))
#define POLY_X(Z)                    ((int32_t)((Points + Z)->Y))
/**
  * @}
  */ 

/** @defgroup stm32f4_discovery_LCD_Private_Macros
  * @{
  */
#define ABS(X)  ((X) > 0 ? (X) : -(X))     
/**
  * @}
  */ 
    
/** @defgroup stm32f4_discovery_LCD_Private_Variables
  * @{
  */ 
static sFONT *LCD_Currentfonts;

  /* Global variables to set the written text color */
static __IO uint16_t TextColor = 0x0000, BackColor = 0xFFFF;
  
/**
  * @}
  */ 

/** @defgroup stm32f4_discovery_LCD_Private_FunctionPrototypes
  * @{
  */ 
#ifndef USE_Delay
static void delay(__IO uint32_t nCount);
#endif /* USE_Delay*/
static void PutPixel(int16_t x, int16_t y);
static void LCD_PolyLineRelativeClosed(pPoint Points, uint16_t PointCount, uint16_t Closed);
/**
  * @}
  */ 

/**
  * @brief  LCD Default FSMC Init
  * @param  None
  * @retval None
  */
void LCD_DeInit(void)
{ 
  GPIO_InitTypeDef GPIO_InitStructure;

  /*!< LCD Display Off */
  LCD_DisplayOff();

  /* BANK 3 (of NOR/SRAM Bank 1~4) is disabled */
  FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM3, ENABLE);
  
  /*!< LCD_SPI DeInit */
  FSMC_NORSRAMDeInit(FSMC_Bank1_NORSRAM3);
   
/*-- GPIO Configuration ------------------------------------------------------*/
  /* SRAM Data lines configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_9 |
                                GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
 
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_MCO);


  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |
                                GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | 
                                GPIO_Pin_15;

  GPIO_Init(GPIOE, &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOE, GPIO_PinSource7 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource8 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource9 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource10 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource11 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource12 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource13 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource14 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource15 , GPIO_AF_MCO);

  /* SRAM Address lines configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | 
                                GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_12 | GPIO_Pin_13 | 
                                GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_Init(GPIOF, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOF,GPIO_PinSource0, GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOF,GPIO_PinSource1, GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOF,GPIO_PinSource2, GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOF,GPIO_PinSource3, GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOF,GPIO_PinSource4, GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOF,GPIO_PinSource5, GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOF,GPIO_PinSource12, GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOF,GPIO_PinSource13, GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOF,GPIO_PinSource14, GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOF,GPIO_PinSource15, GPIO_AF_MCO);


  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | 
                                GPIO_Pin_4 | GPIO_Pin_5;

  GPIO_Init(GPIOG, &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOG,GPIO_PinSource0, GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOG,GPIO_PinSource1, GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOG,GPIO_PinSource2, GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOG,GPIO_PinSource3, GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOG,GPIO_PinSource4, GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOG,GPIO_PinSource5, GPIO_AF_MCO);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13; 

  GPIO_Init(GPIOD, &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOD,GPIO_PinSource11, GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource12, GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource13, GPIO_AF_MCO);

  /* NOE and NWE configuration */  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 |GPIO_Pin_5;

  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource4, GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource5, GPIO_AF_MCO);

  /* NE3 configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 

  GPIO_Init(GPIOG, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource12, GPIO_AF_MCO);

  /* NBL0, NBL1 configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1; 
  GPIO_Init(GPIOE, &GPIO_InitStructure); 

  GPIO_PinAFConfig(GPIOE,GPIO_PinSource0, GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource1, GPIO_AF_MCO);
}

/**
  * @brief  Configures LCD Control lines (FSMC Pins) in alternate function mode.
  * @param  None
  * @retval None
  */
void LCD_CtrlLinesConfig(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable GPIOB, GPIOD, GPIOE, GPIOF, GPIOG and AFIO clocks */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE |
                         RCC_AHB1Periph_GPIOF, ENABLE);

/*-- GPIO Configuration ------------------------------------------------------*/
  /* SRAM Data lines,  NOE and NWE configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_9 |
                                GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15 |
                                GPIO_Pin_4 |GPIO_Pin_5;;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource4, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_FSMC);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |
                                GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | 
                                GPIO_Pin_15;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOE, GPIO_PinSource7 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource8 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource9 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource10 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource11 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource12 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource13 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource14 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource15 , GPIO_AF_FSMC);

  /* SRAM Address lines configuration LCD-DC */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_Init(GPIOE, &GPIO_InitStructure);  
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource3, GPIO_AF_FSMC);	   

  /* NE3 configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; 
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_FSMC);

  /* LCD RST configuration */
  GPIO_InitStructure.GPIO_Pin = LCD_RST_PIN; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

  GPIO_Init(LCD_RST_PORT, &GPIO_InitStructure);

   /* LCD pwm configuration */
  GPIO_InitStructure.GPIO_Pin = LCD_PWM_PIN; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

  GPIO_Init(LCD_PWM_PORT, &GPIO_InitStructure);
  GPIO_SetBits(LCD_PWM_PORT, LCD_PWM_PIN);
}

/**
  * @brief  Configures the Parallel interface (FSMC) for LCD(Parallel mode)
  * @param  None
  * @retval None
  */
void LCD_FSMCConfig(void)
{
  FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
  FSMC_NORSRAMTimingInitTypeDef  p;
   
  /* Enable FSMC clock */
  RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE);
  
/*-- FSMC Configuration ------------------------------------------------------*/
/*----------------------- SRAM Bank 1 ----------------------------------------*/
  /* FSMC_Bank1_NORSRAM4 configuration */
  p.FSMC_AddressSetupTime = 1;
  p.FSMC_AddressHoldTime = 0;
  p.FSMC_DataSetupTime = 9;
  p.FSMC_BusTurnAroundDuration = 0;
  p.FSMC_CLKDivision = 0;
  p.FSMC_DataLatency = 0;
  p.FSMC_AccessMode = FSMC_AccessMode_A;
  /* Color LCD configuration ------------------------------------
     LCD configured as follow:
        - Data/Address MUX = Disable
        - Memory Type = SRAM
        - Data Width = 16bit
        - Write Operation = Enable
        - Extended Mode = Enable
        - Asynchronous Wait = Disable */

  FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
  FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
  FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;
  FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
  FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
  FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
  FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
  FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;

  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);   

  /* Enable FSMC NOR/SRAM Bank1 */
  FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);
}

/**
  * @brief  LCD Init.
  * @retval None
  */
void STM32f4_Discovery_LCD_Init(void)
{ 

  unsigned long ulCount;
	
  /* Configure the LCD Control pins */
  LCD_CtrlLinesConfig();
	
  /* Configure the FSMC Parallel interface */
  LCD_FSMCConfig();
	
  _delay_(5); 
	
  /* Reset LCD */
  GPIO_ResetBits(LCD_RST_PORT, LCD_RST_PIN);	
  _delay_(10);	
  GPIO_SetBits(LCD_RST_PORT, LCD_RST_PIN);

  /*
  SSD2119Init(void)
  */
  /* Enter sleep mode (if we are not already there).*/
  LCD_WriteReg(SSD2119_SLEEP_MODE_1_REG, 0x0001);

  /* Set initial power parameters. */
  LCD_WriteReg(SSD2119_PWR_CTRL_5_REG, 0x00B2);
  LCD_WriteReg(SSD2119_VCOM_OTP_1_REG, 0x0006);
  
  /* Start the oscillator.*/
  LCD_WriteReg(SSD2119_OSC_START_REG, 0x0001);

  /* Set pixel format and basic display orientation (scanning direction).*/
  LCD_WriteReg(SSD2119_OUTPUT_CTRL_REG, 0x30EF);
  LCD_WriteReg(SSD2119_LCD_DRIVE_AC_CTRL_REG, 0x0600);

  /* Exit sleep mode.*/
  LCD_WriteReg(SSD2119_SLEEP_MODE_1_REG, 0x0000);
  _delay_(5);
	  
  /* Configure pixel color format and MCU interface parameters.*/
  LCD_WriteReg(SSD2119_ENTRY_MODE_REG, ENTRY_MODE_DEFAULT);

  /* Set analog parameters */
  LCD_WriteReg(SSD2119_SLEEP_MODE_2_REG, 0x0999);
  LCD_WriteReg(SSD2119_ANALOG_SET_REG, 0x3800);

  /* Enable the display */
  LCD_WriteReg(SSD2119_DISPLAY_CTRL_REG, 0x0033);

  /* Set VCIX2 voltage to 6.1V.*/
  LCD_WriteReg(SSD2119_PWR_CTRL_2_REG, 0x0005);

  /* Configure gamma correction.*/
  LCD_WriteReg(SSD2119_GAMMA_CTRL_1_REG, 0x0000);
  LCD_WriteReg(SSD2119_GAMMA_CTRL_2_REG, 0x0303);
  LCD_WriteReg(SSD2119_GAMMA_CTRL_3_REG, 0x0407);
  LCD_WriteReg(SSD2119_GAMMA_CTRL_4_REG, 0x0301);
  LCD_WriteReg(SSD2119_GAMMA_CTRL_5_REG, 0x0301);
  LCD_WriteReg(SSD2119_GAMMA_CTRL_6_REG, 0x0403);
  LCD_WriteReg(SSD2119_GAMMA_CTRL_7_REG, 0x0707);
  LCD_WriteReg(SSD2119_GAMMA_CTRL_8_REG, 0x0400);
  LCD_WriteReg(SSD2119_GAMMA_CTRL_9_REG, 0x0a00);
  LCD_WriteReg(SSD2119_GAMMA_CTRL_10_REG, 0x1000);

  /* Configure Vlcd63 and VCOMl */
  LCD_WriteReg(SSD2119_PWR_CTRL_3_REG, 0x000A);
  LCD_WriteReg(SSD2119_PWR_CTRL_4_REG, 0x2E00);

  /* Set the display size and ensure that the GRAM window is set to allow
     access to the full display buffer.*/
  LCD_WriteReg(SSD2119_V_RAM_POS_REG, (LCD_PIXEL_HEIGHT-1) << 8);
  LCD_WriteReg(SSD2119_H_RAM_START_REG, 0x0000);
  LCD_WriteReg(SSD2119_H_RAM_END_REG, LCD_PIXEL_WIDTH-1);

  LCD_WriteReg(SSD2119_X_RAM_ADDR_REG, 0x00);
  LCD_WriteReg(SSD2119_Y_RAM_ADDR_REG, 0x00);
  
  /* clear the lcd  */
  LCD_WriteReg(SSD2119_RAM_DATA_REG, 0x0000);
  for(ulCount = 0; ulCount < (LCD_PIXEL_WIDTH * LCD_PIXEL_HEIGHT); ulCount++)
  {
    LCD_WriteRAM(0x0000);
  }
  LCD_SetFont(&LCD_DEFAULT_FONT);
}


/**
  * @brief  Sets the cursor position.
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position. 
  * @retval None
  */
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos)
{
  /* Set the X address of the display cursor.*/
  LCD_WriteReg(SSD2119_X_RAM_ADDR_REG, Xpos);
	
  /* Set the Y address of the display cursor.*/
  LCD_WriteReg(SSD2119_Y_RAM_ADDR_REG, Ypos);
}

/**
  * @brief  Writes to the selected LCD register.
  * @param  LCD_Reg: address of the selected register.
  * @param  LCD_RegValue: value to write to the selected register.
  * @retval None
  */
void LCD_WriteReg(uint8_t LCD_Reg, uint16_t LCD_RegValue)
{
  /* Write 16-bit Index, then Write Reg */
  LCD_CMD = LCD_Reg;
  /* Write 16-bit Reg */
  LCD_Data = LCD_RegValue;
}

/**
  * @brief  Reads the selected LCD Register.
  * @param  LCD_Reg: address of the selected register.
  * @retval LCD Register Value.
  */
uint16_t LCD_ReadReg(uint8_t LCD_Reg)
{
  /* Write 16-bit Index (then Read Reg) */
  LCD_CMD = LCD_Reg;
  /* Read 16-bit Reg */
  return (LCD_Data);
}

/**
  * @brief  Prepare to write to the LCD RAM.
  * @param  None
  * @retval None
  */
void LCD_WriteRAM_Prepare(void)
{
	LCD_CMD = SSD2119_RAM_DATA_REG;
}

/**
  * @brief  Writes to the LCD RAM.
  * @param  RGB_Code: the pixel color in RGB mode (5-6-5).
  * @retval None
  */
void LCD_WriteRAM(uint16_t RGB_Code)
{
  /* Write 16-bit GRAM Reg */
  LCD_Data = RGB_Code;
}

/**
  * @brief  Reads the LCD RAM.
  * @param  None
  * @retval LCD RAM Value.
  */
uint16_t LCD_ReadRAM(void)
{
  /* Write 16-bit Index (then Read Reg) */
//  LCD_CMD = SSD2119_RAM_DATA_REG; /* Select GRAM Reg */
  /* Read 16-bit Reg */
  return LCD_Data;
}

/**
  * @brief  Test LCD Display
  * @retval None
  */
void LCD_RGB_Test(void)
{
  uint32_t index;

  LCD_SetCursor(0x00, 0x00); 
  LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */

	/* R */
  for(index = 0; index < (LCD_PIXEL_HEIGHT*LCD_PIXEL_WIDTH)/3; index++)
  {
    LCD_Data = LCD_COLOR_RED;
  }
	  
  /* G */
  for(;index < 2*(LCD_PIXEL_HEIGHT*LCD_PIXEL_WIDTH)/3; index++)
  {
    LCD_Data = LCD_COLOR_GREEN;
  }
	  
	/* B */
  for(; index < LCD_PIXEL_HEIGHT*LCD_PIXEL_WIDTH; index++)
  {
    LCD_Data = LCD_COLOR_BLUE;
  }
}



/**
  * @brief  Sets the LCD Text and Background colors.
  * @param  _TextColor: specifies the Text Color.
  * @param  _BackColor: specifies the Background Color.
  * @retval None
  */
void LCD_SetColors(__IO uint16_t _TextColor, __IO uint16_t _BackColor)
{
  TextColor = _TextColor; 
  BackColor = _BackColor;
}

/**
  * @brief  Gets the LCD Text and Background colors.
  * @param  _TextColor: pointer to the variable that will contain the Text 
            Color.
  * @param  _BackColor: pointer to the variable that will contain the Background 
            Color.
  * @retval None
  */
void LCD_GetColors(__IO uint16_t *_TextColor, __IO uint16_t *_BackColor)
{
  *_TextColor = TextColor; *_BackColor = BackColor;
}

/**
  * @brief  Sets the Text color.
  * @param  Color: specifies the Text color code RGB(5-6-5).
  * @retval None
  */
void LCD_SetTextColor(__IO uint16_t Color)
{
  TextColor = Color;
}


/**
  * @brief  Sets the Background color.
  * @param  Color: specifies the Background color code RGB(5-6-5).
  * @retval None
  */
void LCD_SetBackColor(__IO uint16_t Color)
{
  BackColor = Color;
}

/**
LCD_DisplayOff
  */
void LCD_DisplayOff(void)
{

}

/**
LCD_DisplayOn
  */
void LCD_DisplayOn(void)
{

}

/**
  * @brief  Sets the Text Font.
  * @param  fonts: specifies the font to be used.
  * @retval None
  */
void LCD_SetFont(sFONT *fonts)
{
  LCD_Currentfonts = fonts;
}

/**
  * @brief  Gets the Text Font.
  * @param  None.
  * @retval the used font.
  */
sFONT *LCD_GetFont(void)
{
  return LCD_Currentfonts;
}

/**
  * @brief  Clears the selected line.
  * @param  Line: the Line to be cleared.
  *   This parameter can be one of the following values:
  *     @arg Linex: where x can be 0..n
  * @retval None
  */
void LCD_ClearLine(uint16_t Line)
{
  uint16_t refcolumn = 0;

  do {
       /* Display one character on LCD */
    LCD_DisplayChar(Line, refcolumn, ' ');
    /* Decrement the column position by 16 */
    refcolumn += LCD_Currentfonts->Width;
  } while (refcolumn < LCD_PIXEL_WIDTH);	
}

/**
  * @brief  Clears the hole LCD.
  * @param  Color: the color of the background.
  * @retval None
  */
void LCD_Clear(uint16_t Color)
{
  uint32_t index = 0;
  
  LCD_SetCursor(0x00, 0x00); 
  LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
  for(index = 0; index < LCD_PIXEL_HEIGHT*LCD_PIXEL_WIDTH; index++)
  {
    LCD_Data = Color;
  }  
}

/**
  * @brief  Displays a pixel.
  * @param  x: pixel x.
  * @param  y: pixel y.  
  * @retval None
  */
static void PutPixel(int16_t x, int16_t y)
{ 
  if(x < 0 || x > LCD_PIXEL_WIDTH-1 || y < 0 || y > LCD_PIXEL_HEIGHT-1)
  {
    return;  
  }
  LCD_DrawLine(x, y, 1, LCD_DIR_HORIZONTAL);
}

/**
  * @brief  Draws a character on LCD.
  * @param  Xpos: the Line where to display the character shape.
  * @param  Ypos: start column address.
  * @param  c: pointer to the character data.
  * @retval None
  */
void LCD_DrawChar(uint16_t Xpos, uint16_t Ypos, const uint16_t *c)
{
  uint32_t index = 0, i = 0;
  uint16_t  Xaddress = 0;
  Xaddress = Xpos;
  
  LCD_SetCursor(Ypos, Xaddress);
  
  for(index = 0; index < LCD_Currentfonts->Height; index++)
  {
    LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
    for(i = 0; i < LCD_Currentfonts->Width; i++)
    {
  
      if((((c[index] & ((0x80 << ((LCD_Currentfonts->Width / 12 ) * 8 ) ) >> i)) == 0x00) &&(LCD_Currentfonts->Width <= 12))||
        (((c[index] & (0x1 << i)) == 0x00)&&(LCD_Currentfonts->Width > 12 )))

      {
        LCD_WriteRAM(BackColor);
      }
      else
      {
        LCD_WriteRAM(TextColor);
      } 
    }
    Xaddress++;
    LCD_SetCursor(Ypos, Xaddress);
  }
}

/**
  * @brief  Displays one character (16dots width, 24dots height).
  * @param  Line: the Line where to display the character shape .
  *   This parameter can be one of the following values:
  *     @arg Linex: where x can be 0..9
  * @param  Column: start column address.
  * @param  Ascii: character ascii code, must be between 0x20 and 0x7E.
  * @retval None
  */
void LCD_DisplayChar(uint16_t Line, uint16_t Column, uint8_t Ascii)
{
  Ascii -= 32;
  LCD_DrawChar(Line, Column, &LCD_Currentfonts->table[Ascii * LCD_Currentfonts->Height]);
}

/**
  * @brief  Displays a maximum of 20 char on the LCD.
  * @param  Line: the Line where to display the character shape .
  *   This parameter can be one of the following values:
  *     @arg Linex: where x can be 0..9
  * @param  *ptr: pointer to string to display on LCD.
  * @retval None
  */
void LCD_DisplayStringLine(uint16_t Line, uint8_t *ptr)
{
  uint16_t refcolumn = 0;

  /* Send the string character by character on lCD */
  while (*ptr != 0)
  {
    /* Display one character on LCD */
    LCD_DisplayChar(Line, refcolumn, *ptr);
    /* Decrement the column position by 16 */
    refcolumn += LCD_Currentfonts->Width;
	if (refcolumn >= LCD_PIXEL_WIDTH) {
		break;
	}
    /* Point on the next character */
    ptr++;
  }
}

/**
  * @brief  Sets a display window
  * @param  Xpos: specifies the X bottom left position.
  * @param  Ypos: specifies the Y bottom left position.
  * @param  Height: display window width.
  * @param  Width: display window Height.
  * @retval None
  */
void LCD_SetDisplayWindow(uint16_t Xpos, uint16_t Ypos, uint16_t width, uint16_t Height)
{
  uint32_t value = 0;	

  LCD_WriteReg(SSD2119_H_RAM_START_REG, Xpos);
	
  if ((Xpos+width) >= LCD_PIXEL_WIDTH) {
    LCD_WriteReg(SSD2119_H_RAM_END_REG, LCD_PIXEL_WIDTH-1);	
  } else {
    LCD_WriteReg(SSD2119_H_RAM_END_REG, Xpos+width);		
  }
  
  if ((Ypos+Height) >= LCD_PIXEL_HEIGHT) {
    value = (LCD_PIXEL_HEIGHT-1) << 8;	
  } else {
    value = (Ypos+Height) << 8;	
  }
  value |= Xpos;
  LCD_WriteReg(SSD2119_V_RAM_POS_REG, value);
  LCD_SetCursor(Xpos, Ypos);
}

/**
  * @brief  Disables LCD Window mode.
  * @param  None
  * @retval None
  */
void LCD_WindowModeDisable(void)
{
#if 0
  LCD_SetDisplayWindow(239, 0x13F, 240, 320);
  LCD_WriteReg(LCD_REG_3, 0x1018);    
#endif
}

/**
  * @brief  Displays a line.
  * @param Xpos: specifies the X position.
  * @param Ypos: specifies the Y position.
  * @param Length: line length.
  * @param Direction: line direction.
  *   This parameter can be one of the following values: Vertical or Horizontal.
  * @retval None
  */
void LCD_DrawLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length, uint8_t Direction)
{
  uint32_t i = 0;
  
  LCD_SetCursor(Xpos, Ypos);
  if(Direction == LCD_DIR_HORIZONTAL)
  {
    LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
    for(i = 0; i < Length; i++)
    {
      LCD_WriteRAM(TextColor);
    }
  }
  else
  {
    for(i = 0; i < Length; i++)
    {
      LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
      LCD_WriteRAM(TextColor);
      Ypos++;
      LCD_SetCursor(Xpos, Ypos);
    }
  }
}

/**
  * @brief  Displays a rectangle.
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position.
  * @param  Height: display rectangle height.
  * @param  Width: display rectangle width.
  * @retval None
  */
void LCD_DrawRect(uint16_t Xpos, uint16_t Ypos, uint8_t Height, uint16_t Width)
{
  LCD_DrawLine(Xpos, Ypos, Width, LCD_DIR_VERTICAL);
  LCD_DrawLine((Xpos + Height-1), Ypos, Width, LCD_DIR_VERTICAL);
  
  LCD_DrawLine(Xpos, Ypos, Height, LCD_DIR_HORIZONTAL);
  LCD_DrawLine(Xpos, (Ypos + Width), Height, LCD_DIR_HORIZONTAL);
}

/**
  * @brief  Displays a circle.
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position.
  * @param  Radius
  * @retval None
  */
void LCD_DrawCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius)
{
  int32_t  D;/* Decision Variable */ 
  uint32_t  CurX;/* Current X Value */
  uint32_t  CurY;/* Current Y Value */ 
  
  D = 3 - (Radius << 1);
  CurX = 0;
  CurY = Radius;
  
  while (CurX <= CurY)
  {
    LCD_SetCursor(Xpos + CurX, Ypos + CurY);
    LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
    LCD_WriteRAM(TextColor);
    LCD_SetCursor(Xpos + CurX, Ypos - CurY);
    LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
    LCD_WriteRAM(TextColor);
    LCD_SetCursor(Xpos - CurX, Ypos + CurY);
    LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
    LCD_WriteRAM(TextColor);
    LCD_SetCursor(Xpos - CurX, Ypos - CurY);
    LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
    LCD_WriteRAM(TextColor);
    LCD_SetCursor(Xpos + CurY, Ypos + CurX);
    LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
    LCD_WriteRAM(TextColor);
    LCD_SetCursor(Xpos + CurY, Ypos - CurX);
    LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
    LCD_WriteRAM(TextColor);
    LCD_SetCursor(Xpos - CurY, Ypos + CurX);
    LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
    LCD_WriteRAM(TextColor);
    LCD_SetCursor(Xpos - CurY, Ypos - CurX);
    LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
    LCD_WriteRAM(TextColor);
    if (D < 0)
    { 
      D += (CurX << 2) + 6;
    }
    else
    {
      D += ((CurX - CurY) << 2) + 10;
      CurY--;
    }
    CurX++;
  }
}

/**
  * @brief  Displays a mono-color picture.
  * @param  Pict: pointer to the picture array.
  * @retval None
  */
void LCD_DrawMonoPict(const uint32_t *Pict)
{
  uint32_t index = 0, i = 0;
  LCD_SetCursor(0, (LCD_PIXEL_WIDTH - 1)); 
  LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
  for(index = 0; index < 2400; index++)
  {
    for(i = 0; i < 32; i++)
    {
      if((Pict[index] & (1 << i)) == 0x00)
      {
        LCD_WriteRAM(BackColor);
      }
      else
      {
        LCD_WriteRAM(TextColor);
      }
    }
  }
}

/**
  * @brief  Displays a bitmap picture loaded in the internal Flash.
  * @param  BmpAddress: Bmp picture address in the internal Flash.
  * @retval None
  */
void LCD_WriteBMP(uint32_t BmpAddress)
{
#if 0
  uint32_t index = 0, size = 0;
  /* Read bitmap size */
  size = *(__IO uint16_t *) (BmpAddress + 2);
  size |= (*(__IO uint16_t *) (BmpAddress + 4)) << 16;
  /* Get bitmap data address offset */
  index = *(__IO uint16_t *) (BmpAddress + 10);
  index |= (*(__IO uint16_t *) (BmpAddress + 12)) << 16;
  size = (size - index)/2;
  BmpAddress += index;
  /* Set GRAM write direction and BGR = 1 */
  /* I/D=00 (Horizontal : decrement, Vertical : decrement) */
  /* AM=1 (address is updated in vertical writing direction) */
  LCD_WriteReg(LCD_REG_3, 0x1008);
 
  LCD_WriteRAM_Prepare();
 
  for(index = 0; index < size; index++)
  {
    LCD_WriteRAM(*(__IO uint16_t *)BmpAddress);
    BmpAddress += 2;
  }
 
  /* Set GRAM write direction and BGR = 1 */
  /* I/D = 01 (Horizontal : increment, Vertical : decrement) */
  /* AM = 1 (address is updated in vertical writing direction) */
  LCD_WriteReg(LCD_REG_3, 0x1018);
#endif
}

/**
  * @brief  Displays a full rectangle.
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position.
  * @param  Height: rectangle height.
  * @param  Width: rectangle width.
  * @retval None
  */
void LCD_DrawFullRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
  LCD_SetTextColor(TextColor);

  LCD_DrawLine(Xpos, Ypos, Width, LCD_DIR_HORIZONTAL);
  LCD_DrawLine(Xpos, (Ypos+Height), Width, LCD_DIR_HORIZONTAL);
  
  LCD_DrawLine(Xpos, Ypos, Height, LCD_DIR_VERTICAL);
  LCD_DrawLine((Xpos+Width-1), Ypos, Height, LCD_DIR_VERTICAL);

  Height--;
  Ypos++;

  LCD_SetTextColor(BackColor);

  while(Height--)
  {
    LCD_DrawLine(Xpos, Ypos++, Width, LCD_DIR_HORIZONTAL);    
  }

  LCD_SetTextColor(TextColor);
}

/**
  * @brief  Displays a full circle.
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position.
  * @param  Radius
  * @retval None
  */
void LCD_DrawFullCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius)
{
  int32_t  D;    /* Decision Variable */ 
  uint32_t  CurX;/* Current X Value */
  uint32_t  CurY;/* Current Y Value */ 
  
  D = 3 - (Radius << 1);

  CurX = 0;
  CurY = Radius;
  
  LCD_SetTextColor(BackColor);

  while (CurX <= CurY)
  {
    if(CurY > 0) 
    {
      LCD_DrawLine(Xpos - CurX, Ypos + CurY, 2*CurY, LCD_DIR_HORIZONTAL);
      LCD_DrawLine(Xpos + CurX, Ypos + CurY, 2*CurY, LCD_DIR_HORIZONTAL);
    }

    if(CurX > 0) 
    {
      LCD_DrawLine(Xpos - CurY, Ypos + CurX, 2*CurX, LCD_DIR_HORIZONTAL);
      LCD_DrawLine(Xpos + CurY, Ypos + CurX, 2*CurX, LCD_DIR_HORIZONTAL);
    }
    if (D < 0)
    { 
      D += (CurX << 2) + 6;
    }
    else
    {
      D += ((CurX - CurY) << 2) + 10;
      CurY--;
    }
    CurX++;
  }

  LCD_SetTextColor(TextColor);
  LCD_DrawCircle(Xpos, Ypos, Radius);
}

/**
  * @brief  Displays an uni-line (between two points).
  * @param  x1: specifies the point 1 x position.
  * @param  y1: specifies the point 1 y position.
  * @param  x2: specifies the point 2 x position.
  * @param  y2: specifies the point 2 y position.
  * @retval None
  */
void LCD_DrawUniLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
  int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0, 
  yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0, 
  curpixel = 0;
  
  deltax = ABS(x2 - x1);        /* The difference between the x's */
  deltay = ABS(y2 - y1);        /* The difference between the y's */
  x = x1;                       /* Start x off at the first pixel */
  y = y1;                       /* Start y off at the first pixel */
  
  if (x2 >= x1)                 /* The x-values are increasing */
  {
    xinc1 = 1;
    xinc2 = 1;
  }
  else                          /* The x-values are decreasing */
  {
    xinc1 = -1;
    xinc2 = -1;
  }
  
  if (y2 >= y1)                 /* The y-values are increasing */
  {
    yinc1 = 1;
    yinc2 = 1;
  }
  else                          /* The y-values are decreasing */
  {
    yinc1 = -1;
    yinc2 = -1;
  }
  
  if (deltax >= deltay)         /* There is at least one x-value for every y-value */
  {
    xinc1 = 0;                  /* Don't change the x when numerator >= denominator */
    yinc2 = 0;                  /* Don't change the y for every iteration */
    den = deltax;
    num = deltax / 2;
    numadd = deltay;
    numpixels = deltax;         /* There are more x-values than y-values */
  }
  else                          /* There is at least one y-value for every x-value */
  {
    xinc2 = 0;                  /* Don't change the x for every iteration */
    yinc1 = 0;                  /* Don't change the y when numerator >= denominator */
    den = deltay;
    num = deltay / 2;
    numadd = deltax;
    numpixels = deltay;         /* There are more y-values than x-values */
  }
  
  for (curpixel = 0; curpixel <= numpixels; curpixel++)
  {
    PutPixel(x, y);             /* Draw the current pixel */
    num += numadd;              /* Increase the numerator by the top of the fraction */
    if (num >= den)             /* Check if numerator >= denominator */
    {
      num -= den;               /* Calculate the new numerator value */
      x += xinc1;               /* Change the x as appropriate */
      y += yinc1;               /* Change the y as appropriate */
    }
    x += xinc2;                 /* Change the x as appropriate */
    y += yinc2;                 /* Change the y as appropriate */
  }
}

/**
  * @brief  Displays an poly-line (between many points).
  * @param  Points: pointer to the points array.
  * @param  PointCount: Number of points.
  * @retval None
  */
void LCD_PolyLine(pPoint Points, uint16_t PointCount)
{
  int16_t X = 0, Y = 0;

  if(PointCount < 2)
  {
    return;
  }

  while(--PointCount)
  {
    X = Points->X;
    Y = Points->Y;
    Points++;
    LCD_DrawUniLine(X, Y, Points->X, Points->Y);
  }
}

/**
  * @brief  Displays an relative poly-line (between many points).
  * @param  Points: pointer to the points array.
  * @param  PointCount: Number of points.
  * @param  Closed: specifies if the draw is closed or not.
  *           1: closed, 0 : not closed.
  * @retval None
  */
static void LCD_PolyLineRelativeClosed(pPoint Points, uint16_t PointCount, uint16_t Closed)
{
  int16_t X = 0, Y = 0;
  pPoint First = Points;

  if(PointCount < 2)
  {
    return;
  }  
  X = Points->X;
  Y = Points->Y;
  while(--PointCount)
  {
    Points++;
    LCD_DrawUniLine(X, Y, X + Points->X, Y + Points->Y);
    X = X + Points->X;
    Y = Y + Points->Y;
  }
  if(Closed)
  {
    LCD_DrawUniLine(First->X, First->Y, X, Y);
  }  
}

/**
  * @brief  Displays a closed poly-line (between many points).
  * @param  Points: pointer to the points array.
  * @param  PointCount: Number of points.
  * @retval None
  */
void LCD_ClosedPolyLine(pPoint Points, uint16_t PointCount)
{
  LCD_PolyLine(Points, PointCount);
  LCD_DrawUniLine(Points->X, Points->Y, (Points+PointCount-1)->X, (Points+PointCount-1)->Y);
}

/**
  * @brief  Displays a relative poly-line (between many points).
  * @param  Points: pointer to the points array.
  * @param  PointCount: Number of points.
  * @retval None
  */
void LCD_PolyLineRelative(pPoint Points, uint16_t PointCount)
{
  LCD_PolyLineRelativeClosed(Points, PointCount, 0);
}

/**
  * @brief  Displays a closed relative poly-line (between many points).
  * @param  Points: pointer to the points array.
  * @param  PointCount: Number of points.
  * @retval None
  */
void LCD_ClosedPolyLineRelative(pPoint Points, uint16_t PointCount)
{
  LCD_PolyLineRelativeClosed(Points, PointCount, 1);
}


/**
  * @brief  Displays a  full poly-line (between many points).
  * @param  Points: pointer to the points array.
  * @param  PointCount: Number of points.
  * @retval None
  */
void LCD_FillPolyLine(pPoint Points, uint16_t PointCount)
{
  /*  public-domain code by Darel Rex Finley, 2007 */
  uint16_t  nodes = 0, nodeX[MAX_POLY_CORNERS], pixelX = 0, pixelY = 0, i = 0,
  j = 0, swap = 0;
  uint16_t  IMAGE_LEFT = 0, IMAGE_RIGHT = 0, IMAGE_TOP = 0, IMAGE_BOTTOM = 0;

  IMAGE_LEFT = IMAGE_RIGHT = Points->X;
  IMAGE_TOP= IMAGE_BOTTOM = Points->Y;

  for(i = 1; i < PointCount; i++)
  {
    pixelX = POLY_X(i);
    if(pixelX < IMAGE_LEFT)
    {
      IMAGE_LEFT = pixelX;
    }
    if(pixelX > IMAGE_RIGHT)
    {
      IMAGE_RIGHT = pixelX;
    }
    
    pixelY = POLY_Y(i);
    if(pixelY < IMAGE_TOP)
    { 
      IMAGE_TOP = pixelY;
    }
    if(pixelY > IMAGE_BOTTOM)
    {
      IMAGE_BOTTOM = pixelY;
    }
  }
  
  LCD_SetTextColor(BackColor);  

  /*  Loop through the rows of the image. */
  for (pixelY = IMAGE_TOP; pixelY < IMAGE_BOTTOM; pixelY++) 
  {  
    /* Build a list of nodes. */
    nodes = 0; j = PointCount-1;

    for (i = 0; i < PointCount; i++) 
    {
      if (((POLY_Y(i)<(double) pixelY) && (POLY_Y(j)>=(double) pixelY)) || \
          ((POLY_Y(j)<(double) pixelY) && (POLY_Y(i)>=(double) pixelY)))
      {
        nodeX[nodes++]=(int) (POLY_X(i)+((pixelY-POLY_Y(i))*(POLY_X(j)-POLY_X(i)))/(POLY_Y(j)-POLY_Y(i))); 
      }
      j = i; 
    }
  
    /* Sort the nodes, via a simple "Bubble" sort. */
    i = 0;
    while (i < nodes-1) 
    {
      if (nodeX[i]>nodeX[i+1]) 
      {
        swap = nodeX[i]; 
        nodeX[i] = nodeX[i+1]; 
        nodeX[i+1] = swap; 
        if(i)
        {
          i--; 
        }
      }
      else 
      {
        i++;
      }
    }
  
    /*  Fill the pixels between node pairs. */
    for (i = 0; i < nodes; i+=2) 
    {
      if(nodeX[i] >= IMAGE_RIGHT) 
      {
        break;
      }
      if(nodeX[i+1] > IMAGE_LEFT) 
      {
        if (nodeX[i] < IMAGE_LEFT)
        {
          nodeX[i]=IMAGE_LEFT;
        }
        if(nodeX[i+1] > IMAGE_RIGHT)
        {
          nodeX[i+1] = IMAGE_RIGHT;
        }
        LCD_SetTextColor(BackColor);
        LCD_DrawLine(pixelY, nodeX[i+1], nodeX[i+1] - nodeX[i], LCD_DIR_HORIZONTAL);
        LCD_SetTextColor(TextColor);
        PutPixel(pixelY, nodeX[i+1]);
        PutPixel(pixelY, nodeX[i]);
        /* for (j=nodeX[i]; j<nodeX[i+1]; j++) PutPixel(j,pixelY); */
      }
    }
  } 
  /* draw the edges */
  LCD_SetTextColor(TextColor);
}


#ifndef USE_Delay
/**
  * @brief  Inserts a delay time.
  * @param  nCount: specifies the delay time length.
  * @retval None
  */
static void delay(__IO uint32_t nCount)
{
  __IO uint32_t index = 0; 
  for(index = (10000 * nCount); index != 0; index--)
  {
  }
}
#endif /* USE_Delay*/
/**
  * @}
  */ 

/**
  * @}
  */ 
  
/**
  * @}
  */ 

/**
  * @}
  */ 
  
/**
  * @}
  */  

/*********** Portions COPYRIGHT 2012 Embest Tech. Co., Ltd.*****END OF FILE****/
