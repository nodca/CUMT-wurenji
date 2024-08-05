#include "lanya.h"
#include "raspi.h"
#include "Ano_Scheduler.h"
#define RXBUFFERSIZE  3    //最大接收字节数
#define TXBUFFERSIZE  7     //最大发送字节数

uint8_t uart1_rxbuff;
uint8_t sendBuf[TXBUFFERSIZE];
uint8_t aaa=0;
u16 BlueRxBuffer[3]={0};
u8 x_high=0;
u8 x_low=0;
u8 y_high=0;
u8 y_low=0;
u8 ok_flag=1;
u8 huowu_num=0;

void Lanya_Receive_Data(uint8_t com_data)
{aaa=com_data;
		uint8_t i;
		static uint8_t BlueRxCounter=0;//计数
	  
		static uint8_t BlueRxState = 0;	


		if(BlueRxState==0&&com_data==0x12)  //0x12帧头
		{
			
			BlueRxState=2;
			BlueRxBuffer[BlueRxCounter++]=com_data;  
		}


		
		else if(BlueRxState==2)
		{
			 
			BlueRxBuffer[BlueRxCounter++]=com_data;
			if(BlueRxCounter>=3||com_data == 0x5B)    
			{
				BlueRxState=3;
				
		   ok_flag= BlueRxBuffer[1];
				
				      

			}
		}

		else if(BlueRxState==3)		//检测是否接受到结束标志
		{
				
			  if(BlueRxBuffer[BlueRxCounter-1] == 0x5B)    
				{
							
							//BlueRxFlag = 0;
							BlueRxState = 0;
							BlueRxCounter = 0;
						
				}
				else   //接收错误
				{
							BlueRxState  = 0;
							BlueRxCounter =0;
							for(i=0;i<3;i++)
							{
									BlueRxBuffer[i]=0x00;      //将存放数据数组清零
							}
				}
		} 

		else   //接收异常
		{
				BlueRxState = 0;
				BlueRxCounter = 0;
				for(i=0;i<3;i++)
				{
						BlueRxBuffer[i]=0x00;      //将存放数据数组清零
				}
		}
}


void lanya_send()
{
	static u8 stage=0;
	if(stage==0){
	  USART_SendData(USART3,0xAA);
		stage=1;
	}
	else if(stage==1){
	  USART_SendData(USART3,0xBB);
		stage=2;
	}

	else if(stage==2){
	  USART_SendData(USART3,ok_flag);
		stage=3;
	}
	else if(stage==3){
	  USART_SendData(USART3,led_flag);
		stage=4;
	}

	else if(stage==4){
	  USART_SendData(USART3,0xCC);
		stage=5;
	}
	else stage=0;
/*	if(stage==0)
	{
		USART_SendData(USART3,0xAA);
		stage=1;
		s16 num = base_link.pose_x_cm ;
		x_high=(num >> 8) & 0xFF;
		x_low=num & 0xFF;
		s16 ynum = base_link.pose_y_cm ;
		y_high=(ynum >> 8) & 0xFF;
		y_low=ynum & 0xFF;
	}
	else if(stage==1)
	{
		USART_SendData(USART3,0XBB);
		stage=2;
	}
	else if(stage==2)
	{

		USART_SendData(USART3,x_high);
		//USART_SendData(USART2,0x05);
		stage=3;
	}
	else if(stage==3)
	{
		USART_SendData(USART3,x_low);
		stage=4;
	}
	else if(stage==4)
	{

		USART_SendData(USART3,y_high);
		//USART_SendData(USART2,0x05);
		stage=5;
	}
	else if(stage==5)
	{
		USART_SendData(USART3,y_low);
		stage=6;
	}
	else if(stage==6)
	{
		USART_SendData(USART3,0xCC);
		stage=0;
	}
	else stage=0;*/
}

u8 fire_flag=0,f_err_x=0,f_err_y=0;
void Openmv_Receive_Data(uint8_t com_data)
{
		uint8_t i;
		static uint8_t RxCounter1=0;//计数
		static uint16_t RxBuffer1[9]={0};
		static uint8_t RxState = 0;	
		static uint8_t RxFlag1 = 0;

		if(RxState==0&&com_data==0x2C)  //0x2c帧头
		{
			
			RxState=1;
			RxBuffer1[RxCounter1++]=com_data;  
		}

		else if(RxState==1&&com_data==0x12)  //0x12帧头
		{
			RxState=2;
			RxBuffer1[RxCounter1++]=com_data;
		}
		
		else if(RxState==2)
		{
			 
			RxBuffer1[RxCounter1++]=com_data;
			if(RxCounter1>=8||com_data == 0x5B)       //RxBuffer1接受满了,接收数据结束
			{
				RxState=3;
				RxFlag1=1;
				/*以下接收数据*/
				if(RxBuffer1[3])
					f_err_x=-RxBuffer1[2];
				else f_err_x=RxBuffer1[2];
				if(RxBuffer1[5])
					f_err_y=-RxBuffer1[4];
				else f_err_y=RxBuffer1[4];
				fire_flag=RxBuffer1[6];
				ok_flag=RxBuffer1[7];
				
			}
		}

		else if(RxState==3)		//检测是否接受到结束标志
		{
				if(RxBuffer1[RxCounter1-1] == 0x5B)
				{
							
							RxFlag1 = 0;
							RxCounter1 = 0;
							RxState = 0;
						
				}
				else   //接收错误
				{
							RxState = 0;
							RxCounter1=0;
							for(i=0;i<10;i++)
							{
									RxBuffer1[i]=0x00;      //将存放数据数组清零
							}
				}
		} 

		else   //接收异常
		{
				RxState = 0;
				RxCounter1=0;
				for(i=0;i<10;i++)
				{
						RxBuffer1[i]=0x00;      //将存放数据数组清零
				}
		}
}