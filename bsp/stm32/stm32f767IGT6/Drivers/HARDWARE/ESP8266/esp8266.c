
#include "qx_rtt.h"


/************************  接收应答检测  *******************************/
uint8_t esp8266_ack_check(char *Cmd ,char *Str)
{
    char *str = NULL;
    USART3_RX_BUF[USART3_RX_STA & 0x7fff] = 0 ;		//添加结束符
    str = strstr((const char*)USART3_RX_BUF,(const char*)Str);
	if(str != NULL){
		printf("AT_CMD : %s\t ACK : %s\r\n",Cmd,str);
		return 0;		
	}
	else{
		printf("ack error!\r\n");
	}
    return 1;
}

/************************  接收数据检测  *******************************/
void esp8266_data_check(void)
{
    char *str = NULL;
	char Str[4] = "IPD";
    USART3_RX_BUF[USART3_RX_STA & 0x7fff] = 0 ;		//添加结束符
    str = strstr((const char*)USART3_RX_BUF,(const char*)Str);
	if(str != NULL){
		USART3_RX_STA = 0 ;	
	}
}

/************************  发送指令  *******************************/
uint8_t esp8266_at_cmd_send(char *CMD ,char *ACK , uint16_t timeout)
{
    uint8_t res = 0 ;
    USART3_RX_STA = 0 ;
    USART3_Send("%s\r\n",CMD);

    if(ACK && timeout){
        while (--timeout){
            rt_thread_mdelay(10);
            if(USART3_RX_STA & 0x8000){
                if(esp8266_ack_check(CMD,ACK)==0){
					USART3_RX_STA = 0 ;
					break;
				}              
                USART3_RX_STA = 0 ;
            }

        }
        if(timeout == 0)
        	res = 1 ;
    }
    return res;
}

/************************  发送数据  *******************************/
void esp8266_data_send(char *DATA)
{
	uint16_t timeout = 100;
	USART3_RX_STA = 0 ;
	USART3_Send("%s",DATA);	

    while (--timeout){
        rt_thread_mdelay(10);
        if(USART3_RX_STA & 0x8000){
            if(esp8266_ack_check("NONE","OK")==0){
					USART3_RX_STA = 0 ;
					break;
				}                
                USART3_RX_STA = 0 ;
            }
        }
}

/************************  退出透传  *******************************/
uint8_t esp8266_quit_trans(void)
{
	while((USART3->ISR&0X40)==0);
	USART3->TDR='+';      
	rt_thread_mdelay(15);
	while((USART3->ISR&0X40)==0);	
	USART3->TDR='+';      
	rt_thread_mdelay(15);				
	while((USART3->ISR&0X40)==0);
	USART3->TDR='+';      
	rt_thread_mdelay(500);
	return esp8266_at_cmd_send("AT","OK",20);
}


/************************  AP-TCP-SERVER配置  *******************************/
void esp8266_ap_tcp_server(char *SSID , char *PWD , uint8_t CH , uint8_t ECN , char *portnum)
{
	char *pt = (char *)rt_malloc(50);
	
	esp8266_at_cmd_send("AT+CWMODE=2","OK",50);
	
	esp8266_at_cmd_send("AT+RST","OK",50);
	rt_thread_mdelay(2000);
	
	sprintf(pt,"AT+CWSAP=\"%s\",\"%s\",%d,%d",SSID, PWD, CH, ECN);
	printf("AP-TCP_SERVER: %s\r\n",pt);
	esp8266_at_cmd_send(pt,"OK",200);
	
	esp8266_at_cmd_send("AT+CIPMUX=1","OK",50);
	
	sprintf(pt,"AT+CIPSERVER=1,%s", portnum);
	printf("AP-TCP_SERVER: %s\r\n",pt);
	esp8266_at_cmd_send(pt,"OK",100);
	
	if(pt != RT_NULL){
		rt_free((void *)pt);
	}
}

/************************  AP-TCP-CLIENT配置  *******************************/
void esp8266_ap_tcp_client(char *SSID , char *PWD , uint8_t CH , uint8_t ECN , char * tcp_ip , char *portnum)
{

	char *pt = (char *)rt_malloc(50);
	esp8266_quit_trans();
	esp8266_at_cmd_send("AT+CWMODE=2","OK",50);
	
	esp8266_at_cmd_send("AT+RST","OK",200);
	rt_thread_mdelay(2000);
	
	sprintf(pt,"AT+CWSAP=\"%s\",\"%s\",%d,%d",SSID, PWD, CH, ECN);
	printf("AP-TCP_CLIENT: %s\r\n",pt);
	esp8266_at_cmd_send(pt,"OK",200);
	
	esp8266_at_cmd_send("AT+CIPMUX=0","OK",100);
	
	sprintf(pt,"AT+CIPSTART=\"TCP\",\"%s\",%s",tcp_ip, portnum);
	printf("AP-TCP_CLIENT: %s\r\n",pt);
	esp8266_at_cmd_send(pt,"CONNECTED",300);
	
	esp8266_at_cmd_send("AT+CIPMODE=1","OK",50);
	
	esp8266_at_cmd_send("AT+CIPSEND","OK",100);

	if(pt != RT_NULL){
		rt_free((void *)pt);
	}
}

