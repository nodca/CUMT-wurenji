
#include "shijue.h"
#include "stm32f1xx.h"
#include "stdio.h"
#include "usart.h"
#include "bsp_sys.h"
#include "stm32f1xx_it.h"

UNION Pos_x,Pos_y;
u8 x1,x2,x3,x4;
u8 y1,y2,y3,y4;
uint8_t BlueRxBuffer[3]={0};
uint8_t RxBuffer2[6]={0};
uint8_t RxBuffer3[6]={0};
uint8_t RxBuffer4[6]={0};
u8 state=10,huowu_num=0,huowu_flag=0;
void Lanya_Receive_Data(uint8_t com_data)
{
		uint8_t i;
		static uint8_t BlueRxCounter=0;//����
	  
		static uint8_t BlueRxState = 0;	


		if(BlueRxState==0&&com_data==0x12)  //0x12֡ͷ
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

		else if(BlueRxState==3)		//����Ƿ���ܵ�������־
		{
				
			  if(BlueRxBuffer[BlueRxCounter-1] == 0x5B)    
				{
							
							//BlueRxFlag = 0;
							BlueRxState = 0;
							BlueRxCounter = 0;
						
				}
				else   //���մ���
				{
							BlueRxState  = 0;
							BlueRxCounter =0;
							for(i=0;i<3;i++)
							{
									BlueRxBuffer[i]=0x00;      //�����������������
							}
				}
		} 

		else   //�����쳣
		{
				BlueRxState = 0;
				BlueRxCounter = 0;
				for(i=0;i<3;i++)
				{
						BlueRxBuffer[i]=0x00;      //�����������������
				}
		}
}



void feikong_Receive_Data(uint8_t com_data)
{
		uint8_t i;
		static uint8_t RxCounter1=0;//����   
		static uint8_t RxState = 0;	
		static uint8_t RxFlag1 = 0;

		if(RxState==0&&com_data==0xAA)  //0x42֡ͷ
		{
			
			RxState=1;
			RxBuffer2[RxCounter1++]=com_data;   
		}

		else if(RxState==1&&com_data==0xBB)  //0x01��ַ
		{
			RxState=2;
			RxBuffer2[RxCounter1++]=com_data;
		}
		
		else if(RxState==2)
		{
			RxBuffer2[RxCounter1++]=com_data;
			
			if(RxCounter1>=5||com_data == 0xCC)       //RxBuffer1��������,�������ݽ���
			{
				RxState=3;
				RxFlag1=1;
				state=RxBuffer2[2];
				liang_flag=RxBuffer2[3];
			//	huowu_num=RxBuffer2[5];
				//����д��������
			/*	Pos_x.U8_BUFF[1]=RxBuffer2[2];//high
				Pos_x.U8_BUFF[0]=RxBuffer2[3]; //low
				Pos_y.U8_BUFF[1]=RxBuffer2[4];
				Pos_y.U8_BUFF[0]=RxBuffer2[5];*/


			}
		}

		else if(RxState==3)		//����Ƿ���ܵ�������־
		{
				if(RxBuffer2[RxCounter1-1] == 0xCC)
				{
							
							RxFlag1 = 0;
							RxCounter1 = 0;
							RxState = 0;
						
				}
				else   //���մ���
				{
							RxState = 0;
							RxCounter1=0;
							for(i=0;i<6;i++)
							{
									RxBuffer2[i]=0x00;      //�����������������
							}
				}
		} 

		else   //�����쳣
		{
				RxState = 0;
				RxCounter1=0;
				for(i=0;i<6;i++)
				{
						RxBuffer2[i]=0x00;      //�����������������
				}
		}
}
u8 shijue_re[5]={0};
char  com;
void shijue_Receive_Data(uint8_t com_data)
{
		uint8_t i;
		static uint8_t RxCounter1=0;//����   
		static uint8_t RxState = 0;	
		static uint8_t RxFlag1 = 0;

		if(RxState==0&&com_data==0XAA)  //0x42֡ͷ
		{
			
			RxState=1;
			RxBuffer3[RxCounter1++]=com_data;   
		}

		else if(RxState==1&&com_data==0xBB)  //0x01��ַ
		{
			RxState=2;
			RxBuffer3[RxCounter1++]=com_data;
		}
		
		else if(RxState==2)
		{
			
			RxBuffer3[RxCounter1++]=com_data;
		com=RxBuffer3[2];
			if(RxCounter1>=5||com_data == 0xCC)       //RxBuffer1��������,�������ݽ���
			{
				RxState=3;
				RxFlag1=1;
				huowu_flag=RxBuffer3[2];
				huowu_num=RxBuffer3[3];
				//����д��������
				//Pos_x.U8_BUFF[1]=RxBuffer1[2];//high
			//	Pos_x.U8_BUFF[0]=RxBuffer1[3]; //low
			//	Pos_y.U8_BUFF[1]=RxBuffer1[4];
			//	Pos_y.U8_BUFF[0]=RxBuffer1[5];


			}
		}

		else if(RxState==3)		//����Ƿ���ܵ�������־
		{
				if(RxBuffer3[RxCounter1-1] == 0xCC)
				{
							
							RxFlag1 = 0;
							RxCounter1 = 0;
							RxState = 0;
						
				}
				else   //���մ���
				{
							RxState = 0;
							RxCounter1=0;
						/*	for(i=0;i<7;i++)
							{
									RxBuffer3[i]=0x00;      //�����������������
							}*/
				}
		} 

		else   //�����쳣
		{
				RxState = 0;
				RxCounter1=0;
			/*	for(i=0;i<7;i++)
				{
						RxBuffer3[i]=0x00;      //�����������������
				}*/
		}
}

