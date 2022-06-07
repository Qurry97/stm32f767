/************************************************************
    Author:  Qurry
    Fution:  stm32f7 ATK-esp8266测试线程
				更多例程说明请查看esp8266_test.TXTX文档
    Time:    5.12.2022
 ************************************************************/

#include "qx_rtt.h"

enum ESP_Mode Esp_mode = AP_TCP_CLIENT_MODE;   //更改枚举值切换WIFI传输模式

void esp8266_mode_select(uint8_t mode)
{
    switch (mode)
    {
    case AP_TCP_SERVER_MODE:
        esp8266_ap_tcp_server("ATK8266","43218765", 1 , 4 ,"8086");
        break;

    case AP_TCP_CLIENT_MODE:
        esp8266_ap_tcp_client("ATK8266","43218765", 1 , 4 ,"192.168.4.2" ,"8086");
        break;

    case AP_UDP_MODE:
        esp8266_ap_udp("ATK8266","43218765", 1 , 4 ,"192.168.4.2" ,"8086");
        break;

    case STA_TCP_SERVER_MODE:
        esp8266_sta_tcp_server("OnePlus9R","13582467","8082");
        break;

    case STA_TCP_CLIENT_MODE:
        esp8266_sta_tcp_client("OnePlus9R","13582467","192.168.0.107","8082");
        break;

    case STA_UDP_MODE:
        esp8266_sta_udp("OnePlus9R","13582467","192.168.0.107","8082");
        break;
    
    default:
        break;
    }
}

static void esp8266_test_thread(void *p)
{
	uart1_init(115200);
	usart3_init(4800);
	rt_thread_mdelay(500);
    esp8266_mode_select(Esp_mode);
	while(1){
        printf("hello\r\n");
		rt_thread_mdelay(2000);
	}

}




int esp8266_test_thread_init(void)
{
	rt_thread_t thread = RT_NULL;
	thread = rt_thread_create("esp8266_thr", esp8266_test_thread, RT_NULL, 10*1024, 5, 10); 
    if(thread == RT_NULL)
    {
        return RT_ERROR;
    }
    rt_thread_startup(thread);
	return RT_EOK;
}

