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
    TIM2_Init();
    TIM5_Init();
    auart_thread_init();

}
