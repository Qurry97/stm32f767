/************************************************************
    Author:  Qurry
    Fution:  MQTT测试线程
    Time:    5.19.2022
 ************************************************************/

#include "qx_rtt.h"


/************************************************************
 * Fction:MQTT接受数据处理函数
 * *********************************************************/
void usart3_rx_data_handle()
{
	if(USART3_RX_STA&0x8000){
		if(strstr("+MQTTSUBRECV",(char *)USART3_RX_BUF))      //接收到服务器发布的数据
			USART3_RX_STA = 0;
			printf("111\r\n");
	}
}

static void mqtt_thread(void *p)
{
	uart1_init(115200);
	usart3_init(4800);
	rt_thread_mdelay(500);
    mqtt_init(WIFI_ID,WIFI_PW);
	while(1){
		printf("hello\r\n");
		rt_thread_mdelay(5000);
	}

}




int mqtt_thread_init(void)
{
	rt_thread_t thread = RT_NULL;
	thread = rt_thread_create("mqtt_thr", mqtt_thread, RT_NULL, 20*1024, 5, 10); 
    if(thread == RT_NULL)
    {
        return RT_ERROR;
    }
    rt_thread_startup(thread);
	return RT_EOK;
}

