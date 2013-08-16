/**
  ******************************************************************************
  * @file    OV9655_Camera/inc/dcmi_ov9655.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    18-April-2011
  * @brief   Header for dcmi_ov9655.c module
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
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 
/**
  ******************************************************************************
  * <h2><center>&copy; Portions COPYRIGHT 2012 Embest Tech. Co., Ltd.</center></h2>
  * @file    dcmi_ov9655.h 
  * @author  CMP Team
  * @version V1.0.0
  * @date    28-December-2012
  * @brief   Header for dcmi_ov9655.c module                          
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DCMI_OV9655_H
#define __DCMI_OV9655_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* Exported types ------------------------------------------------------------*/
typedef struct
{
  uint8_t Manufacturer_ID1;
  uint8_t Manufacturer_ID2;
  uint8_t Version;
  uint8_t PID; 
} OV9655_IDTypeDef; 

/* Exported constants --------------------------------------------------------*/

/* Use this define to set the maximum delay timeout for the I2C DCMI_SingleRandomWrite()
   and DCMI_SingleRandomRead() operations. Exeeding this timeout delay, the read/write 
   functions will be aborted and return error code (0xFF).
   The period of the delay will depend on the system operating frequency. The following
   value has been set for system running at 120MHz. */
#define DCMI_TIMEOUT_MAX  10000

#define OV9655_DEVICE_WRITE_ADDRESS    0x60
#define OV9655_DEVICE_READ_ADDRESS     0x61

