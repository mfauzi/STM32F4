/**
  ******************************************************************************
  * <h2><center>&copy; COPYRIGHT 2012 Embest Tech. Co., Ltd.</center></h2>
  * @file    bsp_int.c 
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

#define  BSP_INT_MODULE
#include <bsp.h>

/* Private define ------------------------------------------------------------*/

#define  BSP_INT_SRC_NBR                                 60

/* Private function prototypes -----------------------------------------------*/
static  CPU_FNCT_VOID  BSP_IntVectTbl[BSP_INT_SRC_NBR];
static  void  BSP_IntHandler     (CPU_DATA  int_id);
static  void  BSP_IntHandlerDummy(void);

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Clear interrupt.
  * @param  int_id      Interrupt to clear.
  * @retval None
  */

void  BSP_IntClr (CPU_DATA  int_id)
{

}

/**
  * @brief  Disable interrupt.
  * @param  int_id      Interrupt to disable.
  * @retval None
  */
void  BSP_IntDis (CPU_DATA  int_id)
{
    if (int_id < BSP_INT_SRC_NBR) {
        CPU_IntSrcDis(int_id + 16);
    }
}

/**
  * @brief  Disable ALL interrupts.
  * @param  None
  * @retval None
  */
void  BSP_IntDisAll (void)
{
    CPU_IntDis();
}

/**
  * @brief  Enable interrupt.
  * @param  int_id      Interrupt to enable.
  * @retval None
  */
void  BSP_IntEn (CPU_DATA  int_id)
{
    if (int_id < BSP_INT_SRC_NBR) {
        CPU_IntSrcEn(int_id + 16);
    }
}

/**
  * @brief  Assign ISR handler.
  * @param  int_id  Interrupt for which vector will be set.
  *         isr     Handler to assign
  * @retval None
  */
void  BSP_IntVectSet (CPU_DATA       int_id,
                      CPU_FNCT_VOID  isr)
{
#if (CPU_CFG_CRITICAL_METHOD == CPU_CRITICAL_METHOD_STATUS_LOCAL)
    CPU_SR   cpu_sr;
#endif


    if (int_id < BSP_INT_SRC_NBR) {
        CPU_CRITICAL_ENTER();
        BSP_IntVectTbl[int_id] = isr;
        CPU_CRITICAL_EXIT();
    }
}

/**
  * @brief  Assign ISR priority.
  * @param  int_id  Interrupt for which vector will be set.
  *         prio    Priority to assign
  * @retval None
  */
void  BSP_IntPrioSet (CPU_DATA    int_id,
                      CPU_INT08U  prio)
{
#if (CPU_CFG_CRITICAL_METHOD == CPU_CRITICAL_METHOD_STATUS_LOCAL)
    CPU_SR    cpu_sr;
#endif


    if (int_id < BSP_INT_SRC_NBR) {
        CPU_CRITICAL_ENTER();
        CPU_IntSrcPrioSet(int_id + 16, prio);
        CPU_CRITICAL_EXIT();
    }
}

/**
  * @brief  Initialize interrupts:
  * @param  None
  * @retval None
  */
void  BSP_IntInit (void)
{
    CPU_DATA  int_id;


    for (int_id = 0; int_id < BSP_INT_SRC_NBR; int_id++) {
        BSP_IntVectSet(int_id, BSP_IntHandlerDummy);
    }
}

/**
  * @brief  Handle an interrupt.
  * @param  None
  * @retval None
  */