/************************  AP-UDP配置  *******************************/
void esp8266_ap_udp(char *SSID , char *PWD , uint8_t CH , uint8_t ECN , char * tcp_ip , char *portnum)
{
	char *pt = (char *)rt_malloc(50);
	
	esp8266_at_cmd_send("AT+CWMODE=2","OK",50);
	
	esp8266_at_cmd_send("AT+RST","OK",200);
	rt_thread_mdelay(2000);
	
	sprintf(pt,"AT+CWSAP=\"%s\",\"%s\",%d,%d",SSID, PWD, CH, ECN);
	printf("AP-UDP: %s\r\n",pt);
	esp8266_at_cmd_send(pt,"OK",200);
	
	esp8266_at_cmd_send("AT+CIPMUX=0","OK",100);
	
	sprintf(pt,"AT+CIPSTART=\"UDP\",\"%s\",%s",tcp_ip, portnum);
	printf("AP-UDP: %s\r\n",pt);
	esp8266_at_cmd_send(pt,"CONNECTED",300);	
	
	if(pt != RT_NULL){
		rt_free((void *)pt);
	}
}


/************************  STA-TCP-SERVER配置  *******************************/
void esp8266_sta_tcp_server(char *SSID , char *PWD , char *portnum)
{
	char *pt = (char *)rt_malloc(50);
	
	esp8266_at_cmd_send("AT+CWMODE=1","OK",50);
	
	esp8266_at_cmd_send("AT+RST","OK",200);
	rt_thread_mdelay(2000);
	
	sprintf(pt,"AT+CWJAP=\"%s\",\"%s\"",SSID, PWD);
	printf("STA-TCP_SERVER: %s\r\n",pt);
	esp8266_at_cmd_send(pt,"CONNECTED",500);
	rt_thread_mdelay(3000);
	
	esp8266_at_cmd_send("AT+CIPMUX=1","OK",100);
	rt_thread_mdelay(50);
	
	sprintf(pt,"AT+CIPSERVER=1,%s", portnum);
	printf("STA-TCP_SERVER: %s\r\n",pt);
	esp8266_at_cmd_send(pt,"OK",100);
	
	if(pt != RT_NULL){
		rt_free((void *)pt);
	}
}

/************************  STA-TCP-CLIENT配置  *******************************/
void esp8266_sta_tcp_client(char *SSID , char *PWD , char * tcp_ip , char *portnum)
{
	char *pt = (char *)rt_malloc(50);
	
	esp8266_at_cmd_send("AT+CWMODE=1","OK",50);
	
	esp8266_at_cmd_send("AT+RST","OK",200);
	rt_thread_mdelay(2000);
	
	sprintf(pt,"AT+CWJAP=\"%s\",\"%s\"",SSID, PWD);
	printf("STA-TCP_CLIENT: %s\r\n",pt);
	esp8266_at_cmd_send(pt,"CONNECTED",500);
	rt_thread_mdelay(100);
	
	esp8266_at_cmd_send("AT+CIPMUX=0","OK",100);

	sprintf(pt,"AT+CIPSTART=\"TCP\",\"%s\",%s",tcp_ip ,portnum);
	printf("STA-TCP_CLIENT: %s\r\n",pt);
	esp8266_at_cmd_send(pt,"OK",200);

	
	esp8266_at_cmd_send("AT+CIPMODE=1","OK",100);
	
	esp8266_at_cmd_send("AT+CIPSEND","OK",100);
	
	if(pt != RT_NULL){
		rt_free((void *)pt);
	}
}

/************************  STA-UDP配置  *******************************/
void esp8266_sta_udp(char *SSID , char *PWD , char * tcp_ip , char *portnum)
{
	char *pt = (char *)rt_malloc(50);
	
	esp8266_at_cmd_send("AT+CWMODE=1","OK",50);
	
	esp8266_at_cmd_send("AT+RST","OK",200);
	rt_thread_mdelay(2000);
	
	sprintf(pt,"AT+CWJAP=\"%s\",\"%s\"",SSID, PWD);
	printf("STA-UDP: %s\r\n",pt);
	esp8266_at_cmd_send(pt,"CONNECTED",500);
	rt_thread_mdelay(500);
	
	esp8266_at_cmd_send("AT+CIPMUX=0","OK",100);
	
	sprintf(pt,"AT+CIPSTART=\"UDP\",\"%s\",%s",tcp_ip ,portnum);
	printf("STA-TCP_CLIENT: %s\r\n",pt);
	esp8266_at_cmd_send(pt,"OK",200);
	
	if(pt != RT_NULL){
		rt_free((void *)pt);
	}
}

/************************  服务端模式数据发送  *******************************/
uint8_t esp8266_server_data_send(char *data_str)
{
	char *pt = (char *)rt_malloc(strlen(data_str));
	uint8_t i = 2 ;
	sprintf(pt,"AT+CIPSEND=0,%d",(uint8_t)strlen(data_str)); 
	while(i--){
		if(esp8266_at_cmd_send(pt,"OK",100)==0)
			break;
		else
			printf("esp8266_server_data_send fail...\r\n");
		
		if(i==0)
			return 1;
	} 
	esp8266_data_send(data_str);
	
	if(pt != RT_NULL){
		rt_free((void *)pt);
	}
	return 0;		
}

/************************  客户端模式数据发送  *******************************/
void esp8266_client_data_send(char *data_str)
{
	printf("esp8266_client_data_send:\r\n");
	esp8266_data_send(data_str);	
		
}

/************************  UDP模式数据发送  *******************************/
uint8_t esp8266_UDP_data_send(char *data_str)
{
	char *pt = (char *)rt_malloc(strlen(data_str));
	uint8_t i = 2 ;
	
	sprintf(pt,"AT+CIPSEND=%d",(uint8_t)strlen(data_str)); 
	while(i--){
		if(esp8266_at_cmd_send(pt,"OK",100)==0)
			break;
		else
			printf("esp8266_UDP_data_send fail...\r\n");
		
		if(i==0)
			return 1;
	} 
	
	esp8266_data_send(data_str);
	
	if(pt != RT_NULL){
		rt_free((void *)pt);
	}
	
	return 0;			
}

