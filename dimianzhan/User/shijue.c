
#include "shijue.h"
#include "stm32f1xx.h"
#include "stdio.h"
#include "usart.h"
#include "bsp_sys.h"
#include "stm32f1xx_it.h"
#include "main.h"
UNION Pos_x,Pos_y;
u8 x1,x2,x3,x4;
u8 y1,y2,y3,y4;
uint8_t RxBuffer1[7]={0},RxBuffer2[5]={0};
u8 state=10,liang_flag=0,huowu_num[25]={0},last_liang_flag=0;;
u8	huowu_last_num=0;
u8 target_huowu=0;
u8 num=0;
u8 huowu=0;
u8 tar_flag=0;
void lanya_rec(uint8_t com_data)
{ static u8 i=0;
  if(i==0&&com_data==0XAA){
		i++;
	}
	else if(i==1&&com_data==0XBB){
		i++;
	}
	else if(i==2&&com_data==0XAA){
		i++;
	}
	else if(i==3){
		state=com_data;
		i++;
	}
	else if(i==4){
		liang_flag=com_data;
		i++;
	}
	else if(i==5){
		i++;

	}
	else if(i==6){
	i++;
		if(com_data<=24)
				huowu=com_data;
		if(huowu_last_num!=huowu&&huowu!=0&&num<=23){
		huowu_num[num]=huowu;
		num++;
		huowu_last_num=huowu;
	  }
		else if(jieshou_flag==1&&tar_flag==0&&huowu!=0&&huowu!=huowu_last_num){
		num=25;
			huowu_num[num-1]=huowu;
		huowu_last_num=huowu;
			tar_flag=1;
		}

	}
	else if(i==7){
	i++;
	}
	else if(i==8) i=0;
	else i=0;
}
void lanya_Receive_Data(uint8_t com_data)
{
		uint8_t i;
		static uint8_t RxCounter1=0;//计数   
		static uint8_t RxState = 0;	
		static uint8_t RxFlag1 = 0;

		if(RxState==0&&com_data==0xAA)  //0x42帧头
		{
			
			RxState=1;
			RxBuffer1[RxCounter1++]=com_data;   
		}

		else if(RxState==1&&com_data==0xBB)  //0x01地址
		{
			RxState=2;
			RxBuffer1[RxCounter1++]=com_data;
		}
		else if(RxState==2&&com_data==0xAA)  //0x01地址
		{
			RxState=3;
			RxBuffer1[RxCounter1++]=com_data;
		}
		
		else if(RxState==3)
		{
			RxBuffer1[RxCounter1++]=com_data;
			
			if(RxCounter1>=7||com_data == 0xCC)       //RxBuffer1接受满了,接收数据结束
			{
				RxState=4;
				RxFlag1=1;
				//以下写接受任务
				state=RxBuffer1[3];
				liang_flag=RxBuffer1[4];
				if(huowu_last_num!=RxBuffer1[5]&&RxBuffer1[5]!=0){
					huowu_num[num]=RxBuffer1[5];
					num++;
					huowu_last_num=RxBuffer1[5];
				}

			}
		}

		else if(RxState==4)		//检测是否接受到结束标志
		{
				if(RxBuffer1[RxCounter1-1] == 0xCC)
				{
							
							RxFlag1 = 0;
							RxCounter1 = 0;
							RxState = 0;
						
				}
				else   //接收错误
				{
							RxState = 0;
							RxCounter1=0;
							for(i=0;i<8;i++)
							{
									RxBuffer1[i]=0x00;      //将存放数据数组清零
							}
				}
		} 

		else   //接收异常
		{
			
				RxState = 0;
				RxCounter1=0;
				for(i=0;i<8;i++)
				{
						RxBuffer1[i]=0x00;      //将存放数据数组清零
				}
		}
}

void HMI_Receive_Data(uint8_t com_data)
{
		uint8_t i;
		static uint8_t RxCounter1=0;//计数   
		static uint8_t RxState = 0;	
		static uint8_t RxFlag1 = 0;

		if(RxState==0&&com_data==0xAA)  //0x42帧头
		{
			
			RxState=1;
			RxBuffer2[RxCounter1++]=com_data;   
		}

		else if(RxState==1&&com_data==0xBB)  //0x01地址
		{
			RxState=2;
			RxBuffer2[RxCounter1++]=com_data;
		}
		
		else if(RxState==2)
		{
			RxBuffer2[RxCounter1++]=com_data;
			
			if(RxCounter1>=5||com_data == 0xCC)       //RxBuffer1接受满了,接收数据结束
			{
				RxState=3;
				RxFlag1=1;
				//以下写接受任务
				ok_flag=RxBuffer2[2];
				target_huowu=RxBuffer2[3];

			}
		}

		else if(RxState==3)		//检测是否接受到结束标志
		{
				if(RxBuffer2[RxCounter1-1] == 0xCC)
				{
							
							RxFlag1 = 0;
							RxCounter1 = 0;
							RxState = 0;
						
				}
				else   //接收错误
				{
							RxState = 0;
							RxCounter1=0;
							for(i=0;i<6;i++)
							{
									RxBuffer2[i]=0x00;      //将存放数据数组清零
							}
				}
		} 

		else   //接收异常
		{
				RxState = 0;
				RxCounter1=0;
				for(i=0;i<6;i++)
				{
						RxBuffer2[i]=0x00;      //将存放数据数组清零
				}
		}
}

/*****************  发送字符 **********************/
void Usart_SendByte(uint8_t str)
{
  HAL_UART_Transmit(&huart1, &str, 1, 1000);
	//HAL_UART_Transmit_IT(&huart1, &str, 1);
	
}
u8 BlueToothSendBuf[5] = {0x2c, 0x12, 0, 0x5b};

void BlueToothTransmitData()
{
   	u8 i=1;
	if(ok_flag!=4)
	  BlueToothSendBuf[2] = ok_flag;
	else BlueToothSendBuf[2] = ok_flag*10+hangxian_num;
	//hangxian_num
		for(i = 1; i <= 3; i++)   //发送字符，上位机可以通过HEX检验数据是否正确
		{
			Usart_SendByte(BlueToothSendBuf[i]);   
		}
	
//  	for(i = 0; i <= 5; i++)   //发送字符串
//		{
//			sprintf((char *)BlueToothSendBuf, "2%d%d%d8", TASK, TargetNum, Load_flag);
//				HAL_UART_Transmit(&huart1, BlueToothSendBuf, sizeof(BlueToothSendBuf), 1000);
//		}
	 
	      
}


