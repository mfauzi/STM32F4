/**
  ******************************************************************************
  * <h2><center>&copy; COPYRIGHT 2012 Embest Tech. Co., Ltd.</center></h2>
  * @file    bmp.c 
  * @author  CMP Team
  * @version V1.0.0
  * @date    28-December-2012
  * @brief   bmp file.                     
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
#include <stdio.h>
#include <string.h>
#include "stm32f4_discovery_lcd.h"
#include "ff.h"
#include "main.h"

/* Private macro -------------------------------------------------------------*/
#define BMP_16BIT
//#define BMP_24BIT
/* Private variables ---------------------------------------------------------*/

FATFS filesys;        /* volume lable */

/* save the picture count  */
uint32_t pic_counter = 0;

/*read the image buf*/
uint8_t image_buf[1024] = {0};

#ifdef BMP_16BIT
/*
bmp header 
image size :320*240
biBitCount :16
biCompression : BI_BITFIELDS = 3
*/
uint8_t bmp_header[70] = {
  0x42, 0x4D, 0x46, 0x58, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x00, 0x00, 0x00, 0x28, 0x00, 
  0x00, 0x00, 0x40, 0x01, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x01, 0x00, 0x10, 0x00, 0x03, 0x00, 
  0x00, 0x00, 0x00, 0x58, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x00, 0x00, 0xE0, 0x07, 0x00, 0x00, 0x1F, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
#endif


#ifdef BMP_24BIT
/**
  bmp header 
  image size :320*240
  biBitCount :24
  biCompression : BI_BITFIELDS = 0
  */
static uint8_t bmp_header[54]={
  0x42, 0x4D, 0x36, 0x84, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00, 
  0x00, 0x00, 0x40, 0x01, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x84, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
#endif


/* Private function prototypes -----------------------------------------------*/
static int32_t set_pic_count(void);
static int32_t get_pic_count(void);

/**
  * @brief  Capture_Image_TO_Bmp
  * @param  None
  * @retval None
  */
int32_t Capture_Image_TO_Bmp(void)
{
  int32_t  ret = -1;
  int32_t  i = 0;
  int32_t  j = 0;
  int16_t  data_temp = 0;
  uint32_t bw = 0;
#ifdef BMP_24BIT
  uint8_t R,G,B = 0;
#endif
  char  file_str[30] = {0};
  FIL file;        /* File object */


  /* mount the filesys */
  if (f_mount(0, &filesys) != FR_OK) {
    return -1;
  }
  Delay(10);

  sprintf(file_str, "pic%d.bmp",pic_counter);

  ret = f_open(&file, file_str, FA_WRITE | FA_CREATE_ALWAYS);
  if (ret) {
    return ret;
  }

#ifdef BMP_16BIT
  /* write the bmp header */
  ret = f_write(&file, bmp_header, 70, &bw);

  LCD_SetCursor(0,0);
  LCD_WriteRAM_Prepare();
  LCD_ReadRAM();
  for (j = 0; j < 240; j++) {
    for(i=0;i<320;i++) { 
      data_temp = LCD_ReadRAM();
      image_buf[i*2+1] = (data_temp&0xff00) >> 8;
      image_buf[i*2+0] = data_temp & 0x00ff;

    }
    ret = f_write(&file, image_buf, 640, &bw);
  }
#endif

#ifdef BMP_24BIT
  /* write the bmp header */
  ret = f_write(&file, bmp_header, 54, &bw);

  LCD_SetCursor(0,0);
  LCD_WriteRAM_Prepare();
  LCD_ReadRAM();
  for (j = 0; j < 240; j++) {
    for(i=0;i<320;i++) { 
      data_temp = LCD_ReadRAM();
      R = (data_temp>>8)&0xF8;
      G = (data_temp>>3)&0xFC;
      B = (data_temp<<3)&0xF8;
      image_buf[i*3+2] = R;
      image_buf[i*3+1] = G;
      image_buf[i*3+0] = B;
    }
    ret = f_write(&file, image_buf, 960, &bw);
  }
#endif

  ret = f_close(&file);

  f_mount(0, NULL);

  /* statistics the take pictures count */
  pic_counter++;
  set_pic_count();

  return ret;
}

/**
  * @brief  init_picture_count
  * @param  None
  * @retval int32_t : picture count
  */
void init_picture_count(void)
{
  pic_counter = get_pic_count();
}


/**
  * @brief  get the bmp picture count	          
  * @param  None
  * @retval int32_t : picture count
  */
static int32_t get_pic_count(void)
{
  int32_t ret = -1;
  uint32_t bw = 0;
  FIL file;		

  /* mount the filesys */
  if (f_mount(0, &filesys) != FR_OK) {
    return -1;
  }
  Delay(10);

  ret = f_open(&file, "counter.dat", FA_OPEN_EXISTING | FA_READ);
  if (ret != FR_OK) {
    f_close(&file);
    ret = f_open(&file,"counter.dat", FA_CREATE_ALWAYS | FA_WRITE);
    if (ret == FR_OK) {
      pic_counter = 0;
      ret = f_write(&file, &pic_counter, sizeof(uint32_t), &bw);
      f_close(&file);
      f_mount(0, NULL);
      return pic_counter;
    } else {
      f_close(&file);
      f_mount(0, NULL);
      return -1;
    }
  } else {
    ret = f_read(&file, &pic_counter, sizeof(uint32_t), &bw);
    f_close(&file);
    f_mount(0, NULL);
    return pic_counter;
  }
}

/**
  * @brief  save the bmp picture count	          
  * @param  None
  * @retval int32_t : picture count
  */
static int32_t set_pic_count(void)
{
  int32_t ret = -1;
  uint32_t bw = 0;
  FIL file;		

  /* mount the filesys */
  if (f_mount(0, &filesys) != FR_OK) {
    return -1;
  }
  Delay(10);

  ret = f_open(&file, "counter.dat", FA_OPEN_EXISTING | FA_WRITE);
  if (ret == FR_OK) {
    ret = f_write(&file, &pic_counter, sizeof(uint32_t), &bw);
    f_close(&file);
    f_mount(0, NULL);
    return pic_counter;
  } else {
    f_close(&file);
    f_mount(0, NULL);
    return -1;
  }
}
/******************* COPYRIGHT 2012 Embest Tech. Co., Ltd. *****END OF FILE****/
