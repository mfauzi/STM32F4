/**
  ******************************************************************************
  * <h2><center>&copy; COPYRIGHT 2012 Embest Tech. Co., Ltd.</center></h2>
  * @file    app.c 
  * @author  CMP Team
  * @version V1.0.0
  * @date    28-December-2012
  * @brief   Main program body.                        
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

#include <includes.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define LED_DLYCOUNT_MAX     500
#define LED_DLYCOUNT_MIN     50
#define LED_DLYCOUNT_STEP    50

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static  OS_STK         App_TaskStartStk[APP_TASK_START_STK_SIZE];
static  OS_STK         App_TaskKbdStk[APP_TASK_KBD_STK_SIZE];
static  CPU_INT16U     led_dly_cout = 100;

/* Private function prototypes -----------------------------------------------*/
static  void  App_TaskCreate       (void);
static  void  App_EventCreate      (void);
static  void  App_TaskStart        (void    *p_arg);
static  void  App_TaskKbd          (void    *p_arg);

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */

int  main (void)
{
    CPU_INT08U  os_err;
    /* Disable all ints until we are ready to accept them. */
    BSP_IntDisAll();                                            
    /* Initialize "uC/OS-II, The Real-Time Kernel".*/
    OSInit();                                                   
    /* Create the start task.*/
    os_err = OSTaskCreateExt((void (*)(void *)) App_TaskStart,  
                             (void          * ) 0,
                             (OS_STK        * )&App_TaskStartStk[APP_TASK_START_STK_SIZE - 1],
                             (INT8U           ) APP_TASK_START_PRIO,
                             (INT16U          ) APP_TASK_START_PRIO,
                             (OS_STK        * )&App_TaskStartStk[0],
                             (INT32U          ) APP_TASK_START_STK_SIZE,
                             (void          * )0,
                             (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));

#if (OS_TASK_NAME_SIZE >= 11)
    OSTaskNameSet(APP_TASK_START_PRIO, (CPU_INT08U *)"Start Task", &os_err);
#endif
    /* Start multitasking (i.e. give control to uC/OS-II).*/
    OSStart();                                                  

    return (0);
}

/**
  * @brief  The startup task.  The uC/OS-II ticker should only be initialize 
  *         once multitasking starts.
  * @param  p_arg  Argument passed to 'App_TaskStart()' by 'OSTaskCreate()'.
  * @retval None
  */
static  void  App_TaskStart (void *p_arg)
{
  (void)p_arg;
  /* Initialize BSP functions. */
  BSP_Init();                                    
  /* Initialize the SysTick. */	
  OS_CPU_SysTickInit();                                       

#if (OS_TASK_STAT_EN > 0)
  /* Determine CPU capacity. */
  OSStatInit();                                               
#endif
  /* Create application events. */
  App_EventCreate();                                          
  /* Create application tasks. */
  App_TaskCreate();                                           

  while (DEF_TRUE) 
  {
    STM_EVAL_LEDToggle(LED4);
    OSTimeDlyHMSM(0, 0, 0, led_dly_cout);
    STM_EVAL_LEDToggle(LED6);
    OSTimeDlyHMSM(0, 0, 0, led_dly_cout);
    STM_EVAL_LEDToggle(LED5);
    OSTimeDlyHMSM(0, 0, 0, led_dly_cout);
    STM_EVAL_LEDToggle(LED3);
    OSTimeDlyHMSM(0, 0, 0, led_dly_cout);
  }
}

/**
  * @brief  Create the application events.
  * @param  None
  * @retval None
  */
static  void  App_EventCreate (void)
{

}

/**
  * @brief  Create the application tasks.
  * @param  None
  * @retval None
  */
static  void  App_TaskCreate (void)
{
    CPU_INT08U  os_err;

    os_err = OSTaskCreateExt((void (*)(void *)) App_TaskKbd,
                             (void          * ) 0,
                             (OS_STK        * )&App_TaskKbdStk[APP_TASK_KBD_STK_SIZE - 1],
                             (INT8U           ) APP_TASK_KBD_PRIO,
                             (INT16U          ) APP_TASK_KBD_PRIO,
                             (OS_STK        * )&App_TaskKbdStk[0],
                             (INT32U          ) APP_TASK_KBD_STK_SIZE,
                             (void          * ) 0,
                             (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));

#if (OS_TASK_NAME_SIZE >= 9)
    OSTaskNameSet(APP_TASK_KBD_PRIO, "Keyboard", &os_err);
#endif
}

