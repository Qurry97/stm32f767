#include "ctiic.h"
#include "delay.h"	 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//���ݴ�����-IIC ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/12/28
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
//********************************************************************************
//��
////////////////////////////////////////////////////////////////////////////////// 	

//����I2C�ٶȵ���ʱ
void CT_Delay(void)
{
	delay_us(2);
} 

//���ݴ���оƬIIC�ӿڳ�ʼ��
void CT_IIC_Init(void)
{					     
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOH_CLK_ENABLE();			//����GPIOHʱ��
	__HAL_RCC_GPIOI_CLK_ENABLE();			//����GPIOIʱ��
    
    GPIO_Initure.Pin=GPIO_PIN_6;            //PH6
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_OD;  //��©���
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(GPIOH,&GPIO_Initure);     //��ʼ��
	
    GPIO_Initure.Pin=GPIO_PIN_3;            //PI3
    HAL_GPIO_Init(GPIOI,&GPIO_Initure);     //��ʼ��
}

//����IIC��ʼ�ź�
void CT_IIC_Start(void)
{
	CT_IIC_SDA(1);	  	  
	CT_IIC_SCL(1);
    CT_Delay();
 	CT_IIC_SDA(0);//START:when CLK is high,DATA change form high to low 
    CT_Delay();
	CT_IIC_SCL(0);//ǯסI2C���ߣ�׼�����ͻ�������� 
    CT_Delay();
}	 

//����IICֹͣ�ź�
void CT_IIC_Stop(void)
{
	CT_IIC_SDA(0);
    CT_Delay();
	CT_IIC_SCL(1);
	CT_IIC_SDA(1);//STOP:when CLK is high DATA change form low to high
	CT_Delay();
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 CT_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
    u8 rack = 0;

	CT_IIC_SDA(1);	
	CT_Delay();
	CT_IIC_SCL(1);
	CT_Delay();
    
	while(CT_READ_SDA)
	{
		ucErrTime++;
        
		if(ucErrTime>250)
		{
			CT_IIC_Stop();
            rack = 1;
            break;
		} 
	}
    
	CT_IIC_SCL(0);//ʱ�����0 	
    CT_Delay(); 
    return rack;
} 

//����ACKӦ��
void CT_IIC_Ack(void)
{
	CT_IIC_SDA(0);
	CT_Delay();
	CT_IIC_SCL(1);
	CT_Delay();
	CT_IIC_SCL(0);
	CT_Delay();
	CT_IIC_SDA(1);
	CT_Delay();
}
//������ACKӦ��		    
void CT_IIC_NAck(void)
{
	CT_IIC_SDA(1);
	CT_Delay();
	CT_IIC_SCL(1);
	CT_Delay();
	CT_IIC_SCL(0);
    CT_Delay();
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void CT_IIC_Send_Byte(u8 txd)
{                        
    u8 t;   

	for(t=0;t<8;t++)
    {              
        CT_IIC_SDA((txd&0x80)>>7); 
        CT_Delay();
		CT_IIC_SCL(1); 
		CT_Delay();
		CT_IIC_SCL(0);	
        txd<<=1; 	      
    }
    CT_IIC_SDA(1);
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 CT_IIC_Read_Byte(unsigned char ack)
{
	u8 i,receive=0;

	for(i=0;i<8;i++ )
	{ 
        receive<<=1;
		CT_IIC_SCL(1); 	    	   
		CT_Delay();
        
		if(CT_READ_SDA)receive++;   
		CT_IIC_SCL(0);	
		CT_Delay();
        
	}	  				 
	if (!ack)
        CT_IIC_NAck();//����nACK
	else 
        CT_IIC_Ack(); //����ACK   
 	return receive;
}




