void  BSP_IntHandlerWWDG          (void)  { BSP_IntHandler(BSP_INT_ID_WWDG);            }
void  BSP_IntHandlerPVD           (void)  { BSP_IntHandler(BSP_INT_ID_PVD);             }
void  BSP_IntHandlerTAMPER        (void)  { BSP_IntHandler(BSP_INT_ID_TAMPER);          }
void  BSP_IntHandlerRTC           (void)  { BSP_IntHandler(BSP_INT_ID_RTC);             }
void  BSP_IntHandlerFLASH         (void)  { BSP_IntHandler(BSP_INT_ID_FLASH);           }
void  BSP_IntHandlerRCC           (void)  { BSP_IntHandler(BSP_INT_ID_RCC);             }
void  BSP_IntHandlerEXTI0         (void)  { BSP_IntHandler(BSP_INT_ID_EXTI0);           }
void  BSP_IntHandlerEXTI1         (void)  { BSP_IntHandler(BSP_INT_ID_EXTI1);           }
void  BSP_IntHandlerEXTI2         (void)  { BSP_IntHandler(BSP_INT_ID_EXTI2);           }
void  BSP_IntHandlerEXTI3         (void)  { BSP_IntHandler(BSP_INT_ID_EXTI3);           }
void  BSP_IntHandlerEXTI4         (void)  { BSP_IntHandler(BSP_INT_ID_EXTI4);           }
void  BSP_IntHandlerDMA1_CH1      (void)  { BSP_IntHandler(BSP_INT_ID_DMA1_CH1);        }
void  BSP_IntHandlerDMA1_CH2      (void)  { BSP_IntHandler(BSP_INT_ID_DMA1_CH2);        }
void  BSP_IntHandlerDMA1_CH3      (void)  { BSP_IntHandler(BSP_INT_ID_DMA1_CH3);        }
void  BSP_IntHandlerDMA1_CH4      (void)  { BSP_IntHandler(BSP_INT_ID_DMA1_CH4);        }
void  BSP_IntHandlerDMA1_CH5      (void)  { BSP_IntHandler(BSP_INT_ID_DMA1_CH5);        }
void  BSP_IntHandlerDMA1_CH6      (void)  { BSP_IntHandler(BSP_INT_ID_DMA1_CH6);        }
void  BSP_IntHandlerDMA1_CH7      (void)  { BSP_IntHandler(BSP_INT_ID_DMA1_CH7);        }
void  BSP_IntHandlerADC1_2        (void)  { BSP_IntHandler(BSP_INT_ID_ADC1_2);          }
void  BSP_IntHandlerUSB_HP_CAN_TX (void)  { BSP_IntHandler(BSP_INT_ID_USB_HP_CAN_TX);   }
void  BSP_IntHandlerUSB_LP_CAN_RX0(void)  { BSP_IntHandler(BSP_INT_ID_USB_LP_CAN_RX0);  }
void  BSP_IntHandlerCAN_RX1       (void)  { BSP_IntHandler(BSP_INT_ID_CAN_RX1);         }
void  BSP_IntHandlerCAN_SCE       (void)  { BSP_IntHandler(BSP_INT_ID_CAN_SCE);         }
void  BSP_IntHandlerEXTI9_5       (void)  { BSP_IntHandler(BSP_INT_ID_EXTI9_5);         }
void  BSP_IntHandlerTIM1_BRK      (void)  { BSP_IntHandler(BSP_INT_ID_TIM1_BRK);        }
void  BSP_IntHandlerTIM1_UP       (void)  { BSP_IntHandler(BSP_INT_ID_TIM1_UP);         }
void  BSP_IntHandlerTIM1_TRG_COM  (void)  { BSP_IntHandler(BSP_INT_ID_TIM1_TRG_COM);    }
void  BSP_IntHandlerTIM1_CC       (void)  { BSP_IntHandler(BSP_INT_ID_TIM1_CC);         }
void  BSP_IntHandlerTIM2          (void)  { BSP_IntHandler(BSP_INT_ID_TIM2);            }
void  BSP_IntHandlerTIM3          (void)  { BSP_IntHandler(BSP_INT_ID_TIM3);            }
void  BSP_IntHandlerTIM4          (void)  { BSP_IntHandler(BSP_INT_ID_TIM4);            }
void  BSP_IntHandlerI2C1_EV       (void)  { BSP_IntHandler(BSP_INT_ID_I2C1_EV);         }
void  BSP_IntHandlerI2C1_ER       (void)  { BSP_IntHandler(BSP_INT_ID_I2C1_ER);         }
void  BSP_IntHandlerI2C2_EV       (void)  { BSP_IntHandler(BSP_INT_ID_I2C2_EV);         }
void  BSP_IntHandlerI2C2_ER       (void)  { BSP_IntHandler(BSP_INT_ID_I2C2_ER);         }
void  BSP_IntHandlerSPI1          (void)  { BSP_IntHandler(BSP_INT_ID_SPI1);            }
void  BSP_IntHandlerSPI2          (void)  { BSP_IntHandler(BSP_INT_ID_SPI2);            }
void  BSP_IntHandlerUSART1        (void)  { BSP_IntHandler(BSP_INT_ID_USART1);          }
void  BSP_IntHandlerUSART2        (void)  { BSP_IntHandler(BSP_INT_ID_USART2);          }
void  BSP_IntHandlerUSART3        (void)  { BSP_IntHandler(BSP_INT_ID_USART3);          }
void  BSP_IntHandlerEXTI15_10     (void)  { BSP_IntHandler(BSP_INT_ID_EXTI15_10);       }
void  BSP_IntHandlerRTCAlarm      (void)  { BSP_IntHandler(BSP_INT_ID_RTCAlarm);        }
void  BSP_IntHandlerUSBWakeUp     (void)  { BSP_IntHandler(BSP_INT_ID_USBWakeUp);       }
void  BSP_IntHandlerTIM8_BRK      (void)  { BSP_IntHandler(BSP_INT_ID_TIM8_BRK);        }
void  BSP_IntHandlerTIM8_UP       (void)  { BSP_IntHandler(BSP_INT_ID_TIM8_UP);         }
void  BSP_IntHandlerTIM8_TRG_COM  (void)  { BSP_IntHandler(BSP_INT_ID_TIM8_TRG_COM);    }
void  BSP_IntHandlerTIM8_CC       (void)  { BSP_IntHandler(BSP_INT_ID_TIM8_CC);         }
void  BSP_IntHandlerADC3          (void)  { BSP_IntHandler(BSP_INT_ID_ADC3);            }
void  BSP_IntHandlerFSMC          (void)  { BSP_IntHandler(BSP_INT_ID_FSMC);            }
void  BSP_IntHandlerSDIO          (void)  { BSP_IntHandler(BSP_INT_ID_SDIO);            }
void  BSP_IntHandlerTIM5          (void)  { BSP_IntHandler(BSP_INT_ID_TIM5);            }
void  BSP_IntHandlerSPI3          (void)  { BSP_IntHandler(BSP_INT_ID_SPI3);            }
void  BSP_IntHandlerUART4         (void)  { BSP_IntHandler(BSP_INT_ID_UART4);           }
void  BSP_IntHandlerUART5         (void)  { BSP_IntHandler(BSP_INT_ID_UART5);           }
void  BSP_IntHandlerTIM6          (void)  { BSP_IntHandler(BSP_INT_ID_TIM6);            }
void  BSP_IntHandlerTIM7          (void)  { BSP_IntHandler(BSP_INT_ID_TIM7);            }
void  BSP_IntHandlerDMA2_CH1      (void)  { BSP_IntHandler(BSP_INT_ID_DMA2_CH1);        }
void  BSP_IntHandlerDMA2_CH2      (void)  { BSP_IntHandler(BSP_INT_ID_DMA2_CH2);        }
void  BSP_IntHandlerDMA2_CH3      (void)  { BSP_IntHandler(BSP_INT_ID_DMA2_CH3);        }
void  BSP_IntHandlerDMA2_CH4_5    (void)  { BSP_IntHandler(BSP_INT_ID_DMA2_CH4_5);      }

/**
  * @brief  Central interrupt handler.
  * @param  int_id  Interrupt that will be handled.
  * @retval None
  */
static  void  BSP_IntHandler (CPU_DATA  int_id)
{
#if (CPU_CFG_CRITICAL_METHOD == CPU_CRITICAL_METHOD_STATUS_LOCAL)
    CPU_SR         cpu_sr;
#endif
    CPU_FNCT_VOID  isr;

    /* Tell uC/OS-II that we are starting an ISR            */
    CPU_CRITICAL_ENTER();                                       
    OSIntNesting++;
    CPU_CRITICAL_EXIT();

    if (int_id < BSP_INT_SRC_NBR) {
        isr = BSP_IntVectTbl[int_id];
        if (isr != (CPU_FNCT_VOID)0) {
            isr();
        }
    }
    /* Tell uC/OS-II that we are leaving the ISR            */
    OSIntExit();                                                
}

/**
  * @brief  Dummy interrupt handler.
  * @param  None
  * @retval None
  */
static  void  BSP_IntHandlerDummy (void)
{

}

/******************** COPYRIGHT 2012 Embest Tech. Co., Ltd.*****END OF FILE****/
