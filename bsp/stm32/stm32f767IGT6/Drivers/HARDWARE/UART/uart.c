
#include "../qx_rtt.h"


#if 1
#pragma import(__use_no_semihosting)             
                
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
   
void _sys_exit(int x) 
{ 
	x = x; 
} 

int fputc(int ch, FILE *f)
{ 	
	while((USART1->ISR&0X40)==0);  
	USART1->TDR=(uint8_t)ch;      
	return ch;
}

#endif


/************************************************************
    				Fution:  uart1配置
 ************************************************************/

uint8_t USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
uint16_t USART_RX_STA=0;       //接收状态标记	
uint8_t aRxBuffer[RXBUFFERSIZE];//HAL库使用的串口接收缓冲
UART_HandleTypeDef UART1_Handler; //UART1句柄

void uart1_init(uint32_t bound)
{	
	//UART 初始化设置
	UART1_Handler.Instance=USART1;					    //USART1
	UART1_Handler.Init.BaudRate=bound;				    //波特率
	UART1_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //字长为8位数据格式
	UART1_Handler.Init.StopBits=UART_STOPBITS_1;	    //一个停止位
	UART1_Handler.Init.Parity=UART_PARITY_NONE;		    //无奇偶校验位
	UART1_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //无硬件流控
	UART1_Handler.Init.Mode=UART_MODE_TX_RX;		    //收发模式
	HAL_UART_Init(&UART1_Handler);					    //HAL_UART_Init()会使能UART1
	
	HAL_UART_Receive_IT(&UART1_Handler, (uint8_t *)aRxBuffer, RXBUFFERSIZE);//该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
  
}



/************************************************************
    				Fution:  uart3配置
 ************************************************************/		

__align(8) uint8_t USART3_TX_BUF[USART3_MAX_SEND_LEN];  	
uint8_t USART3_RX_BUF[USART3_MAX_RECV_LEN];
uint16_t USART3_RX_STA=0;
UART_HandleTypeDef UART3_Handler;

void usart3_init(uint32_t bound)
{	
	UART3_Handler.Instance=USART3;					   
	UART3_Handler.Init.BaudRate=bound;				    
	UART3_Handler.Init.WordLength=UART_WORDLENGTH_8B;  
	UART3_Handler.Init.StopBits=UART_STOPBITS_1;	   
	UART3_Handler.Init.Parity=UART_PARITY_NONE;		   
	UART3_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;  
	UART3_Handler.Init.Mode=UART_MODE_TX_RX;		   
	HAL_UART_Init(&UART3_Handler);

}



void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	GPIO_InitTypeDef GPIO_Initure;
	
	if(huart->Instance==USART1)
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();			
		__HAL_RCC_USART1_CLK_ENABLE();			
	
		GPIO_Initure.Pin=GPIO_PIN_9;			
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		
		GPIO_Initure.Pull=GPIO_PULLUP;			
		GPIO_Initure.Speed=GPIO_SPEED_FAST;		
		GPIO_Initure.Alternate=GPIO_AF7_USART1;	
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   

		GPIO_Initure.Pin=GPIO_PIN_10;			
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   		
	}

	if(huart==(&UART3_Handler))
	{	
		__HAL_RCC_GPIOB_CLK_ENABLE();			//使能GPIOB时钟
		__HAL_RCC_USART3_CLK_ENABLE();			//使能USART3时钟
	
		GPIO_Initure.Pin=GPIO_PIN_10;			//PB10
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//复用推挽输出
		GPIO_Initure.Pull=GPIO_PULLUP;			//上拉
		GPIO_Initure.Speed=GPIO_SPEED_FAST;		//高速
		GPIO_Initure.Alternate=GPIO_AF7_USART3;	//复用为USART3
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);	   	//初始化PB10

		GPIO_Initure.Pin=GPIO_PIN_11;			//PB11
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);	   	//初始化PB11
		
		TIM7_Int_Init(1000-1,10800-1);		//300ms中断
		TIM7->CR1&=~(1<<0);        //关闭定时器7	
		
		__HAL_UART_ENABLE_IT(huart,UART_IT_RXNE);		//开启接收中断
		HAL_NVIC_EnableIRQ(USART3_IRQn);				//使能USART3中断
		HAL_NVIC_SetPriority(USART3_IRQn,2,3);			//抢占优先级2，子优先级3

		USART3_RX_STA=0;
	}

}


