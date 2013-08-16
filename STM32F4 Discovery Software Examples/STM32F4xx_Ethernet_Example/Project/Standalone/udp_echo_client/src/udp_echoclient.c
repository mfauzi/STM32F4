/**
  ******************************************************************************
  * @file    udp_echoclient.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    31-October-2011
  * @brief   UDP echo client
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "lwip/pbuf.h"
#include "lwip/udp.h"
#include "lwip/tcp.h"
#include <string.h>
#include <stdio.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void udp_receive_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, struct ip_addr *addr, u16_t port);

u8_t   data[100];
__IO   uint32_t message_count = 0;

__IO   uint8_t  Flag = 0;
struct udp_pcb *upcb;
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Connect to UDP echo server
  * @param  None
  * @retval None
  */
void udp_echoclient_connect(void)
{
//   struct udp_pcb *upcb;
  struct pbuf *p;
  struct ip_addr DestIPaddr;
  err_t err;
       
  
  if (Flag == 0) {
    Flag = 1;
    /* Create a new UDP control block  */
    upcb = udp_new();
  } else {
    sprintf((char*)data, "sending udp client message %d", (int*)message_count);
    p = pbuf_alloc(PBUF_TRANSPORT,strlen((char*)data), PBUF_POOL);
      
    if (p != NULL) {
      /* copy data to pbuf */
      pbuf_take(p, (char*)data, strlen((char*)data));
          
      /* send udp data */
      udp_send(upcb, p); 
        
      /* free pbuf */
      pbuf_free(p);
    } 
    return ;    
  }
  
  if (upcb!=NULL) {
    /*assign destination IP address */
    IP4_ADDR( &DestIPaddr, DEST_IP_ADDR0, DEST_IP_ADDR1, DEST_IP_ADDR2, DEST_IP_ADDR3 );
  
    /* configure destination IP address and port */
    err= udp_connect(upcb, &DestIPaddr, UDP_SERVER_PORT);
    
    if (err == ERR_OK) {
      /* Set a receive callback for the upcb */
      udp_recv(upcb, udp_receive_callback, NULL);
      
      sprintf((char*)data, "sending udp client message %d", (int*)message_count);
  
      /* allocate pbuf from pool*/
      p = pbuf_alloc(PBUF_TRANSPORT,strlen((char*)data), PBUF_POOL);
      
      if (p != NULL) {
        /* copy data to pbuf */
        pbuf_take(p, (char*)data, strlen((char*)data));
          
        /* send udp data */
        udp_send(upcb, p); 
        
        /* free pbuf */
        pbuf_free(p);
      } else {
        #ifdef SERIAL_DEBUG
         printf("\n\r can not allocate pbuf ");
        #endif
      }
    } else {
      #ifdef SERIAL_DEBUG
       printf("\n\r can not connect udp pcb");
      #endif
    }
  } else {
    #ifdef SERIAL_DEBUG
     printf("\n\r can not create udp pcb");
    #endif
  } 
}


/**
  * @brief This function is called when an UDP datagrm has been received on the port UDP_PORT.
  * @param arg user supplied argument (udp_pcb.recv_arg)
  * @param pcb the udp_pcb which received data
  * @param p the packet buffer that was received
  * @param addr the remote IP address from which the packet was received
  * @param port the remote port from which the packet was received
  * @retval None
  */
void udp_receive_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, struct ip_addr *addr, u16_t port)
{
  Flag = 0;
  /*increment message count */
  message_count++;
  
  /* Free receive pbuf */
  pbuf_free(p);
  
  /* free the UDP connection, so we can accept new clients */
  udp_remove(upcb);   
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
