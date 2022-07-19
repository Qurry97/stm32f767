/***********************************************
 * Autor :Qurry
 * Time  :2022/7/14
 * Fuction :IO模拟串口功能实现
 * *********************************************/

#include "auart.h"

/**********************************************
 * PE2:TX
 * PE3:RX
 * ********************************************/

AUART_TX  auart_tx;
AUART_RX  auart_rx;


void TX_IO_Config(void)
{
    GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_GPIOE_CLK_ENABLE();  
    GPIO_Initure.Pin=GPIO_PIN_2;                //PE2
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;      //输出推挽模式
    GPIO_Initure.Pull=GPIO_PULLUP;              //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;
    HAL_GPIO_Init(GPIOE,&GPIO_Initure);

    auart_tx.tx_state = AUART_TX_IDLE;
}

void RX_IO_Config(void)
{
    GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_GPIOE_CLK_ENABLE();  
    GPIO_Initure.Pin=GPIO_PIN_3;                //PE3
    GPIO_Initure.Mode=GPIO_MODE_IT_FALLING;     //输入模式下降沿触发
    GPIO_Initure.Pull=GPIO_PULLUP;              //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;
    HAL_GPIO_Init(GPIOE,&GPIO_Initure); 

    HAL_NVIC_SetPriority(EXTI3_IRQn,2,2);  
    HAL_NVIC_EnableIRQ(EXTI3_IRQn);

    auart_rx.rx_state = AUART_RX_IDLE; 
}

/*** 计算传输1bit数据所需时间(us) ***/
void Auart_One_Bit_Time(uint32_t bound)
{
    auart_tx.bound = bound ;
    auart_tx.one_bit_time = (1E+6) / bound ;
}

uint16_t get_auart_One_Bit_Time(void)
{
    return auart_tx.one_bit_time ;
}

void data_init(AUART_TX *tx_data , AUART_RX *rx_data)
{
    memset(tx_data , 0 , sizeof(AUART_TX));
    memset(rx_data , 0 , sizeof(AUART_RX));
}

void Auart_Init(uint32_t Bound)
{
    TX_IO_Config();
    RX_IO_Config();
    data_init(&auart_tx , &auart_rx);
    Auart_One_Bit_Time(Bound);    
}

/***  读取发送数据当前字节的位值  ***/
uint8_t Auart_Tx_Data_Read_Bit(uint16_t Cur , uint8_t Bit)
{
    uint8_t temp;
    temp = (auart_tx.tx_data_buffer[Cur]>>Bit) & 0x01;
    return temp;
}

/***   auart发送开启   ***/
void Auart_Send_Open(void)
{
    TIM_Start(&TIM2_Handler , auart_tx.one_bit_time);
    auart_tx.tx_state = AUART_TX_SEND;
}

/***   auart开始位   ***/
void Auart_Send_Start(void)
{
    if(TX_IO_Read != GPIO_HIGH){
        TX_IO_Write(GPIO_HIGH);
        rt_hw_us_delay(5);
    }
    TX_IO_Write(GPIO_LOW);
}

/***   auart停止位   ***/
void Auart_Send_Stop(void)
{
    TX_IO_Write(GPIO_HIGH);
}

/***   auart空闲   ***/
void Auart_Send_Idle(void)
{
    TX_IO_Write(GPIO_HIGH);
    TIM_Stop(&TIM2_Handler);
    auart_tx.tx_state =AUART_TX_IDLE;
}

/***   auart发送数据   ***/
void Auart_Send_Data(uint8_t *str)
{

    //auart_tx.tx_data_len = sizeof(str);
    auart_tx.data_cur = 0;
    auart_tx.cur_bit  = 0;
    memset(&auart_tx.tx_data_buffer , 0 , sizeof(auart_tx.tx_data_buffer));
	for(int i = 0 ; *str!='\0' ; i++){
        auart_tx.tx_data_buffer[i] = *str;
        str++ ;
        auart_tx.tx_data_len++ ;
    }
	printf("%s,%d\r\n",auart_tx.tx_data_buffer,auart_tx.tx_data_len);
    Auart_Send_Open();
}

/***   auart数据发送处理函数   ***/
void Auart_Send_Data_Handler(void)
{
    if(auart_tx.data_cur < auart_tx.tx_data_len){
        if(auart_tx.cur_bit == 0){
            Auart_Send_Start();
            return ;
        }
        if(auart_tx.cur_bit < 8){
            if(Auart_Tx_Data_Read_Bit(auart_tx.data_cur , auart_tx.cur_bit) == 0){
				TX_IO_Write(GPIO_LOW);
			}
            else{
                TX_IO_Write(GPIO_HIGH);
            }
            auart_tx.cur_bit++;
        }
        else{
            auart_tx.cur_bit = 0;
            auart_tx.data_cur++;
            Auart_Send_Stop();
        }
    }
    else{
        Auart_Send_Idle();
    }
}

/***   auart数据接收处理函数   ***/
void Auart_Read_Data_Handler(void)
{
    
}



