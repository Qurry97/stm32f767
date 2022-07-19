#ifndef _TIMER_H
#define _TIMER_H

#include "qx_rtt.h"

extern TIM_HandleTypeDef TIM2_Handler; 
extern TIM_HandleTypeDef TIM5_Handler; 

void TIM2_Init(void);
void TIM5_Init(void);
void TIM_Start(TIM_HandleTypeDef *htim , uint16_t arr);
void TIM_Stop(TIM_HandleTypeDef *htim);


#endif