void USART3_Send(char* fmt, ...)
{
	uint16_t i,j; 
	va_list ap; 
	va_start(ap,fmt);
	vsprintf((char*)USART3_TX_BUF,fmt,ap);
	va_end(ap);
    i=strlen((const char*)USART3_TX_BUF);
	for(j=0;j<i;j++)
	{
		while((USART3->ISR&0X40)==0);			
		USART3->TDR=USART3_TX_BUF[j];  
	} 
	printf("uart3 tx data:%s\r\n",USART3_TX_BUF);
}


void USART3_IRQHandler(void)
{
	uint8_t res;
	if(__HAL_UART_GET_FLAG(&UART3_Handler,UART_IT_ERR) == RESET){
		__HAL_UART_CLEAR_OREFLAG(&UART3_Handler);
		printf("UART_FLAG_ORE\r\n");
	} 	
	if(__HAL_UART_GET_FLAG(&UART3_Handler,UART_FLAG_RXNE) != RESET)
	{	  
		 res=USART3->RDR;	
		if((USART3_RX_STA&(1<<15))==0)
		{ 
			if(USART3_RX_STA<USART3_MAX_RECV_LEN)
			{
				TIM7->CNT=0;         				//计数器清空
				if(USART3_RX_STA==0)
				{
					TIM7->CR1|=1<<0;     			//使能定时器7
					memset(USART3_RX_BUF, 0 , USART3_MAX_RECV_LEN);
					//printf("timer7 open!\r\n");
				}
				USART3_RX_BUF[USART3_RX_STA++]=res;		 
			}
			else 
			{
				USART3_RX_STA|=1<<15;
				printf("uart3 rx error!\r\n");
			} 
		}
	}  	
  	
}   


void USART1_IRQHandler(void)                	
{ 
	uint8_t Res;

	if((__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_RXNE)!=RESET))
	{
        HAL_UART_Receive(&UART1_Handler,&Res,1,1000); 
		if((USART_RX_STA&0x8000)==0)
		{
			if(USART_RX_STA&0x4000)
			{
				if(Res!=0x0a)
					USART_RX_STA=0;
				else 
					USART_RX_STA|=0x8000;
			}
			else
			{	
				if(Res==0x0d)
					USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))
						USART_RX_STA=0;  
				}		 
			}
		}   		 
	}
	HAL_UART_IRQHandler(&UART1_Handler);	
} 

#if 0

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART1)//如果是串口1
	{
		if((USART_RX_STA&0x8000)==0)//接收未完成
		{
			if(USART_RX_STA&0x4000)//接收到了0x0d
			{
				if(aRxBuffer[0]!=0x0a)
					USART_RX_STA=0;//接收错误,重新开始
				else 
					USART_RX_STA|=0x8000;	//接收完成了 
			}
			else //还没收到0X0D
			{	
				if(aRxBuffer[0]==0x0d)
					USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=aRxBuffer[0] ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))
						USART_RX_STA=0;//接收数据错误,重新开始接收	  
				}		 
			}
		}

	}
}
 

void USART1_IRQHandler(void)                	
{ 
	uint32_t timeout=0;
    uint32_t maxDelay=0x1FFFF;
	
	HAL_UART_IRQHandler(&UART1_Handler);
	
	timeout=0;
    while (HAL_UART_GetState(&UART1_Handler)!=HAL_UART_STATE_READY)//等待就绪
	{
        timeout++;////超时处理
        if(timeout>maxDelay) break;		
	}
     
	timeout=0;
	while(HAL_UART_Receive_IT(&UART1_Handler,(uint8_t *)aRxBuffer, RXBUFFERSIZE)!=HAL_OK)//一次处理完成之后，重新开启中断并设置RxXferCount为1
	{
        timeout++; //超时处理
        if(timeout>maxDelay) break;	
	}
}


#endif

