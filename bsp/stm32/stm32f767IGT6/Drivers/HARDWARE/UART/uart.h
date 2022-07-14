
#ifndef  _UART_H_
#define  _UART_H_


#include "qx_rtt.h"


#define USART_REC_LEN   100 	
#define RXBUFFERSIZE   1 //缓存大小
#define USART3_MAX_RECV_LEN		400					
#define USART3_MAX_SEND_LEN		400		

extern uint16_t USART3_RX_STA;
extern __align(8) uint8_t USART3_TX_BUF[USART3_MAX_SEND_LEN];  	
extern uint8_t USART3_RX_BUF[USART3_MAX_RECV_LEN];
void uart1_init(uint32_t bound);
void usart3_init(uint32_t bound);
void USART3_Send(char* fmt, ...);

#endif