void shijue2_Receive_Data(uint8_t com_data)
{
		uint8_t i;
		static uint8_t RxCounter1=0;//����   
		static uint8_t RxState = 0;	
		static uint8_t RxFlag1 = 0;

		if(RxState==0&&com_data==0XAA)  //0x42֡ͷ
		{
			
			RxState=1;
			RxBuffer4[RxCounter1++]=com_data;   
		}

		else if(RxState==1&&com_data==0xBB)  //0x01��ַ
		{
			RxState=2;
			RxBuffer4[RxCounter1++]=com_data;
		}
		
		else if(RxState==2)
		{
			
			RxBuffer4[RxCounter1++]=com_data;
		  com=RxBuffer4[2];
			if(RxCounter1>=5||com_data == 0xCC)       //RxBuffer1��������,�������ݽ���
			{
				RxState=3;
				RxFlag1=1;
				huowu_flag=RxBuffer4[2];
				huowu_num=RxBuffer4[3];
				//����д��������
				//Pos_x.U8_BUFF[1]=RxBuffer1[2];//high
			//	Pos_x.U8_BUFF[0]=RxBuffer1[3]; //low
			//	Pos_y.U8_BUFF[1]=RxBuffer1[4];
			//	Pos_y.U8_BUFF[0]=RxBuffer1[5];


			}
		}

		else if(RxState==3)		//����Ƿ���ܵ�������־
		{
				if(RxBuffer3[RxCounter1-1] == 0xCC)
				{
							
							RxFlag1 = 0;
							RxCounter1 = 0;
							RxState = 0;
						
				}
				else   //���մ���
				{
							RxState = 0;
							RxCounter1=0;
						/*	for(i=0;i<7;i++)
							{
									RxBuffer3[i]=0x00;      //�����������������
							}*/
				}
		} 

		else   //�����쳣
		{
				RxState = 0;
				RxCounter1=0;
			/*	for(i=0;i<7;i++)
				{
						RxBuffer3[i]=0x00;      //�����������������
				}*/
		}
}

/*****************  �����ַ� **********************/
void Usart_SendByte(uint8_t str)
{
  HAL_UART_Transmit(&huart3, &str, 1, 1000);
	//HAL_UART_Transmit_IT(&huart1, &str, 1);
	
}
void Usart2_SendByte(uint8_t str)  
{
  HAL_UART_Transmit(&huart1, &str, 1, 1000);
	//HAL_UART_Transmit_IT(&huart1, &str, 1);
	
}
u8 FeiKongSendBuf[4] = {0x2c, 0x12, 0, 0x5b};
u8 BLueToothSendBuf[7]={0XAA,0XAA,0XBB,0,0,0,0XCC};
void FeiKongTransmitData()
{
   	u8 i=1;
	  FeiKongSendBuf[2] = ok_flag;
	
		for(i = 1; i <= 3; i++)   //�����ַ�����λ������ͨ��HEX���������Ƿ���ȷ
		{
			Usart_SendByte(FeiKongSendBuf[i]);   
		}
	
//  	for(i = 0; i <= 5; i++)   //�����ַ���
//		{
//			sprintf((char *)BlueToothSendBuf, "2%d%d%d8", TASK, TargetNum, Load_flag);
//				HAL_UART_Transmit(&huart1, BlueToothSendBuf, sizeof(BlueToothSendBuf), 1000);
//		}
	       
}
void LanYaTransmitData(){
	static u8 stage=0;
	if(stage==0){
	  Usart2_SendByte(0xAA);
		stage=1;
	}
	else if(stage==1){
	  Usart2_SendByte(0xBB);
		stage=2;
	}
	else if(stage==2){
	  Usart2_SendByte(0xAA);
		stage=3;
	}
	else if(stage==3){
	  Usart2_SendByte(state);
		stage=4;
	}
	else if(stage==4){
	  Usart2_SendByte(liang_flag);
		stage=5;
	}
	else if(stage==5){
	  Usart2_SendByte(huowu_num);
		stage=6;
	}
	else if(stage==6){
	 Usart2_SendByte(huowu_num);
		stage=7;
	}  
	else if(stage==7){
	  Usart2_SendByte(huowu_num);
		stage=8;
	}
	else stage=0;
}

