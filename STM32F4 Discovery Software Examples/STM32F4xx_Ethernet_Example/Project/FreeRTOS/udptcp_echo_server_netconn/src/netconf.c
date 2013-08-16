/**
  ******************************************************************************
  * @file    netconf.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    31-October-2011
  * @brief   Network connection configuration
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
  * @file    netconf.c
  * @author  CMP Team
  * @version V1.0.0
  * @date    28-December-2012
  * @brief   Network connection configuration
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
#include "lwip/mem.h"
#include "lwip/memp.h"
#include "lwip/dhcp.h"
#include "ethernetif.h"
#include "main.h"
#include "netconf.h"
#include "tcpip.h"
#include <stdio.h>

/* Private typedef -----------------------------------------------------------*/
typedef enum 
{ 
  DHCP_START=0,
  DHCP_WAIT_ADDRESS,
  DHCP_ADDRESS_ASSIGNED,
  DHCP_TIMEOUT
} 
DHCP_State_TypeDef;
/* Private define ------------------------------------------------------------*/
#define MAX_DHCP_TRIES 5

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
struct netif xnetif; /* network interface structure */

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Initializes the lwIP stack
  * @param  None
  * @retval None
  */
void LwIP_Init(void)
{
  struct ip_addr ipaddr;
  struct ip_addr netmask;
  struct ip_addr gw;
#ifndef USE_DHCP 
  uint8_t iptab[4];
  uint8_t iptxt[20];
#endif
  /* Create tcp_ip stack thread */
  tcpip_init( NULL, NULL );	

  /* IP address setting & display on STM32_evalboard LCD*/
#ifdef USE_DHCP
  ipaddr.addr = 0;
  netmask.addr = 0;
  gw.addr = 0;
#else
  IP4_ADDR(&ipaddr, IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
  IP4_ADDR(&netmask, NETMASK_ADDR0, NETMASK_ADDR1 , NETMASK_ADDR2, NETMASK_ADDR3);
  IP4_ADDR(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
#ifdef USE_LCD 
  iptab[0] = IP_ADDR3;
  iptab[1] = IP_ADDR2;
  iptab[2] = IP_ADDR1;
  iptab[3] = IP_ADDR0;

  sprintf((char*)iptxt, "  %d.%d.%d.%d", iptab[3], iptab[2], iptab[1], iptab[0]); 

  LCD_DisplayStringLine(Line8, (uint8_t*)"  Static IP address   ");
  LCD_DisplayStringLine(Line9, iptxt);
#endif
#endif

  /* - netif_add(struct netif *netif, struct ip_addr *ipaddr,
            struct ip_addr *netmask, struct ip_addr *gw,
            void *state, err_t (* init)(struct netif *netif),
            err_t (* input)(struct pbuf *p, struct netif *netif))
    
   Adds your network interface to the netif_list. Allocate a struct
  netif and pass a pointer to this structure as the first argument.
  Give pointers to cleared ip_addr structures when using DHCP,
  or fill them with sane numbers otherwise. The state pointer may be NULL.

  The init function pointer must point to a initialization function for
  your ethernet netif interface. The following code illustrates it's use.*/

  netif_add(&xnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &tcpip_input);

 /*  Registers the default network interface. */
  netif_set_default(&xnetif);

 /*  When the netif is fully configured this function must be called.*/
  netif_set_up(&xnetif); 
}

#ifdef USE_DHCP
/**
  * @brief  LwIP_DHCP_Process_Handle
  * @param  None
  * @retval None
  */
void LwIP_DHCP_task(void * pvParameters)
{
  struct ip_addr ipaddr;
  struct ip_addr netmask;
  struct ip_addr gw;
  uint32_t IPaddress;
  uint8_t iptab[4];
  uint8_t iptxt[20];
  uint8_t DHCP_state;  
  DHCP_state = DHCP_START;

  for (;;)
  {
    switch (DHCP_state)
    {
      case DHCP_START:
      {
        dhcp_start(&xnetif);
        IPaddress = 0;
        DHCP_state = DHCP_WAIT_ADDRESS;
#ifdef USE_LCD
        LCD_DisplayStringLine(Line4, (uint8_t*)"     Looking for    ");
        LCD_DisplayStringLine(Line5, (uint8_t*)"     DHCP server    ");
        LCD_DisplayStringLine(Line6, (uint8_t*)"     please wait... ");
#endif
      }
      break;

      case DHCP_WAIT_ADDRESS:
      {
        /* Read the new IP address */
        IPaddress = xnetif.ip_addr.addr;

        if (IPaddress!=0) 
        {
          DHCP_state = DHCP_ADDRESS_ASSIGNED;	
          
          /* Stop DHCP */
          dhcp_stop(&xnetif);

#ifdef USE_LCD      
          iptab[0] = (uint8_t)(IPaddress >> 24);
          iptab[1] = (uint8_t)(IPaddress >> 16);
          iptab[2] = (uint8_t)(IPaddress >> 8);
          iptab[3] = (uint8_t)(IPaddress);

          sprintf((char*)iptxt, "  %d.%d.%d.%d", iptab[3], iptab[2], iptab[1], iptab[0]);  

          LCD_ClearLine(Line4);
          LCD_ClearLine(Line5);
          LCD_ClearLine(Line6);
          /* Display the IP address */
          LCD_DisplayStringLine(Line7, (uint8_t*)"IP address assigned ");
          LCD_DisplayStringLine(Line8, (uint8_t*)"  by a DHCP server  ");
          LCD_DisplayStringLine(Line9, iptxt);
#endif  
          vTaskDelete(NULL);
        }
        else
        {
          /* DHCP timeout */
          if (xnetif.dhcp->tries > MAX_DHCP_TRIES)
          {
            DHCP_state = DHCP_TIMEOUT;

            /* Stop DHCP */
            dhcp_stop(&xnetif);

            /* Static address used */
            IP4_ADDR(&ipaddr, IP_ADDR0 ,IP_ADDR1 , IP_ADDR2 , IP_ADDR3 );
            IP4_ADDR(&netmask, NETMASK_ADDR0, NETMASK_ADDR1, NETMASK_ADDR2, NETMASK_ADDR3);
            IP4_ADDR(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
            netif_set_addr(&xnetif, &ipaddr , &netmask, &gw);

#ifdef USE_LCD   
            LCD_DisplayStringLine(Line7, (uint8_t*)"    DHCP timeout    ");

            iptab[0] = IP_ADDR3;
            iptab[1] = IP_ADDR2;
            iptab[2] = IP_ADDR1;
            iptab[3] = IP_ADDR0;

            sprintf((char*)iptxt, "  %d.%d.%d.%d", iptab[3], iptab[2], iptab[1], iptab[0]); 

            LCD_ClearLine(Line4);
            LCD_ClearLine(Line5);
            LCD_ClearLine(Line6);
            LCD_DisplayStringLine(Line8, (uint8_t*)"  Static IP address   ");
            LCD_DisplayStringLine(Line9, iptxt);
#endif
            vTaskDelete(NULL);
          }
        }
      }
      break;

      default: break;
    }
    /* wait 250 ms */
    vTaskDelay(250);
  }   
}
#endif  /* USE_DHCP */

/*********** Portions COPYRIGHT 2012 Embest Tech. Co., Ltd.*****END OF FILE****/