/* OV9655 Registers definition */
#define OV9655_GAIN       0x00
#define OV9655_BLUE       0x01
#define OV9655_RED        0x02
#define OV9655_VREF       0x03  
#define OV9655_COM1       0x04  
#define OV9655_BAVE       0x05
#define OV9655_GbAVE      0x06
#define OV9655_GrAVE      0x07
#define OV9655_RAVE       0x08
#define OV9655_COM2       0x09  
#define OV9655_PID        0x0A  
#define OV9655_VER        0x0B  
#define OV9655_COM3       0x0C 
#define OV9655_COM4       0x0D  
#define OV9655_COM5       0x0E  
#define OV9655_COM6       0x0F  
#define OV9655_AEC        0x10
#define OV9655_CLKRC      0x11 
#define OV9655_COM7       0x12 
#define OV9655_COM8       0x13 
#define OV9655_COM9       0x14  
#define OV9655_COM10      0x15 
#define OV9655_REG16      0x16 
#define OV9655_HSTART     0x17 
#define OV9655_HSTOP      0x18 
#define OV9655_VSTART     0x19 
#define OV9655_VSTOP      0x1A 
#define OV9655_PSHFT      0x1B
#define OV9655_MIDH       0x1C 
#define OV9655_MIDL       0x1D 
#define OV9655_MVFP       0x1E
#define OV9655_BOS        0x20
#define OV9655_GBOS       0x21
#define OV9655_GROS       0x22
#define OV9655_ROS        0x23
#define OV9655_AEW        0x24
#define OV9655_AEB        0x25
#define OV9655_VPT        0x26
#define OV9655_BBIAS      0x27
#define OV9655_GbBIAS     0x28
#define OV9655_PREGAIN    0x29
#define OV9655_EXHCH      0x2A 
#define OV9655_EXHCL      0x2B 
#define OV9655_RBIAS      0x2C
#define OV9655_ADVFL      0x2D
#define OV9655_ADVFH      0x2E
#define OV9655_YAVE       0x2F
#define OV9655_HSYST      0x30
#define OV9655_HSYEN      0x31
#define OV9655_HREF       0x32  
#define OV9655_CHLF       0x33
#define OV9655_AREF1      0x34
#define OV9655_AREF2      0x35
#define OV9655_AREF3      0x36
#define OV9655_ADC1       0x37
#define OV9655_ADC2       0x38
#define OV9655_AREF4      0x39
#define OV9655_TSLB       0x3A
#define OV9655_COM11      0x3B  
#define OV9655_COM12      0x3C  
#define OV9655_COM13      0x3D  
#define OV9655_COM14      0x3E  
#define OV9655_EDGE       0x3F
#define OV9655_COM15      0x40 
#define OV9655_COM16      0x41 
#define OV9655_COM17      0x42 
#define OV9655_MTX1       0x4F 
#define OV9655_MTX2       0x50
#define OV9655_MTX3       0x51
#define OV9655_MTX4       0x52
#define OV9655_MTX5       0x53
#define OV9655_MTX6       0x54 
#define OV9655_BRTN       0x55
#define OV9655_CNST1      0x56
#define OV9655_CNST2      0x57
#define OV9655_MTXS       0x58
#define OV9655_AWBOP1     0x59
#define OV9655_AWBOP2     0x5A
#define OV9655_AWBOP3     0x5B
#define OV9655_AWBOP4     0x5C
#define OV9655_AWBOP5     0x5D
#define OV9655_AWBOP6     0x5E
#define OV9655_BLMT       0x5F
#define OV9655_RLMT       0x60
#define OV9655_GLMT       0x61
#define OV9655_LCC1       0x62
#define OV9655_LCC2       0x63
#define OV9655_LCC3       0x64
#define OV9655_LCC4       0x65
#define OV9655_MANU       0x66
#define OV9655_MANV       0x67
#define OV9655_MANY       0x68
#define OV9655_VARO       0x69
#define OV9655_BD50MAX    0x6A
#define OV9655_DBLV       0x6B
#define OV9655_DNSTH      0x70
#define OV9655_POIDX      0x72 
#define OV9655_PCKDV      0x73 
#define OV9655_XINDX      0x74
#define OV9655_YINDX      0x75
#define OV9655_SLOP       0x7A
#define OV9655_GAM1       0x7B
#define OV9655_GAM2       0x7C
#define OV9655_GAM3       0x7D
#define OV9655_GAM4       0x7E
#define OV9655_GAM5       0x7F
#define OV9655_GAM6       0x80
#define OV9655_GAM7       0x81
#define OV9655_GAM8       0x82
#define OV9655_GAM9       0x83
#define OV9655_GAM10      0x84
#define OV9655_GAM11      0x85
#define OV9655_GAM12      0x86
#define OV9655_GAM13      0x87
#define OV9655_GAM14      0x88
#define OV9655_GAM15      0x89
#define OV9655_COM18      0x8B
#define OV9655_COM19      0x8C
#define OV9655_COM20      0x8D
#define OV9655_DMLNL      0x92
#define OV9655_DMLNH      0x93
#define OV9655_LCC6       0x9D
#define OV9655_LCC7       0x9E
#define OV9655_AECH       0xA1
#define OV9655_BD50       0xA2
#define OV9655_BD60       0xA3
#define OV9655_COM21      0xA4 
#define OV9655_GREEN      0xA6
#define OV9655_VZST       0xA7
#define OV9655_REFA8      0xA8
#define OV9655_REFA9      0xA9
#define OV9655_BLC1       0xAC
#define OV9655_BLC2       0xAD
#define OV9655_BLC3       0xAE
#define OV9655_BLC4       0xAF
#define OV9655_BLC5       0xB0
#define OV9655_BLC6       0xB1
#define OV9655_BLC7       0xB2
#define OV9655_BLC8       0xB3
#define OV9655_CTRLB4     0xB4
#define OV9655_FRSTL      0xB7
#define OV9655_FRSTH      0xB8
#define OV9655_ADBOFF     0xBC
#define OV9655_ADROFF     0xBD
#define OV9655_ADGbOFF    0xBE
#define OV9655_ADGrOFF    0xBF
#define OV9655_COM23      0xC4 
#define OV9655_BD60MAX    0xC5 
#define OV9655_COM24      0xC7 

/* Registers bit definition */
/* COM1 Register */
#define CCIR656_FORMAT  0x40
#define HREF_SKIP_0     0x00
#define HREF_SKIP_1     0x04
#define HREF_SKIP_3     0x08

