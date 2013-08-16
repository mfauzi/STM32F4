/**
  ******************************************************************************
  * @file    httpd_cg_ssi.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    31-October-2011
  * @brief   Webserver SSI and CGI handlers
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
  * @file    httpd_cg_ssi.c
  * @author  CMP Team
  * @version V1.0.0
  * @date    28-December-2012
  * @brief   Webserver SSI and CGI handlers
  *          Modified to support the STM32F4DISCOVERY, STM32F4DIS-BB and
  *          STM32F4DIS-LCD modules. 
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


#include "lwip/debug.h"
#include "httpd.h"
#include "lwip/tcp.h"
#include "fs.h"
#include "main.h"

#include <string.h>
#include <stdlib.h>

tSSIHandler ADC_Page_SSI_Handler;
uint32_t ADC_not_configured=1;

/* we will use character "t" as tag for CGI */
char const* TAGCHAR="t";
char const** TAGS=&TAGCHAR;

/* CGI handler for LED control */ 
const char * LEDS_CGI_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);

/* Html request for "/leds.cgi" will start LEDS_CGI_Handler */
const tCGI LEDS_CGI={"/leds.cgi", LEDS_CGI_Handler};

/* Cgi call table, only one CGI used */
tCGI CGI_TAB[1];


/**
  * @brief  Configures the ADC.
  * @param  None
  * @retval None
  */
static void ADC_Configuration(void)
{
  ADC_InitTypeDef ADC_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable ADC1 clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

  /* Configure ADC Channel 7 as analog input */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* ADC Common Init */
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div6;
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles; 
  ADC_CommonInit(&ADC_CommonInitStructure); 

  /* ADC1 Configuration ------------------------------------------------------*/
  ADC_StructInit(&ADC_InitStructure);
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; 
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;
  ADC_Init(ADC1, &ADC_InitStructure);

  /* ADC1 Regular Channel Config */
  ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 1, ADC_SampleTime_56Cycles);

  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);

  /* ADC1 regular Software Start Conv */ 
  ADC_SoftwareStartConv(ADC1);
}

/**
  * @brief  ADC_Handler : SSI handler for ADC page 
  */
u16_t ADC_Handler(int iIndex, char *pcInsert, int iInsertLen)
{
  /* We have only one SSI handler iIndex = 0 */
  if (iIndex ==0) {  
    char Digit1=0, Digit2=0, Digit3=0, Digit4=0; 
    uint32_t ADCVal = 0;        

    /* configure ADC if not yet configured */
    if (ADC_not_configured ==1) {
       ADC_Configuration();
       ADC_not_configured=0;
    }
     
    /* get ADC conversion value */
    ADCVal = ADC_GetConversionValue(ADC1);
    
    /* convert to Voltage,  step = 0.8 mV */
    ADCVal = (uint32_t)(ADCVal * 0.8);  
     
    /* get digits to display */
     
    Digit1= ADCVal/1000;
    Digit2= (ADCVal-(Digit1*1000))/100 ;
    Digit3= (ADCVal-((Digit1*1000)+(Digit2*100)))/10;
    Digit4= ADCVal -((Digit1*1000)+(Digit2*100)+ (Digit3*10));
        
    /* prepare data to be inserted in html */
    *pcInsert       = (char)(Digit1+0x30);
    *(pcInsert + 1) = (char)(Digit2+0x30);
    *(pcInsert + 2) = (char)(Digit3+0x30);
    *(pcInsert + 3) = (char)(Digit4+0x30);
    
    /* 4 characters need to be inserted in html*/
    return 4;
  }
  return 0;
}

/**
  * @brief  CGI handler for LEDs control 
  */
const char * LEDS_CGI_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
  uint32_t i=0;
  
  /* We have only one SSI handler iIndex = 0 */
  if (iIndex==0) {
    /* All leds off */
    STM_EVAL_LEDOff(LED4);
    
    /* Check cgi parameter : example GET /leds.cgi?led=2&led=4 */
    for (i=0; i<iNumParams; i++) {
      /* check parameter "led" */
      if (strcmp(pcParam[i] , "led")==0) {
          STM_EVAL_LEDOn(LED4);
      }
    }
  }
  /* uri to send after cgi call*/
  return "/STM32F4x7LED.html";  
}

/**
 * Initialize SSI handlers
 */
void httpd_ssi_init(void)
{  
  /* configure SSI handlers (ADC page SSI) */
  http_set_ssi_handler(ADC_Handler, (char const **)TAGS, 1);
}

/**
 * Initialize CGI handlers
 */
void httpd_cgi_init(void)
{ 
  /* configure CGI handlers (LEDs control CGI) */
  CGI_TAB[0] = LEDS_CGI;
  http_set_cgi_handlers(CGI_TAB, 1);
}

/*********** Portions COPYRIGHT 2012 Embest Tech. Co., Ltd.*****END OF FILE****/
