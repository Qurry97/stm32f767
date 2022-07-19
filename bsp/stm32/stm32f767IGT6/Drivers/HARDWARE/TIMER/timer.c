/***********************************************
 * Autor :Qurry
 * Time  :2022/7/15
 * Fuction :定时器实现模拟串口收发线一位数据时间延迟
 * *********************************************/

#include "timer.h"


/***********************************************
 * Timer_2 :TX , 挂载在APB1上，时钟为108Mhz；
 * Timer_5 :RX , 挂载在APB1上，时钟为108Mhz；
 * 
 * ********************************************/

TIM_HandleTypeDef TIM2_Handler; 
TIM_HandleTypeDef TIM5_Handler; 


//arr us中断触发一次
void TIM2_Init(uint16_t arr)
{
	TIM2_Handler.Instance=TIM2;                         
    TIM2_Handler.Init.Prescaler=10800;                    
    TIM2_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    
    TIM2_Handler.Init.Period=arr;                       
    TIM2_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_Base_Init(&TIM2_Handler);						 
}

void TIM5_Init(uint16_t arr)
{
	TIM5_Handler.Instance=TIM5;                         
    TIM5_Handler.Init.Prescaler=108;                    
    TIM5_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    
    TIM5_Handler.Init.Period=arr;                       
    TIM5_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_Base_Init(&TIM5_Handler);						 
}

void TIM_Start(TIM_HandleTypeDef *htim , uint16_t arr)
{
    //printf("tick = %ld\r\n",rt_tick_get());
    __HAL_TIM_SET_AUTORELOAD(htim , arr);
    HAL_TIM_Base_Start_IT(htim);
}

void TIM_Stop(TIM_HandleTypeDef *htim)
{
    HAL_TIM_Base_Stop_IT(htim);	
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM2){
		__HAL_RCC_TIM2_CLK_ENABLE();            //使能TIM2时钟
		HAL_NVIC_SetPriority(TIM2_IRQn,2,0);    //设置中断优先级，抢占优先级2，子优先级0
		HAL_NVIC_EnableIRQ(TIM2_IRQn);          //开启ITM2中断   
	}

    else if(htim->Instance==TIM5){
		__HAL_RCC_TIM5_CLK_ENABLE();  
		HAL_NVIC_SetPriority(TIM5_IRQn,2,1);
		HAL_NVIC_EnableIRQ(TIM5_IRQn);
    }
}

void TIM2_IRQHandler(void)
{ 	    		    
	HAL_TIM_IRQHandler(&TIM2_Handler);					 
} 

void TIM5_IRQHandler(void)
{ 	    		    
    HAL_TIM_IRQHandler(&TIM5_Handler);
} 

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    //printf("tick = %ld\r\n",rt_tick_get());
    //TIM_Stop(&TIM2_Handler);
    if(htim==(&TIM2_Handler)){
        if(auart_tx.tx_state == AUART_TX_SEND)
            Auart_Send_Data_Handler();
    }

    else if(htim==(&TIM5_Handler)){
        if(auart_rx.rx_state == AUART_RX_READ)
            Auart_Read_Data_Handler();
    }
}

