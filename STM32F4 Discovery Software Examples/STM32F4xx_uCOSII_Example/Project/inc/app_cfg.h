/**
  ******************************************************************************
  * <h2><center>&copy; COPYRIGHT 2012 Embest Tech. Co., Ltd.</center></h2>
  * @file    app_cfg.h 
  * @author  CMP Team
  * @version V1.0.0
  * @date    28-December-2012
  * @brief   application configuration file.                       
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
#ifndef  __APP_CFG_H__
#define  __APP_CFG_H__

/* Exported define -----------------------------------------------------------*/
#define  APP_OS_PROBE_EN                         DEF_DISABLED 

#define  APP_TASK_START_PRIO                               3
#define  APP_TASK_KBD_PRIO                                 4
#define  OS_TASK_TMR_PRIO                (OS_LOWEST_PRIO - 2)

#define  APP_TASK_START_STK_SIZE                         512
#define  APP_TASK_KBD_STK_SIZE                           128

#define  uC_CFG_OPTIMIZE_ASM_EN                 DEF_ENABLED
#define  LIB_STR_CFG_FP_EN                      DEF_DISABLED


#endif

/******************** COPYRIGHT 2012 Embest Tech. Co., Ltd.*****END OF FILE****/