/**
  * @brief  Monitor the state of the push buttons and passes messages to AppTaskUserIF()
  * @param  p_arg  Argument passed to 'App_TaskKbd()' by 'OSTaskCreate()'.
  * @retval None
  */
static  void  App_TaskKbd (void *p_arg)
{
  CPU_BOOLEAN  b1_prev;
  CPU_BOOLEAN  b1;


  (void)p_arg;

  b1_prev = DEF_TRUE;

  while (DEF_TRUE) 
  {
    b1 = STM_EVAL_PBGetState(BUTTON_USER);

    if ((b1 == DEF_FALSE) && (b1_prev == DEF_TRUE)) {
      led_dly_cout += LED_DLYCOUNT_STEP;
      if (led_dly_cout >= LED_DLYCOUNT_MAX) {
        led_dly_cout = LED_DLYCOUNT_MIN;
      }
      b1_prev = b1;
    }

    if ((b1 == DEF_TRUE) && (b1_prev==DEF_FALSE)) {
      b1_prev = DEF_TRUE;		
    }
    OSTimeDlyHMSM(0, 0, 0, 20);
  }
}

#if (OS_APP_HOOKS_EN > 0)
/**
  * @brief  This function is called when a task is created.
  * @param  ptcb  is a pointer to the task control block of the task being created.
  * @retval None
  */
void  App_TaskCreateHook (OS_TCB *ptcb)
{

}

/**
  * @brief  This function is called when a task is deleted.
  * @param  ptcb  is a pointer to the task control block of the task being deleted.
  * @retval None
  */
void  App_TaskDelHook (OS_TCB *ptcb)
{
    (void)ptcb;
}

/**
  * @brief  This function is called by OSTaskIdleHook(), which is called by the 
  *         idle task.  This hook has been added to allow you to do such things 
  *         as STOP the CPU to conserve power.
  * @param  None
  * @retval None
  */
#if OS_VERSION >= 251
void  App_TaskIdleHook (void)
{
}
#endif

/**
  * @brief  This function is called by OSTaskStatHook(), which is called every 
  *         second by uC/OS-II's statistics task.  This allows your application 
  *         to add functionality to the statistics task.
  * @param  None
  * @retval None
  */
void  App_TaskStatHook (void)
{
}

/**
  * @brief  This function is called when a task switch is performed.  This 
  *         allows you to perform other operations during a context switch.
  * @param  None
  * @retval None
  */
#if OS_TASK_SW_HOOK_EN > 0
void  App_TaskSwHook (void)
{

}
#endif

/**
  * @brief  This function is called by OSTCBInitHook(), which is called by 
  *         OS_TCBInit() after setting up most of the TCB.
  * @param  ptcb is a pointer to the TCB of the task being created.
  * @retval None
  */
#if OS_VERSION >= 204
void  App_TCBInitHook (OS_TCB *ptcb)
{
    (void)ptcb;
}
#endif

/**
  * @brief  This function is called every tick.
  * @param  None
  * @retval None
  */
#if OS_TIME_TICK_HOOK_EN > 0
void  App_TimeTickHook (void)
{

}
#endif
#endif

/**
  * @brief  This function is called if a task accidentally returns.  In other words,
  *         a task should either be an infinite loop or delete itself when done.
  * @param  ptcb  is a pointer to the task control block of the task that is returning.
  * @retval None
  */
#if OS_VERSION >= 289
void  App_TaskReturnHook (OS_TCB  *ptcb)
{
    (void)ptcb;
}
#endif
/**
  * @brief  assert failed function.
  * @param  None
  * @retval None
  */
void assert_failed(char* file, int line)
{
	
}

/******************** COPYRIGHT 2012 Embest Tech. Co., Ltd.*****END OF FILE****/
