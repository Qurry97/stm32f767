/************************************************************
    Author:  Qurry
    Fution:  stm32f7 key逻辑处理线程
    Time:    4.25.2022
 ************************************************************/

#include "qx_rtt.h"

#define KEY_SEM
#define KEY_UP    GPIO_PIN_0
#define KEY_0     GPIO_PIN_3
#define KEY_1     GPIO_PIN_2
#define LED_0(X)  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,X) 

rt_sem_t key_sem = RT_NULL;
uint16_t key_pin = 0 ;

void LED_Init()
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOB_CLK_ENABLE();
	
    GPIO_Initure.Pin=GPIO_PIN_0;
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;
    GPIO_Initure.Pull=GPIO_PULLUP;
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
	
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);
}

void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE(); 
    
    GPIO_Initure.Pin    = GPIO_PIN_0;             //PA0 - KEY_UP
    GPIO_Initure.Mode   = GPIO_MODE_INPUT|GPIO_MODE_IT_RISING;
    GPIO_Initure.Pull   = GPIO_PULLDOWN;
    GPIO_Initure.Speed  = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
    
    GPIO_Initure.Pin   = GPIO_PIN_2|GPIO_PIN_3;  //PH2-KEY1     PH3-KEY0    
	GPIO_Initure.Mode  = GPIO_MODE_INPUT|GPIO_MODE_IT_FALLING;
    GPIO_Initure.Pull  = GPIO_PULLUP;
    GPIO_Initure.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(GPIOH,&GPIO_Initure);

    HAL_NVIC_SetPriority(EXTI0_IRQn,2,0);
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);
    
    HAL_NVIC_SetPriority(EXTI2_IRQn,2,1);
    HAL_NVIC_EnableIRQ(EXTI2_IRQn);
   
    HAL_NVIC_SetPriority(EXTI3_IRQn,2,2);
    HAL_NVIC_EnableIRQ(EXTI3_IRQn);
}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    rt_sem_release(key_sem);
    key_pin = GPIO_Pin ;
    printf("HAL_GPIO_EXTI_Callback : keysem release.\r\n");
}

void EXTI0_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

void EXTI2_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
}

void EXTI3_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);
}


void key_handler()
{
#ifdef  ESP8266_THREAD
    if(key_pin == KEY_UP){
        esp8266_at_cmd_send("AT+CIPSEND","OK",20);
        LED_0(0);
    }
    else if(key_pin == KEY_0){
        if((Esp_mode == 0)||(Esp_mode == 3))
            esp8266_server_data_send("1.stm32f7-esp8266");
        else if((Esp_mode == 1)||(Esp_mode == 4))
            esp8266_client_data_send("2.stm32f7-esp8266");
        else
            esp8266_UDP_data_send("3.stm32f7-esp8266");
    }
    else if(key_pin == KEY_1){
        esp8266_quit_trans();
        LED_0(1);
    }
    key_pin = 0 ;

#endif

#ifdef  MQTT_THREAD
    if(key_pin == KEY_UP){
		if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0) == GPIO_PIN_SET){
			mqtt_connect_aliyun(HOST_URL,PORT,CLIENT_ID,USER_NAME,PASSWORD);
			LED_0(0);
		}
		else{
			mqtt_close_connect();
			LED_0(1);
		}
         
    }
    else if(key_pin == KEY_0){
		mqtt_publish_topic(PUB_TOPIC,"21.7");
    }
    else if(key_pin == KEY_1){
		mqtt_sub_topic(SUB_TOPIC);      
    }
    key_pin = 0 ;
#endif

}


static void key_thread(void *p)
{
    LED_Init(); 
    KEY_Init();

#ifdef KEY_SEM	
	key_sem = rt_sem_create("keysem", 0, RT_IPC_FLAG_PRIO);
	if(key_sem == RT_NULL)
		printf("key_sem creat failed!\r\n");
    else
        printf("key_sem creat ok!\r\n");
#endif

	while(1){
        while(rt_sem_trytake(key_sem) != RT_EOK){
            rt_thread_mdelay(10);
        }
        printf("key_thread : key_sem take.\r\n");
        key_handler();
	}

}


int key_thread_init(void)
{
	rt_thread_t thread = RT_NULL;
	thread = rt_thread_create("key_thr", key_thread, RT_NULL, 10*1024, 5, 10); 
    if(thread == RT_NULL)
    {
        return RT_ERROR;
    }
    rt_thread_startup(thread);
	return RT_EOK;
}




