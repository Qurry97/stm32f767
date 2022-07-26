/************************************************************
    Author:  Qurry
    Time:    2022/7/19
    Fution:  Stm32f7 Auart RX (IO模拟串口)逻辑处理线程   
 ************************************************************/

#include "qx_rtt.h"


rt_sem_t auart_rx_sem = RT_NULL;

void read_rx_data()
{
	DEBUG("rx:%s ,%d\r\n",auart_rx.rx_data_buffer,RX_IO_Read);
	memset(&auart_rx , 0 , sizeof(auart_rx));
	HAL_NVIC_EnableIRQ(EXTI3_IRQn);
	auart_rx.rx_state = AUART_RX_IDLE;
}

static void auart_rx_thread(void *p)
{
    auart_rx_sem = rt_sem_create("rx_sem", 0, RT_IPC_FLAG_PRIO);
    if(auart_rx_sem == RT_NULL){
        DEBUG("auart_rx_sem create failed !!!\r\n");
    }

	while(1){
        if(rt_sem_take(auart_rx_sem, RT_WAITING_FOREVER) != RT_EOK){
			rt_thread_mdelay(10);
            continue;
        }
		read_rx_data();
	}
}


int auart_rx_thread_init(void)
{
	rt_thread_t thread = RT_NULL;
	thread = rt_thread_create("auart_rxthr", auart_rx_thread, RT_NULL, 10*1024, 5, 10); 
    if(thread == RT_NULL)
    {
        return RT_ERROR;
    }
    rt_thread_startup(thread);
	return RT_EOK;
}
