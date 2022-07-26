/************************************************************
    Author:  Qurry
    Time:    2022/7/19
    Fution:  Stm32f7 Auart TX (IO模拟串口)逻辑处理线程   
 ************************************************************/

#include "qx_rtt.h"

static void auart_tx_thread(void *p)
{
    uart1_init(19200);     //printf输出串口
	Auart_Init(AUART_BOUND);
	while(1){
		Auart_Send_Data("Qurry hello!");
		DEBUG("auart_tx_thread :%s\r\n",auart_tx.tx_data_buffer);
        rt_thread_mdelay(3000);
	}
}


int auart_tx_thread_init(void)
{
	rt_thread_t thread = RT_NULL;
	thread = rt_thread_create("auart_txthr", auart_tx_thread, RT_NULL, 10*1024, 5, 10); 
    if(thread == RT_NULL)
    {
        return RT_ERROR;
    }
    rt_thread_startup(thread);
	return RT_EOK;
}
