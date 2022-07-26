#ifndef _AUART_H
#define _AUART_H

#include "qx_rtt.h"

#define DBG_EN

#ifdef  DBG_EN
    #define DEBUG   printf
#else
    #define DEBUG
#endif



#define TX_DATA_MAX   192
#define RX_DATA_MAX   192

#define TX_IO_Read      HAL_GPIO_ReadPin(GPIOE , GPIO_PIN_2)
#define TX_IO_Write(X)  HAL_GPIO_WritePin(GPIOE , GPIO_PIN_2 , X)
#define RX_IO_Read      HAL_GPIO_ReadPin(GPIOE , GPIO_PIN_3)

typedef enum _PIN_{
    GPIO_LOW = 0,
    GPIO_HIGH,
}PIN;

typedef enum _TX_STATE_{
    AUART_TX_IDLE = 0,      //发送空闲
    AUART_TX_SEND,          //数据发送
}TX_STATE;

typedef enum _RX_STATE_{
    AUART_RX_IDLE = 0,     //读取空闲
    AUART_RX_READ,         //数据读取
}RX_STATE;

typedef struct _AUART_TX_{
    uint8_t  tx_data_buffer[TX_DATA_MAX];   //发送数据缓冲区
    uint8_t  cur_bit;                       //当前数据字节位
    volatile uint8_t  start_flag;           //启动位标志位
    TX_STATE tx_state;                      //发送状态
    uint32_t bound;                         //波特率
    uint16_t one_bit_time;                  //发送1bit时间
    uint16_t data_cur;                      //当前数据
    uint16_t tx_data_len;                   //发送数据长度
}AUART_TX;

typedef struct _AUART_RX_{
    uint8_t  rx_data_buffer[RX_DATA_MAX];   //读取数据缓冲区
    volatile uint8_t  cur_bit;              //当前数据字节位
    volatile uint16_t data_cur;             //接受数据当前位置   
    RX_STATE rx_state;                      //接受状态   
}AUART_RX;



extern AUART_TX  auart_tx;
extern AUART_RX  auart_rx;

void TX_IO_Config(void);
void RX_IO_Config(void);
void Auart_One_Bit_Time(uint32_t bound);
uint16_t get_auart_One_Bit_Time(void);
void data_init(AUART_TX *tx_data , AUART_RX *rx_data);
void Auart_Init(uint32_t Bound);
uint8_t Auart_Tx_Data_Read_Bit(uint16_t Cur , uint8_t Bit);
void Auart_Rx_Data_Bit(uint16_t Cur , uint8_t Bit ,uint8_t Bit_Data);
void Auart_Send_Open(void);
void Auart_Send_Start(void);
void Auart_Send_Stop(void);
void Auart_Send_Idle(void);
void Auart_Send_Data(uint8_t *str);
void Auart_Send_Data_Handler(void);
void Auart_Read_Data_Handler(void);

#endif

