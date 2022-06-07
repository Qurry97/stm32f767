
/************************************************************
    Author:  Qurry
    Fution:  uart1-printf     uart3-esp8266
    Time:    4.18.2022
 ************************************************************/

#include "qx_rtt.h"



static void uart_tx_thread(void *p)
{
	uart1_init(115200);
	usart3_init(4800);
	rt_thread_mdelay(500);
	esp8266_sta_tcp_client("OnePlus9R","13582467","192.168.0.109","8086");

	while(1){
		printf("hello!\r\n");
		rt_thread_mdelay(3000);
	}

}




int uart_tx_thread_init(void)
{
	rt_thread_t thread = RT_NULL;
	thread = rt_thread_create("uart_tx_thr", uart_tx_thread, RT_NULL, 10*1024, 5, 10); 
    if(thread == RT_NULL)
    {
        return RT_ERROR;
    }
    rt_thread_startup(thread);
	return RT_EOK;
}










