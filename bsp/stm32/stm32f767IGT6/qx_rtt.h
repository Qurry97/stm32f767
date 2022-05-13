
#ifndef  _QX_RTT_H_
#define  _QX_RTT_H_

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include "stdio.h"
#include "stdarg.h"	
#include "string.h"

#include "Drivers/HARDWARE/ESP8266/esp8266.h"
#include "Drivers/HARDWARE/TIMER/timer.h"
#include "Drivers/HARDWARE/UART/uart.h"

extern int uart_tx_thread_init(void);
extern int uart_rx_thread_init(void);
extern int esp8266_test_thread_init(void);

#endif

