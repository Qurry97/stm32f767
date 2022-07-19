/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-06     SummerGift   first version
 */



#include "qx_rtt.h"

int main(void)
{
    uart1_init(115200);
    TIM2_Init(104);
    Auart_Init(9600);
	rt_thread_delay(500);
	Auart_Send_Data("Hello World.\r\n");
    while(1){
        //Auart_Send_Data("Hello World.\r\n");
		printf("111\r\n");
        rt_thread_delay(3000);
    }


}
