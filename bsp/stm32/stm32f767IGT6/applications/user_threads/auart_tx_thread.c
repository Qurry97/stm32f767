/************************************************************
    Author:  Qurry
    Time:    2022/7/19
    Fution:  Stm32f7 Auart TX (IO模拟串口)逻辑处理线程   
 ************************************************************/

#include "qx_rtt.h"

static void key_thread(void *p)
{
    uart1_init(115200);     //printf输出串口
	Auart_Init(9600);
    rt_thread_mdelay(50);

	while(1){
        Auart_Send_Data("Hello Qurry! 2022.");
        rt_thread_mdelay(3000);
	}
}


int auart_thread_init(void)
{
	rt_thread_t thread = RT_NULL;
	thread = rt_thread_create("auart_thr", auart_thread, RT_NULL, 10*1024, 5, 10); 
    if(thread == RT_NULL)
    {
        return RT_ERROR;
    }
    rt_thread_startup(thread);
	return RT_EOK;
}
