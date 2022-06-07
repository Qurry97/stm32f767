
#include "qx_rtt.h"

TIM_HandleTypeDef TIM7_Handler;      

void TIM7_Int_Init(uint16_t arr,uint16_t psc)
{
	TIM7_Handler.Instance=TIM7;                         
    TIM7_Handler.Init.Prescaler=psc;                    
    TIM7_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    
    TIM7_Handler.Init.Period=arr;                       
    TIM7_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_Base_Init(&TIM7_Handler);
    __HAL_TIM_ENABLE_IT(&TIM7_Handler, TIM_IT_UPDATE);
    //HAL_TIM_Base_Start_IT(&TIM7_Handler);								 
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM7)
	{
		__HAL_RCC_TIM7_CLK_ENABLE();            //使能TIM7时钟
		HAL_NVIC_SetPriority(TIM7_IRQn,0,1);    //设置中断优先级，抢占优先级0，子优先级1
		HAL_NVIC_EnableIRQ(TIM7_IRQn);          //开启ITM7中断   
	}
}
extern rt_tick_t timer_tick;
void TIM7_IRQHandler(void)
{ 	    		    
		if(USART3_RX_STA !=0){
			USART3_RX_STA|=1<<15;	//标记接收完成
			printf("uart3 rx data:%s\r\n",USART3_RX_BUF);
			esp8266_data_check();
		}

		__HAL_TIM_CLEAR_FLAG(&TIM7_Handler,TIM_EventSource_Update );       //清除TIM7更新中断标志  
		TIM7->CR1&=~(1<<0);     			//关闭定时器7     				
		printf("timer7 close!\r\n");							 
}  

