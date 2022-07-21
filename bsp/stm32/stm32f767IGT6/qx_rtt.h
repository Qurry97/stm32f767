
#ifndef  _QX_RTT_H_
#define  _QX_RTT_H_

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include "stdio.h"
#include "stdarg.h"	
#include "string.h"
#include "stdlib.h"
#include "time.h"

#include "Drivers/HARDWARE/TIMER/timer.h"
#include "Drivers/HARDWARE/AUART/auart.h"

int auart_tx_thread_init(void);
int auart_rx_thread_init(void);
void uart1_init(uint32_t bound);

extern rt_sem_t auart_rx_sem;

#endif

