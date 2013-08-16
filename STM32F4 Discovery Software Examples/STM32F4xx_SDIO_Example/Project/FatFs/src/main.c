/**
  ******************************************************************************
  * <h2><center>&copy; COPYRIGHT 2012 Embest Tech. Co., Ltd.</center></h2>
  * @file    main.c
  * @author  CMP Team
  * @version V1.0.0
  * @date    28-December-2012
  * @brief   This example provides a example of how to use the SDIO firmware library 
  *          and an associate driver to implement Fatfs on the SD Card memory.      
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
#include <stdio.h>
#include "stm32f4xx.h"
#include "stm32f4_discovery_sdio_sd.h"
#include "stm32f4_discovery_debug.h"
#include "ff.h"


/** @addtogroup STM32F4xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup SDIO_uSDCard
  * @{
  */

/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
SD_Error Status = SD_OK;

FATFS filesystem;		/* volume lable */

FRESULT ret;			  /* Result code */

FIL file;				    /* File object */

DIR dir;				    /* Directory object */

FILINFO fno;			  /* File information object */

UINT bw, br;

uint8_t buff[128];

/* Private function prototypes -----------------------------------------------*/
static void Delay(__IO uint32_t nCount);
static void fault_err (FRESULT rc);
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f4xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f4xx.c file
     */

  /* Initialize Debug uart available on Devkit407 board *******************/
  STM32f4_Discovery_Debug_Init();

  /* Initialize LEDs available on Devkit407 board *************************/
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED6);

  /* mount the filesystem */
  if (f_mount(0, &filesystem) != FR_OK) {
    printf("could not open filesystem \n\r");
  }
  Delay(10);	

  printf("Open a test file (message.txt) \n\r");
  ret = f_open(&file, "MESSAGE.TXT", FA_READ);
  if (ret) {
    printf("not exist the test file (message.txt)\n\r");
  } else {
    printf("Type the file content\n\r");
    for (;;) {
      ret = f_read(&file, buff, sizeof(buff), &br);	/* Read a chunk of file */
      if (ret || !br) {
        break;			/* Error or end of file */
      }
      buff[br] = 0;
      printf("%s",buff);
      printf("\n\r");
    }
    if (ret) {
      printf("Read the file error\n\r");
      fault_err(ret);
    }

    printf("Close the file\n\r");
    ret = f_close(&file);
    if (ret) {
      printf("Close the file error\n\r");
    }
  }

  /*  hello.txt write test*/
  Delay(50);	
  printf("Create a new file (hello.txt)\n\r");
  ret = f_open(&file, "HELLO.TXT", FA_WRITE | FA_CREATE_ALWAYS);
  if (ret) {
    printf("Create a new file error\n\r");
    fault_err(ret);
  } else {
    printf("Write a text data. (hello.txt)\n\r");
    ret = f_write(&file, "Hello world!", 14, &bw);
    if (ret) {
      printf("Write a text data to file error\n\r");
    } else {
      printf("%u bytes written\n\r", bw);
    }
    Delay(50);
    printf("Close the file\n\r");
    ret = f_close(&file);
    if (ret) {
      printf("Close the hello.txt file error\n\r");			
    }
  }

  /*  hello.txt read test*/
  Delay(50);	
  printf("read the file (hello.txt)\n\r");
  ret = f_open(&file, "HELLO.TXT", FA_READ);
  if (ret) {
    printf("open hello.txt file error\n\r");
  } else {
    printf("Type the file content(hello.txt)\n\r");
    for (;;) {
      ret = f_read(&file, buff, sizeof(buff), &br);	/* Read a chunk of file */
      if (ret || !br) {
        break;			/* Error or end of file */
      }
      buff[br] = 0;
      printf("%s",buff);
      printf("\n\r");
    }
    if (ret) {
      printf("Read file (hello.txt) error\n\r");
      fault_err(ret);
    }

    printf("Close the file (hello.txt)\n\r");
    ret = f_close(&file);
    if (ret) {
      printf("Close the file (hello.txt) error\n\r");
    }
  }

  /*  directory display test*/
  Delay(50);	
  printf("Open root directory\n\r");
  ret = f_opendir(&dir, "");
  if (ret) {
    printf("Open root directory error\n\r");	
  } else {
    printf("Directory listing...\n\r");
    for (;;) {
      ret = f_readdir(&dir, &fno);		/* Read a directory item */
      if (ret || !fno.fname[0]) {
        break;	/* Error or end of dir */
      }
      if (fno.fattrib & AM_DIR) {
        printf("  <dir>  %s\n\r", fno.fname);
      } else {
        printf("%8lu  %s\n\r", fno.fsize, fno.fname);
      }
    }
    if (ret) {
      printf("Read a directory error\n\r");
      fault_err(ret);
    }
  }
  Delay(50);	
  printf("Test completed\n\r");

  /* Infinite loop */
  while (1) {
    STM_EVAL_LEDToggle(LED3);
    Delay(100);	
  }
}

/**
 * @brief   FatFs err dispose
 * @param  None
 * @retval None
*/
static void fault_err (FRESULT rc)
{
  const char *str =
                    "OK\0" "DISK_ERR\0" "INT_ERR\0" "NOT_READY\0" "NO_FILE\0" "NO_PATH\0"
                    "INVALID_NAME\0" "DENIED\0" "EXIST\0" "INVALID_OBJECT\0" "WRITE_PROTECTED\0"
                    "INVALID_DRIVE\0" "NOT_ENABLED\0" "NO_FILE_SYSTEM\0" "MKFS_ABORTED\0" "TIMEOUT\0"
                    "LOCKED\0" "NOT_ENOUGH_CORE\0" "TOO_MANY_OPEN_FILES\0";
  FRESULT i;

  for (i = (FRESULT)0; i != rc && *str; i++) {
    while (*str++) ;
  }
  printf("rc=%u FR_%s\n\r", (UINT)rc, str);
  STM_EVAL_LEDOn(LED6);
  while(1);
}

/**
  * @brief  Delay
  * @param  None
  * @retval None
  */
static void Delay(__IO uint32_t nCount)
{
  __IO uint32_t index = 0; 
  for (index = (100000 * nCount); index != 0; index--);
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
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
  {}
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/******************** COPYRIGHT 2012 Embest Tech. Co., Ltd.*****END OF FILE****/
