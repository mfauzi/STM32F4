/**
  ******************************************************************************
  * <h2><center>&copy; COPYRIGHT 2012 Embest Tech. Co., Ltd.</center></h2>
  * @file    LCD_Touch_Calibration.h
  * @author  CMP Team
  * @version V1.0.0
  * @date    28-December-2012
  * @brief   header for LCD_Touch_Calibration.c      
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

#ifndef _LCD_TOUCH_CALIBRATION_H_
#define _LCD_TOUCH_CALIBRATION_H_

typedef struct{
    uint16_t x;
    uint16_t y;
}Point_Struct;

typedef struct{
    /*scale factor for x direction*/
    float    xScale;
    /*scale factor for x direction*/
    float    yScale;
    /*Offset in x direction*/
    int16_t  xOffset;
    /*Offset in y direction*/
    int16_t  yOffset;
}Adjust_Struct;

#ifdef LCD_TOUCH_CALIBRATION_MODULE
    #define LCD_TOUCH_CALIBRATION_EXT
#else
    #define LCD_TOUCH_CALIBRATION_EXT extern
#endif

/* Private define ------------------------------------------------------------*/

    
/* External variables prototypes ---------------------------------------------*/
LCD_TOUCH_CALIBRATION_EXT Adjust_Struct adjust_Para;

/* Private functions ---------------------------------------------------------*/
void    Lcd_Touch_Calibration             (void);
void 	Calibration_Test_Dispose		  (void);
#endif

/******************** COPYRIGHT 2012 Embest Tech. Co., Ltd.*****END OF FILE****/