/* COM2 Register */
#define SOFT_SLEEP_MODE  0x10	
#define ODCAP_1x         0x00	
#define ODCAP_2x         0x01	
#define ODCAP_3x         0x02	
#define ODCAP_4x         0x03
	
/* COM3 Register */
#define COLOR_BAR_OUTPUT         0x80
#define OUTPUT_MSB_LAS_SWAP      0x40
#define PIN_REMAP_RESETB_EXPST   0x08 
#define RGB565_FORMAT            0x00 
#define RGB_OUTPUT_AVERAGE       0x04 
#define SINGLE_FRAME             0x01

/* COM5 Register */
#define SLAM_MODE_ENABLE      0x40
#define EXPOSURE_NORMAL_MODE  0x01

/* COM7 Register */
#define SCCB_REG_RESET                       0x80
#define FORMAT_CTRL_15fpsVGA                 0x00
#define FORMAT_CTRL_30fpsVGA_NoVArioPixel    0x50
#define FORMAT_CTRL_30fpsVGA_VArioPixel      0x60
#define OUTPUT_FORMAT_RAWRGB                 0x00
#define OUTPUT_FORMAT_RAWRGB_DATA            0x00
#define OUTPUT_FORMAT_RAWRGB_INTERP          0x01
#define OUTPUT_FORMAT_YUV                    0x02
#define OUTPUT_FORMAT_RGB                    0x03

/* COM9 Register */
#define GAIN_2x         0x00	
#define GAIN_4x         0x10	
#define GAIN_8x         0x20	
#define GAIN_16x        0x30	
#define GAIN_32x        0x40	
#define GAIN_64x        0x50	
#define GAIN_128x       0x60	
#define DROP_VSYNC      0x04	
#define DROP_HREF       0x02
	
/* COM10 Register */
#define RESETb_REMAP_SLHS    0x80
#define HREF_CHANGE_HSYNC    0x40
#define PCLK_ON              0x00
#define PCLK_OFF             0x20
#define PCLK_POLARITY_REV    0x10
#define HREF_POLARITY_REV    0x08
#define RESET_ENDPOINT       0x04
#define VSYNC_NEG            0x02
#define HSYNC_NEG            0x01

/* TSLB Register */
#define PCLK_DELAY_0         0x00
#define PCLK_DELAY_2         0x40
#define PCLK_DELAY_4         0x80
#define PCLK_DELAY_6         0xC0
#define OUTPUT_BITWISE_REV   0x20
#define UV_NORMAL            0x00
#define UV_FIXED             0x10
#define YUV_SEQ_YUYV         0x00
#define YUV_SEQ_YVYU         0x02
#define YUV_SEQ_VYUY         0x04
#define YUV_SEQ_UYVY         0x06
#define BANDING_FREQ_50      0x02

#define RGB_NORMAL   0x00 
#define RGB_565      0x10 
#define RGB_555      0x30 

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void DCMI_Control_IO_Init(void);
void DCMI_OV9655_Reset(void);
void DCMI_OV9655_QQVGASizeSetup(void);
void DCMI_OV9655_QVGASizeSetup(void);
void DCMI_OV9655_ReadID(OV9655_IDTypeDef* OV9655ID);
void DCMI_OV9655_SetPrescaler(uint8_t OV9655_Prescaler);
void DCMI_OV9655_SelectOutputFormat(uint8_t OV9655_OuputFormat);
void DCMI_OV9655_SelectFormatResolution(uint8_t OV9655_FormatResolution);
void DCMI_OV9655_SetRegister(uint8_t OV9655_Register, uint8_t Register_Val);
void DCMI_OV9655_HREFControl(uint8_t OV9665_HREFControl);
void DCMI_OV9655_SelectRGBOption(uint8_t OV9665_RGBOption);
uint8_t DCMI_SingleRandomWrite(uint8_t Device, uint16_t Addr, uint8_t Data);
uint8_t DCMI_SingleRandomRead(uint8_t Device, uint16_t Addr);

#endif /* __DCMI_OV9655_H */

/*********** Portions COPYRIGHT 2012 Embest Tech. Co., Ltd.*****END OF FILE****/

