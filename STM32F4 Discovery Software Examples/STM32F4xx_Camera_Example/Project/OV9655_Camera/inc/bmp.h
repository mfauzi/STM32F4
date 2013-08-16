/**
  ******************************************************************************
  * <h2><center>&copy; COPYRIGHT 2012 Embest Tech. Co., Ltd.</center></h2>
  * @file    bmp.h 
  * @author  CMP Team
  * @version V1.0.0
  * @date    28-December-2012
  * @brief   Header for bmp.c module.                     
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
#ifndef __BMP_H
#define __BMP_H
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
int32_t Capture_Image_TO_Bmp  (void);
void    init_picture_count    (void);

#endif /* #define __BMP_H */
/******************* COPYRIGHT 2012 Embest Tech. Co., Ltd. *****END OF FILE****/
