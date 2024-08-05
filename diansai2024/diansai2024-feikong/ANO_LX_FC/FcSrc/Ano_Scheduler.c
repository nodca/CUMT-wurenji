/******************** (C) COPYRIGHT 2017 ANO Tech ********************************
 * ����    �������ƴ�
 * ����    ��www.anotc.com
 * �Ա�    ��anotc.taobao.com
 * ����QȺ ��190169595
 * ����    ���������
**********************************************************************************/
#include "Ano_Scheduler.h"
#include "User_Task.h"
#include "LX_FC_Fun.h"
#include "ANO_DT_LX.h"
#include "Drv_AnoOf.h"
#include "raspi.h"
#include "lanya.h"
int task_num=0,error_num=0,fire_num=0,task2_num=0;
u16 time=0,error_time=0,fire_time=0,time2=0;
s16 x=0,y=0,z=0,yaw=0;
s16 Height=140; //140cm
s16 land_time=200,error_land_time=250;//4s 5s
u8 fire_task=0;
u8 land=0;
u8 led_flag=0;
int task2=-1;
u8 mode=0;
/*���������������д������*/


void jiguang_test(){
								if(base_link.pose_x_cm>(-75-3)&&base_link.pose_x_cm<(-75+3)){
									led_flag=1;
								}
								else if(base_link.pose_x_cm>(-125-3)&&base_link.pose_x_cm>(-125+3)){
									led_flag=2;
								}
								else if(base_link.pose_x_cm>(-175-3)&&base_link.pose_x_cm>(-175+3)){
									led_flag=3;
								}
}
//ֱ�ӷ�
void test1(){
	rc_in.rc_ch.st_data.ch_[ch_5_aux1]=1500;
	switch(task_num)
					{
						case 0:
						{
							
							//reset
							//rc_in.rc_ch.st_data.ch_[ch_5_aux1]=1500;//����ģʽ
							rc_in.rc_ch.st_data.ch_[ch_5_aux1]=1500;
							PID_init();	
              //TIM_SetCompare4(TIM4,140);							
							task_num=1;
							
						}
						break;
						case 1://����
						{
							
							if(time<1000)//20s  50=1s
							{
								time++;
							}
							else if(time>=1000&&ok_flag==3)
							{
								time = 0;
								task_num=2;
								FC_Unlock();
							}
						}
						break;
						case 2://���
						{
							if(time<100){  //2s
							time++;
							}
							else{
							time=0;
							task_num=3;
							OneKey_Takeoff(Height);
							}
						}
						break;
						case 3://��1��
						{
							if(time<100)//������ʱ
							{
								time++;
							}
							else
							{
								time = 0;
								task_num=4;
							}
						}
						break;
						case 4:
						{
							if(time<550){  //11s
																if(base_link.pose_x_cm>(-75-3)&&base_link.pose_x_cm<(-75+3)){
									led_flag=1;
								}
								else if(base_link.pose_x_cm>(-125-3)&&base_link.pose_x_cm>(-125+3)){
									led_flag=2;
								}
								else if(base_link.pose_x_cm>(-175-3)&&base_link.pose_x_cm>(-175+3)){
									led_flag=3;
								}
								time++;
							x=xyPos_set(base_link.pose_x_cm,-175);
							y=xyPos_set(base_link.pose_y_cm,0);
							z=height_set(ano_of.of_alt_cm,Height);
						//	yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
							tar_setdata(x,y, z,yaw);

							}
							else{
								tar_setdata(0,0,0,0);
								task_num=5;
								time = 0;
							}
						}	
						break;
						case 5:
						{
							if(time<150){  //3s
							time++;
								x=xyPos_set(base_link.pose_x_cm,-175);
								y=xyPos_set(base_link.pose_y_cm,0);
								z=height_set(ano_of.of_alt_cm,100);
							//	yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
								tar_setdata(x,y, z,yaw);
							}
							else{
							tar_setdata(0,0,0,0);
							task_num=6;
							time=0;
							}
						
				}
						break;
				case 6:
						{
							if(time<550){  //11s
							if(base_link.pose_x_cm>(-75-3)&&base_link.pose_x_cm<(-75+3)){
									led_flag=6;
								}
								else if(base_link.pose_x_cm>(-125-3)&&base_link.pose_x_cm>(-125+3)){
									led_flag=5;
								}
								else if(base_link.pose_x_cm>(-175-3)&&base_link.pose_x_cm>(-175+3)){
									led_flag=4;
								}
							time++;
								x=xyPos_set(base_link.pose_x_cm,0);
								y=xyPos_set(base_link.pose_y_cm,0);
								z=height_set(ano_of.of_alt_cm,100);
								//yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
								tar_setdata(x,y,z,yaw);

							}
							else{
							tar_setdata(0,0,0,0);
						//	TIM_SetCompare4(TIM4,220);	
							task_num=12;
							time=0;
							}
						
				}
						break;
				case 7:
						{
						
						
				}
						break;
						case 8:
						{
							
						}
						break;
						case 9:
						{
							
						}
						break;
						case 10:
						{
							
						}
						break;
						case 11:
						{
							
						}
						break;
						case 12://����
						{
							OneKey_Land();
							task_num=13;
						}
						break;
						case 13://3s
						{
							if(time<land_time)
							{
								time++;
							}
							else
							{
								task_num=14;
								time = 0;
							}
						}
						break;
						case 14:
						{
							FC_Lock();
							task_num=15;
						}
						break;
					}

}
//���·�
void test2(){
	rc_in.rc_ch.st_data.ch_[ch_5_aux1]=1500;
	switch(task_num)
					{
						case 0:
						{
							
							//reset
							//rc_in.rc_ch.st_data.ch_[ch_5_aux1]=1500;//����ģʽ
							rc_in.rc_ch.st_data.ch_[ch_5_aux1]=1500;
							PID_init();	
              //TIM_SetCompare4(TIM4,140);							
							task_num=1;
							
						}
						break;
						case 1://����
						{
							
							if(time<1000)//20s  50=1s
							{
								time++;
							}
							else if(time>=1000&&ok_flag==3)
							{
								time = 0;
								task_num=2;
								FC_Unlock();
							}
						}
						break;
						case 2://���
						{
							if(time<100){  //2s
							time++;
							}
							else{
							time=0;
							task_num=3;
							OneKey_Takeoff(150);
							}
						}
						break;
						case 3://��1��
						{
							if(time<100)//������ʱ
							{
								time++;
							}
							else
							{
								time = 0;
								task_num=4;
							}
						}
						break;
						case 4:
						{
							if(time<150){  //3s
								time++;
							x=xyPos_set(base_link.pose_x_cm,-75);
							y=xyPos_set(base_link.pose_y_cm,0);
							z=height_set(ano_of.of_alt_cm,145);
						//	yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
							tar_setdata(x,y, z,yaw);
								if(time>=125)  //��ǰ0.25s���伤��
									led_flag=1;
							}
							else{
								tar_setdata(0,0,0,0);
								led_flag=1;
								task_num=5;
								time = 0;
							}
						}	
						break;
						case 5:
						{
							if(time<150){  //3s
							time++;
								x=xyPos_set(base_link.pose_x_cm,-75);
								y=xyPos_set(base_link.pose_y_cm,0);
								z=height_set(ano_of.of_alt_cm,100);
							//	yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
								tar_setdata(x,y,z,yaw);
								if(time>=120)  //��ǰ0.25s���伤��
									led_flag=2;
							}
							else{
							tar_setdata(0,0,0,0);
								led_flag=2;
							task_num=6;
							time=0;
							}
						
				}
						break;
				case 6:
						{
							if(time<150){  //3s
							time++;
								x=xyPos_set(base_link.pose_x_cm,-125);
								y=xyPos_set(base_link.pose_y_cm,0);
								z=height_set(ano_of.of_alt_cm,100);
								//yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
								tar_setdata(x,y,z,yaw);
								if(time>=120)  //��ǰ0.25s���伤��
									led_flag=3;
							}
							else{
							tar_setdata(0,0,0,0);
								led_flag=3;
							task_num=7;
							time=0;
							}
						
				}
						break;
				case 7:
						{
							if(time<150){  //3s
							time++;
								x=xyPos_set(base_link.pose_x_cm,-125);
								y=xyPos_set(base_link.pose_y_cm,0);
								z=height_set(ano_of.of_alt_cm,145);
								//yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
								tar_setdata(x,y,z,yaw);
						if(time>=120)  //��ǰ0.25s���伤��
									led_flag=4;
							}
							else{
							tar_setdata(0,0,0,0);	
							led_flag=4;
							task_num=8;
							time=0;
							}
						
						
				}
						break;
						case 8:
						{
						if(time<150){  //3s
							time++;
								x=xyPos_set(base_link.pose_x_cm,-175);
								y=xyPos_set(base_link.pose_y_cm,0);
								z=height_set(ano_of.of_alt_cm,145);
								//yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
								tar_setdata(x,y,z,yaw);
					     	if(time>=120)  //��ǰ0.25s���伤��
									led_flag=5;
							}
							else{
							tar_setdata(0,0,0,0);	
								led_flag=5;
							task_num=9;
							time=0;
							}
							
						}
						break;
						case 9:
						{
							if(time<200){  //4s
							time++;
								x=xyPos_set(base_link.pose_x_cm,-175);
								y=xyPos_set(base_link.pose_y_cm,0);
								z=height_set(ano_of.of_alt_cm,100);
								//yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
								tar_setdata(x,y,z,yaw);
								if(time>=160) //��ǰ0.25s���伤��
								led_flag=6;		
							}
							else{
							tar_setdata(0,0,0,0);
               led_flag=6;			
							task_num=10;
							time=0;
							}
						}
						break;
						/*����A�����*/
						/*�����Ƹ�*/
						case 10:
						{
              if(time<200){  //4s
							time++;
								x=xyPos_set(base_link.pose_x_cm,-270);
								y=xyPos_set(base_link.pose_y_cm,0);
								z=height_set(ano_of.of_alt_cm,100);
								//yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
								tar_setdata(x,y,z,yaw);
							}
							else{
							tar_setdata(0,0,0,0);
                led_flag=6;								
							task_num=11;
							time=0;
							}
							
						}
						break;
						/*��A�浽B��*/
						case 11:
						{
							if(time<300){  //6s
							time++;
								x=xyPos_set(base_link.pose_x_cm,-270);
								y=xyPos_set(base_link.pose_y_cm,150);
								z=height_set(ano_of.of_alt_cm,100);
								//yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
								tar_setdata(x,y,z,yaw);
							}
							else{
							tar_setdata(0,0,0,0);
                led_flag=6;								
							task_num=12;
							time=0;
							}
						}
						break;
						/*�Ƹ˽�������ʼB��*/
						case 12:
						{
							if(time<200){  //4s
							  time++;
								x=xyPos_set(base_link.pose_x_cm,-175);
								y=xyPos_set(base_link.pose_y_cm,150);
								z=height_set(ano_of.of_alt_cm,100);
								//yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
								tar_setdata(x,y,z,yaw);
								if(time>=160) //��ǰ0.25s���伤��
								led_flag=7;	
							}
							else{
							tar_setdata(0,0,0,0);
                led_flag=7;								
							task_num=13;
							time=0;
							}
						}
						break;
						case 13:
						{
							if(time<150){  //3s
							  time++;
								x=xyPos_set(base_link.pose_x_cm,-175);
								y=xyPos_set(base_link.pose_y_cm,150);
								z=height_set(ano_of.of_alt_cm,145);
								//yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
								tar_setdata(x,y,z,yaw);
								if(time>=120) //��ǰ0.25s���伤��
								led_flag=8;	
							}
							else{
							tar_setdata(0,0,0,0);
                led_flag=8;								
							task_num=14;
							time=0;
							}
						}
						break;
						case 14:
						{
							if(time<150){  //3s
							  time++;
								x=xyPos_set(base_link.pose_x_cm,-125);
								y=xyPos_set(base_link.pose_y_cm,150);
								z=height_set(ano_of.of_alt_cm,140);
								//yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
								tar_setdata(x,y,z,yaw);
								if(time>=120) //��ǰ0.25s���伤��
								led_flag=9;	
							}
							else{
							tar_setdata(0,0,0,0);
                led_flag=9;								
							task_num=15;
							time=0;
							}
						}
						break;
						case 15:
						{
							if(time<150){  //3s
							  time++;
								x=xyPos_set(base_link.pose_x_cm,-125);
								y=xyPos_set(base_link.pose_y_cm,150);
								z=height_set(ano_of.of_alt_cm,100);
								//yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
								tar_setdata(x,y,z,yaw);
								if(time>=120) //��ǰ0.25s���伤��
								led_flag=10;	
							}
							else{
							tar_setdata(0,0,0,0);
                led_flag=10;								
							task_num=16;
							time=0;
							}
						}
						break;
						case 16:
						{
							if(time<150){  //3s
							  time++;
								x=xyPos_set(base_link.pose_x_cm,-75);
								y=xyPos_set(base_link.pose_y_cm,150);
								z=height_set(ano_of.of_alt_cm,100);
								//yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
								tar_setdata(x,y,z,yaw);
								if(time>=120) //��ǰ0.25s���伤��
								led_flag=11;	
							}
							else{
							tar_setdata(0,0,0,0);
                led_flag=11;								
							task_num=17;
							time=0;
							}
						}
						break;
						case 17:
						{
							if(time<200){  //4s
							  time++;
								x=xyPos_set(base_link.pose_x_cm,-75);
								y=xyPos_set(base_link.pose_y_cm,150);
								z=height_set(ano_of.of_alt_cm,145);
								//yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
								tar_setdata(x,y,z,yaw);
								if(time>=160) //��ǰ0.25s���伤��
								led_flag=12;	
							}
							else{
							tar_setdata(0,0,0,0);
                led_flag=12;								
							task_num=18;
							time=0;
							}
						}
						break;
						/*C�濪ʼ*/
						case 18:
						{
							if(time<200){  //4s
							  time++;
								x=xyPos_set(base_link.pose_x_cm,-75);
								y=xyPos_set(base_link.pose_y_cm,200);
								z=height_set(ano_of.of_alt_cm,145);
								//yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
								tar_setdata(x,y,z,yaw);
								if(time>=160) //��ǰ0.25s���伤��
								led_flag=13;	
							}
							else{
							tar_setdata(0,0,0,0);
                led_flag=13;								
							task_num=19;
							time=0;
							}
						}
						break;
						case 19:
						{
							if(time<200){  //4s
							  time++;
								x=xyPos_set(base_link.pose_x_cm,-75);
								y=xyPos_set(base_link.pose_y_cm,200);
								z=height_set(ano_of.of_alt_cm,100);
								//yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
								tar_setdata(x,y,z,yaw);
								if(time>=160) //��ǰ0.25s���伤��
								led_flag=14;	
							}
							else{
							tar_setdata(0,0,0,0);
                led_flag=14;								
							task_num=20;
							time=0;
							}
						}
						break;
					case 20:
						{
							if(time<150){  //3s
							  time++;
								x=xyPos_set(base_link.pose_x_cm,-125);
								y=xyPos_set(base_link.pose_y_cm,200);
								z=height_set(ano_of.of_alt_cm,100);
								//yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
								tar_setdata(x,y,z,yaw);
								if(time>=120) //��ǰ0.25s���伤��
								led_flag=15;	
							}
							else{
							tar_setdata(0,0,0,0);
                led_flag=15;								
							task_num=21;
							time=0;
							}
						}
						break;
					case 21:
						{
							if(time<150){  //3s
							  time++;
								x=xyPos_set(base_link.pose_x_cm,-125);
								y=xyPos_set(base_link.pose_y_cm,200);
								z=height_set(ano_of.of_alt_cm,140);
								//yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
								tar_setdata(x,y,z,yaw);
								if(time>=120) //��ǰ0.25s���伤��
								led_flag=16;	
							}
							else{
							tar_setdata(0,0,0,0);
                led_flag=16;								
							task_num=22;
							time=0;
							}
						}
						break;
					case 22:
						{
							if(time<150){  //3s
							  time++;
								x=xyPos_set(base_link.pose_x_cm,-175);
								y=xyPos_set(base_link.pose_y_cm,200);
								z=height_set(ano_of.of_alt_cm,140);
								//yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
								tar_setdata(x,y,z,yaw);
								if(time>=120) //��ǰ0.25s���伤��
								led_flag=17;	
							}
							else{
							tar_setdata(0,0,0,0);
                led_flag=17;								
							task_num=23;
							time=0;
							}
						}
						break;
					case 23:
						{
							if(time<150){  //3s
							  time++;
								x=xyPos_set(base_link.pose_x_cm,-175);
								y=xyPos_set(base_link.pose_y_cm,200);
								z=height_set(ano_of.of_alt_cm,100);
								//yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
								tar_setdata(x,y,z,yaw);
								if(time>=120) //��ǰ0.25s���伤��
								led_flag=18;	
							}
							else{
							tar_setdata(0,0,0,0);
                led_flag=18;								
							task_num=24;
							time=0;
							}
						}
						break;
						/*��ʼ�Ƶ�D��*/
					case 24:
						{
							if(time<200){  //4s
							  time++;
								x=xyPos_set(base_link.pose_x_cm,-270);
								y=xyPos_set(base_link.pose_y_cm,200);
								z=height_set(ano_of.of_alt_cm,100);
								//yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
								tar_setdata(x,y,z,yaw);
							}
							else{
							tar_setdata(0,0,0,0);
                led_flag=18;								
							task_num=25;
							time=0;
							}
						}
						break;
						case 25:
						{
							if(time<300){  //6s
							  time++;
								x=xyPos_set(base_link.pose_x_cm,-270);
								y=xyPos_set(base_link.pose_y_cm,350);
								z=height_set(ano_of.of_alt_cm,100);
								//yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
								tar_setdata(x,y,z,yaw);
							}
							else{
							tar_setdata(0,0,0,0);
                led_flag=18;								
							task_num=26;
							time=0;
							}
						}
						break;
						/*�Ƹ˽�����֮����D��*/
						case 26:
						{
							if(time<200){  //4s
							  time++;
								x=xyPos_set(base_link.pose_x_cm,-175);
								y=xyPos_set(base_link.pose_y_cm,350);
								z=height_set(ano_of.of_alt_cm,100);
								//yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
								tar_setdata(x,y,z,yaw);
								if(time>=160) //��ǰ0.5s���伤��
								led_flag=19;	
							}
							else{
							tar_setdata(0,0,0,0);
                led_flag=19;								
							task_num=27;
							time=0;
							}
						}
						break;
					case 27:
						{
							if(time<150){  //3s
							  time++;
								x=xyPos_set(base_link.pose_x_cm,-125);
								y=xyPos_set(base_link.pose_y_cm,350);
								z=height_set(ano_of.of_alt_cm,100);
								//yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
								tar_setdata(x,y,z,yaw);
								if(time>=120) //��ǰ0.5s���伤��
								led_flag=20;	
							}
							else{
							tar_setdata(0,0,0,0);
                led_flag=20;								
							task_num=28;
							time=0;
							}
						}
						break;
						case 28:
						{
							if(time<150){  //3s
							  time++;
								x=xyPos_set(base_link.pose_x_cm,-75);
								y=xyPos_set(base_link.pose_y_cm,350);
								z=height_set(ano_of.of_alt_cm,100);
								//yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
								tar_setdata(x,y,z,yaw);
								if(time>=120) //��ǰ0.5s���伤��
								led_flag=21;	
							}
							else{
							tar_setdata(0,0,0,0);
               led_flag=21;								
							task_num=29;
							time=0;
							}
						}
						break;
						case 29:
						{
							if(time<150){  //3s
							  time++;
								x=xyPos_set(base_link.pose_x_cm,-75);
								y=xyPos_set(base_link.pose_y_cm,350);
								z=height_set(ano_of.of_alt_cm,140);
								//yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
								tar_setdata(x,y,z,yaw);
								if(time>=120) //��ǰ0.5s���伤��
								led_flag=22;	
							}
							else{
							tar_setdata(0,0,0,0);
               led_flag=22;								
							task_num=30;
							time=0;
							}
						}
						break;
						case 30:
						{
							if(time<150){  //3s
							  time++;
								x=xyPos_set(base_link.pose_x_cm,-125);
								y=xyPos_set(base_link.pose_y_cm,350);
								z=height_set(ano_of.of_alt_cm,140);
								//yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
								tar_setdata(x,y,z,yaw);
								if(time>=120) //��ǰ0.5s���伤��
								led_flag=23;	
							}
							else{
							tar_setdata(0,0,0,0);
               led_flag=23;								
							task_num=31;
							time=0;
							}
						}
						break;
						case 31:
						{
							if(time<150){  //3s
							  time++;
								x=xyPos_set(base_link.pose_x_cm,-175);
								y=xyPos_set(base_link.pose_y_cm,350);
								z=height_set(ano_of.of_alt_cm,140);
								//yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
								tar_setdata(x,y,z,yaw);
								if(time>=120) //��ǰ0.5s���伤��
								led_flag=24;	
							}
							else{
							tar_setdata(0,0,0,0);
               led_flag=24;								
							task_num=32;
							time=0;
							}
						}
						break;
						/*D����������䵽�յ�*/
						case 32:
						{
							if(time<150){  //4s
							  time++;
								x=xyPos_set(base_link.pose_x_cm,-250);
								y=xyPos_set(base_link.pose_y_cm,350);
								z=height_set(ano_of.of_alt_cm,140);
								//yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
							}
							else{
							tar_setdata(0,0,0,0);
               led_flag=25;								
							task_num=33;
							time=0;
							}
						}
						break;
						/*����*/
						case 33:
						{
							if(time<150){  //3s
								led_flag=25;
							  time++;
								x=xyPos_set(base_link.pose_x_cm,-250);
								y=xyPos_set(base_link.pose_y_cm,350);
								z=height_set(ano_of.of_alt_cm,50);
								//yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
								tar_setdata(x,y,z,yaw);	
							}
							else{
							tar_setdata(0,0,0,0);						
							task_num=34;
							time=0;
							}
						}
						break;
						case 34://����
						{
							OneKey_Land();
							task_num=35;
						}
						break;
						case 35://5s
						{
							if(time<land_time)
							{
								time++;
							}
							else
							{
								task_num=36;
								time = 0;
							}
						}
						break;
						case 36:
						{
							FC_Lock();
							task_num=37;
						}
						break;
					}

}
void test(){
	rc_in.rc_ch.st_data.ch_[ch_5_aux1]=1500;
	switch(task_num)
					{
						case 0:
						{
							
							//reset
							//rc_in.rc_ch.st_data.ch_[ch_5_aux1]=1500;//����ģʽ
							rc_in.rc_ch.st_data.ch_[ch_5_aux1]=1500;
							PID_init();	
              //TIM_SetCompare4(TIM4,140);							
							task_num=1;
							
						}
						break;
						case 1://����
						{
							
							if(time<1000)//20s  50=1s
							{
								time++;
							}
							else if(time>=1000&&ok_flag==3)
							{
								time = 0;
								task_num=2;
								FC_Unlock();
							}
						}
						break;
						case 2://���
						{
							if(time<100){  //2s
							time++;
							}
							else{
							time=0;
							task_num=3;
							OneKey_Takeoff(Height);
							}
						}
						break;
						case 3://��1��
						{
							if(time<100)//������ʱ
							{
								time++;
							}
							else
							{
								time = 0;
								task_num=4;
							}
						}
						break;
						case 4:
						{
							if(time<250){  //5s
								time++;
							x=xyPos_set(base_link.pose_x_cm,-175);
							y=xyPos_set(base_link.pose_y_cm,0);
							z=height_set(ano_of.of_alt_cm,Height);
						//	yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
							tar_setdata(x,y, z,yaw);}
							else{
								tar_setdata(0,0,0,0);
								task_num=5;
								time = 0;
							}
						}	
						break;
						case 5:
						{
							if(time<100){  //2s
							time++;
								x=xyPos_set(base_link.pose_x_cm,-175);
								y=xyPos_set(base_link.pose_y_cm,0);
								z=height_set(ano_of.of_alt_cm,100);
							//	yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
								tar_setdata(x,y,z,yaw);
							}
							else{
							tar_setdata(0,0,0,0);
							task_num=6;
							time=0;
							}
						
				}
						break;
				case 6:
						{
							if(time<250){  //5s
							time++;
								x=xyPos_set(base_link.pose_x_cm,0);
								y=xyPos_set(base_link.pose_y_cm,0);
								z=height_set(ano_of.of_alt_cm,100);
								//yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
								tar_setdata(x,y,z,yaw);
							}
							else{
							tar_setdata(0,0,0,0);
							TIM_SetCompare4(TIM4,220);	
							task_num=7;
							time=0;
							}
						
				}
						break;
				case 7:
						{
							if(time<150){  //3s
							time++;
								x=xyPos_set(base_link.pose_x_cm,-275);
								y=xyPos_set(base_link.pose_y_cm,25);
								z=height_set(ano_of.of_alt_cm,Height);
								yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
								tar_setdata(x,y,z,yaw);
							}
							else{
							tar_setdata(0,0,0,0);
							//TIM_SetCompare4(TIM4,220);	
							task_num=8;
							time=0;
							}
						
				}
						break;
						case 8:
						{
							if(time<300){  //6s
							time++;
								x=xyPos_set(base_link.pose_x_cm,-275);
								y=xyPos_set(base_link.pose_y_cm,125);
								z=height_set(ano_of.of_alt_cm,Height);
								yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
								tar_setdata(x,y,z,yaw);
							}
							else{
							tar_setdata(0,0,0,0);
							task_num=9;	
							time=0;
							}		
						}
						break;
						case 9:
						{
							if(time<600){  //12s
							time++;
								x=xyPos_set(base_link.pose_x_cm,-260);
								y=xyPos_set(base_link.pose_y_cm,60);
								z=height_set(ano_of.of_alt_cm,Height);
								yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
								tar_setdata(x,y,z,yaw);
							}
							else{
							tar_setdata(0,0,0,0);
							task_num=10;	
							time=0;
							}		
						}
						break;
						case 10:
						{
							if(time<150){  //3s
							time++;
								x=xyPos_set(base_link.pose_x_cm,-260);
								y=xyPos_set(base_link.pose_y_cm,0);
								z=height_set(ano_of.of_alt_cm,Height);
								yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
								tar_setdata(x,y,z,yaw);
							}
							else{
							tar_setdata(0,0,0,0);
							task_num=11;	
							time=0;
							}		
						}
						break;
						case 11:
						{
							if(time<650){  //13s
							time++;
								x=xyPos_set(base_link.pose_x_cm,0);
								y=xyPos_set(base_link.pose_y_cm,0);
								z=height_set(ano_of.of_alt_cm,Height);
								yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
								tar_setdata(x,y,z,yaw);
							}
							else{
							tar_setdata(0,0,0,0);
							task_num=12;	
							time=0;
							}		
						}
						break;
						case 12://����
						{
							OneKey_Land();
							task_num=13;
						}
						break;
						case 13://3s
						{
							if(time<land_time)
							{
								time++;
							}
							else
							{
								task_num=14;
								time = 0;
							}
						}
						break;
						case 14:
						{
							FC_Lock();
							task_num=15;
						}
						break;
					}

}



void error_land(){
	if(error_num==0){
		tar_setdata(0,0,0,0);
		error_num=1;
	}
	else if(error_num==1){
		OneKey_Land();
		error_num=2;
	}
	else if(error_num==2){
	error_time++;
		if(error_time>=error_land_time){
			error_num=3;
			error_time=0;
		}
	}
	else if(error_num==3){
		FC_Lock();
		error_num=4;
	}
}

void move_pos(){
			switch(task_num)
					{
						case 0:
						{
							//reset
							rc_in.rc_ch.st_data.ch_[ch_5_aux1]=1500;//����ģʽ
							PID_init();		
							task_num=1;	
						}
						break;
						case 1://����
						{
							if(time<1500)//30s
							{
								time++;
							}
							else
							{
								time = 0;
								task_num=2;
								FC_Unlock();
							}
						}
						break;
						case 2://���100cm
						{
							if(time<150){  //3s
							time++;
							}
							else{
							time=0;
							task_num=3;
							OneKey_Takeoff(Height);
							}
						}
						break;
						case 3://��3��
						{
							if(time<150)//������ʱ
							{
								time++;
							}
							else
							{
								time = 0;
								task_num=4;
							}
						}
						break;
						case 4:
						{
							if(time<150){  //3s
							time++;
								x=xyPos_set(base_link.pose_x_cm,-50);
								y=xyPos_set(base_link.pose_y_cm,50);
								z=height_set(ano_of.of_alt_cm,Height);
								yaw=0;
								tar_setdata(x,y,z,yaw);
							}
							else{
							tar_setdata(0,0,0,0);
							task_num=5;	
							time=0;
							}							
						}	
						break;
						case 5:
						{

							task_num=6;

						
				}
						break;
						case 6:
						{

							task_num=7;	

						}
						break;
						case 7:
						{
	
							task_num=8;	

						}
						break;
						case 8:
						{
	
							task_num=9;	
	
						}
						break;
						case 9:
						{
							
							task_num=10;	

						}
						break;
						case 10://����
						{
							OneKey_Land();
							task_num=11;
						}
						break;
						case 11://3s
						{
							if(time<150)
							{
								time++;
							}
							else
							{
								task_num=12;
								time = 0;
							}
						}
						break;
						case 12:
						{
							FC_Lock();
							task_num=13;
						}
						break;
					}
}

/**/
//////////////////////////////////////////////////////////////////////
//�û����������
//////////////////////////////////////////////////////////////////////

static void Loop_1000Hz(void) //1msִ��һ��
{
	//////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////
}

static void Loop_500Hz(void) //2msִ��һ��
{
	//////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////
}

static void Loop_200Hz(void) //5msִ��һ��
{
	//////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////
}

static void Loop_100Hz(void) //10msִ��һ��
{
	//////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////
}
int yaw_init=0;
u8 yaw_buchang=0;
static void Loop_50Hz(void) //20msִ��һ��
{
	//////////////////////////////////////////////////////////////////////
	//if(ano_of.intergral_x-base_link.pose_x_cm>30||ano_of.intergral_x-base_link.pose_x_cm<-30){
  //        error_land();
//	}
//	if(ano_of.intergral_y-base_link.pose_y_cm>30||ano_of.intergral_y-base_link.pose_y_cm<-30){
  //        error_land();
	//}
	/*if(fc_att.st_data.yaw_x100!=0&&yaw_init==0){
		yaw_init=1;
		my_yaw=(fc_att.st_data.yaw_x100/100);
		if(my_yaw<=-165||my_yaw>=165) yaw_buchang=1;
	}*/
	if(ok_flag==0||land==1||base_link.pose_x_cm>=1000||base_link.pose_y_cm>=1000||base_link.pose_x_cm<=-1000||base_link.pose_y_cm<=-1000){
		land=1;
	error_land();
	}
	else {
		if(ok_flag<40&&mode!=2)
		test2();//���·�
		else if(mode==2||ok_flag>=40){
			if(task2==-1){
				task2=ok_flag-40+1;
				mode=2;
			}
			if(task2==1||task2==4||task2==5)
			{
					rc_in.rc_ch.st_data.ch_[ch_5_aux1]=1500;
        	switch(task2_num)
	 				{
						case 0:
						{
							
							//reset
							//rc_in.rc_ch.st_data.ch_[ch_5_aux1]=1500;//����ģʽ
							rc_in.rc_ch.st_data.ch_[ch_5_aux1]=1500;
							PID_init();	
              //TIM_SetCompare4(TIM4,140);							
							task2_num=1;
							
						}
						break;
						case 1://����
						{
							
							if(time2<1000)//20s  50=1s
							{
								time2++;
							}
							else if(time2>=1000&&ok_flag==3)
							{
								time2 = 0;
								task2_num=2;
								FC_Unlock();
							}
						}
						break;
						case 2://���
						{
							if(time2<100){  //2s
							time2++;
							}
							else{
							time2=0;
							task2_num=3;
							OneKey_Takeoff(145);
							}
						}
						break;
						case 3://��1��
						{
							if(time2<100)//������ʱ
							{
								time2++;
							}
							else
							{
								time2 = 0;
								task2_num=4;
							}
						}
						break;
						case 4:
						{
							if(time2<600){  //10s
								time2++;
							x=xyPos_set(base_link.pose_x_cm,-270);
							y=xyPos_set(base_link.pose_y_cm,0);
							z=height_set(ano_of.of_alt_cm,145);
						//	yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
							tar_setdata(x,y, z,yaw);
								if(task2==1){
									if(base_link.pose_x_cm<=-60&&base_link.pose_x_cm>=-80)
									 led_flag=1;
								}
								else if(task2==4){
									if(base_link.pose_x_cm<=-110&&base_link.pose_x_cm>=-130)
									 led_flag=1;
								}
								else if(task2==5){
									if(base_link.pose_x_cm<=-160&&base_link.pose_x_cm>=-180)
									 led_flag=1;
								}
							}
							else{
								tar_setdata(0,0,0,0);
								led_flag=1;
								task2_num=5;
								time2 = 0;
							}
						}	
						break;
						case 5:
						{
							if(time2<700){  //14s
								time2++;
							x=xyPos_set(base_link.pose_x_cm,-270);
							y=xyPos_set(base_link.pose_y_cm,350);
							z=height_set(ano_of.of_alt_cm,145);
						//	yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
							tar_setdata(x,y, z,yaw);

							}
							else{
								tar_setdata(0,0,0,0);
								task2_num=6;
								time2 = 0;
							}
						}	
						break;
					case 6:
						{
							if(time2<100){  //2s
								time2++;
							x=xyPos_set(base_link.pose_x_cm,-250);
							y=xyPos_set(base_link.pose_y_cm,350);
							z=height_set(ano_of.of_alt_cm,145);
						//	yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
							tar_setdata(x,y, z,yaw);

							}
							else{
								tar_setdata(0,0,0,0);
								task2_num=7;
								time2 = 0;
							}
						}	
						break;
					case 7:
						{
							if(time2<150){  //3s
								time2++;
							x=xyPos_set(base_link.pose_x_cm,-250);
							y=xyPos_set(base_link.pose_y_cm,350);
							z=height_set(ano_of.of_alt_cm,50);
						//	yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
							tar_setdata(x,y, z,yaw);

							}
							else{
								tar_setdata(0,0,0,0);
								task2_num=8;
								time2 = 0;
							}
						}	
						break;
					  case 8:
						{
							if(time2<150){  //3s
								time2++;
							x=xyPos_set(base_link.pose_x_cm,-250);
							y=xyPos_set(base_link.pose_y_cm,350);
							z=height_set(ano_of.of_alt_cm,50);
						//	yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
							tar_setdata(x,y, z,yaw);

							}
							else{
								tar_setdata(0,0,0,0);
								task2_num=9;
								time2 = 0;
							}
						}	
						break;
						case 9://����
						{
							OneKey_Land();
							task2_num=10;
						}
						break;
						case 10://5s
						{
							if(time2<land_time)
							{
								time2++;
							}
							else
							{
								task2_num=11;
								time2 = 0;
							}
						}
						break;
						case 11:
						{
							FC_Lock();
							task_num=12;
						}
						break;
			}
			
		}
			
		else if(task2==2||task2==3||task2==6){
								rc_in.rc_ch.st_data.ch_[ch_5_aux1]=1500;
        	switch(task2_num)
	 				{
						case 0:
						{
							
							//reset
							//rc_in.rc_ch.st_data.ch_[ch_5_aux1]=1500;//����ģʽ
							rc_in.rc_ch.st_data.ch_[ch_5_aux1]=1500;
							PID_init();	
              //TIM_SetCompare4(TIM4,140);							
							task2_num=1;
							
						}
						break;
						case 1://����
						{
							
							if(time2<1000)//20s  50=1s
							{
								time2++;
							}
							else if(time2>=1000&&ok_flag==3)
							{
								time2 = 0;
								task2_num=2;
								FC_Unlock();
							}
						}
						break;
						case 2://���
						{
							if(time2<100){  //2s
							time2++;
							}
							else{
							time2=0;
							task2_num=3;
							OneKey_Takeoff(100);
							}
						}
						break;
						case 3://��1��
						{
							if(time2<100)//������ʱ
							{
								time2++;
							}
							else
							{
								time2 = 0;
								task2_num=4;
							}
						}
						break;
						case 4:
						{
							if(time2<600){  //12s
								time2++;
							x=xyPos_set(base_link.pose_x_cm,-270);
							y=xyPos_set(base_link.pose_y_cm,0);
							z=height_set(ano_of.of_alt_cm,100);
						//	yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
							tar_setdata(x,y, z,yaw);
								if(task2==1){
									if(base_link.pose_x_cm<=-60&&base_link.pose_x_cm>=-80)
									 led_flag=1;
								}
								else if(task2==4){
									if(base_link.pose_x_cm<=-110&&base_link.pose_x_cm>=-130)
									 led_flag=1;
								}
								else if(task2==5){
									if(base_link.pose_x_cm<=-160&&base_link.pose_x_cm>=-180)
									 led_flag=1;
								}
							}
							else{
								tar_setdata(0,0,0,0);
								led_flag=1;
								task2_num=5;
								time2 = 0;
							}
						}	
						break;
						case 5:
						{
							if(time2<700){  //14s
								time2++;
							x=xyPos_set(base_link.pose_x_cm,-270);
							y=xyPos_set(base_link.pose_y_cm,350);
							z=height_set(ano_of.of_alt_cm,100);
						//	yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
							tar_setdata(x,y, z,yaw);

							}
							else{
								tar_setdata(0,0,0,0);
								task2_num=6;
								time2 = 0;
							}
						}	
						break;
					case 6:
						{
							if(time2<100){  //2s
								time2++;
							x=xyPos_set(base_link.pose_x_cm,-250);
							y=xyPos_set(base_link.pose_y_cm,350);
							z=height_set(ano_of.of_alt_cm,100);
						//	yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
							tar_setdata(x,y, z,yaw);

							}
							else{
								tar_setdata(0,0,0,0);
								task2_num=7;
								time2 = 0;
							}
						}	
						break;
					case 7:
						{
							if(time2<150){  //3s
								time2++;
							x=xyPos_set(base_link.pose_x_cm,-250);
							y=xyPos_set(base_link.pose_y_cm,350);
							z=height_set(ano_of.of_alt_cm,50);
						//	yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
							tar_setdata(x,y, z,yaw);

							}
							else{
								tar_setdata(0,0,0,0);
								task2_num=8;
								time2 = 0;
							}
						}	
						break;
						case 8://����
						{
							OneKey_Land();
							task2_num=9;
						}
						break;
						case 9://5s
						{
							if(time2<land_time)
							{
								time2++;
							}
							else
							{
								task2_num=10;
								time2 = 0;
							}
						}
						break;
						case 10:
						{
							FC_Lock();
							task_num=11;
						}
						break;
			}
		}
				else if(task2==22||task2==23||task2==24){
						rc_in.rc_ch.st_data.ch_[ch_5_aux1]=1500;
        	switch(task2_num)
	 				{
						case 0:
						{
							
							//reset
							//rc_in.rc_ch.st_data.ch_[ch_5_aux1]=1500;//����ģʽ
							rc_in.rc_ch.st_data.ch_[ch_5_aux1]=1500;
							PID_init();	
              //TIM_SetCompare4(TIM4,140);							
							task2_num=1;
							
						}
						break;
						case 1://����
						{
							
							if(time2<1000)//20s  50=1s
							{
								time2++;
							}
							else if(time2>=1000&&ok_flag==3)
							{
								time2 = 0;
								task2_num=2;
								FC_Unlock();
							}
						}
						break;
						case 2://���
						{
							if(time2<100){  //2s
							time2++;
							}
							else{
							time2=0;
							task2_num=3;
							OneKey_Takeoff(140);
							}
						}
						break;
						case 3://��1��
						{
							if(time2<100)//������ʱ
							{
								time2++;
							}
							else
							{
								time2 = 0;
								task2_num=4;
							}
						}
						break;
						case 4:
						{
							if(time2<100){  //2s
								time2++;
							x=xyPos_set(base_link.pose_x_cm,40);
							y=xyPos_set(base_link.pose_y_cm,0);
							z=height_set(ano_of.of_alt_cm,140);
						//	yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
							tar_setdata(x,y, z,yaw);
								if(task2==1){
									if(base_link.pose_x_cm<=-70&&base_link.pose_x_cm>=-80)
									 led_flag=1;
								}
								else if(task2==4){
									if(base_link.pose_x_cm<=-120&&base_link.pose_x_cm>=-130)
									 led_flag=1;
								}
								else if(task2==5){
									if(base_link.pose_x_cm<=-170&&base_link.pose_x_cm>=-180)
									 led_flag=1;
								}
							}
							else{
								tar_setdata(0,0,0,0);
								led_flag=1;
								task2_num=5;
								time2 = 0;
							}
						}	
						break;
						case 5:
						{
							if(time2<650){  //13s
								time2++;
							x=xyPos_set(base_link.pose_x_cm,40);
							y=xyPos_set(base_link.pose_y_cm,350);
							z=height_set(ano_of.of_alt_cm,140);
						//	yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
							tar_setdata(x,y, z,yaw);
								if(task2==1){
									if(base_link.pose_x_cm<=-70&&base_link.pose_x_cm>=-80)
									 led_flag=1;
								}
								else if(task2==4){
									if(base_link.pose_x_cm<=-120&&base_link.pose_x_cm>=-130)
									 led_flag=1;
								}
								else if(task2==5){
									if(base_link.pose_x_cm<=-170&&base_link.pose_x_cm>=-180)
									 led_flag=1;
								}
							}
							else{
								tar_setdata(0,0,0,0);
								led_flag=1;
								task2_num=6;
								time2 = 0;
							}
						}	
						break;
						case 6:
						{
							if(time2<500){  //10s
								time2++;
							x=xyPos_set(base_link.pose_x_cm,-250);
							y=xyPos_set(base_link.pose_y_cm,350);
							z=height_set(ano_of.of_alt_cm,140);
						//	yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
							tar_setdata(x,y, z,yaw);
										if(task2==24){
									if(base_link.pose_x_cm<=-60&&base_link.pose_x_cm>=-80)
									 led_flag=8;
								}
								else if(task2==23){
									if(base_link.pose_x_cm<=-110&&base_link.pose_x_cm>=-130)
									 led_flag=8;
								}
								else if(task2==22){
									if(base_link.pose_x_cm<=-160&&base_link.pose_x_cm>=-180)
									 led_flag=8;
								}

							}
							else{
								tar_setdata(0,0,0,0);
								task2_num=7;
								time2 = 0;
							}
						}	
						break;
					case 7:
						{
							if(time2<100){  //2s
								time2++;
							x=xyPos_set(base_link.pose_x_cm,-250);
							y=xyPos_set(base_link.pose_y_cm,350);
							z=height_set(ano_of.of_alt_cm,50);
						//	yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
							tar_setdata(x,y, z,yaw);

							}
							else{
								tar_setdata(0,0,0,0);
								task2_num=8;
								time2 = 0;
							}
						}	
						break;
						case 8://����
						{
							OneKey_Land();
							task2_num=9;
						}
						break;
						case 9://5s
						{
							if(time2<land_time)
							{
								time2++;
							}
							else
							{
								task2_num=10;
								time2 = 0;
							}
						}
						break;
						case 10:
						{
							FC_Lock();
							task_num=11;
						}
						break;
			}
		}
				else if(task2==19||task2==20||task2==21){
						rc_in.rc_ch.st_data.ch_[ch_5_aux1]=1500;
        	switch(task2_num)
	 				{
						case 0:
						{
							
							//reset
							//rc_in.rc_ch.st_data.ch_[ch_5_aux1]=1500;//����ģʽ
							rc_in.rc_ch.st_data.ch_[ch_5_aux1]=1500;
							PID_init();	
              //TIM_SetCompare4(TIM4,140);							
							task2_num=1;
							
						}
						break;
						case 1://����
						{
							
							if(time2<1000)//20s  50=1s
							{
								time2++;
							}
							else if(time2>=1000&&ok_flag==3)
							{
								time2 = 0;
								task2_num=2;
								FC_Unlock();
							}
						}
						break;
						case 2://���
						{
							if(time2<100){  //2s
							time2++;
							}
							else{
							time2=0;
							task2_num=3;
							OneKey_Takeoff(100);
							}
						}
						break;
						case 3://��1��
						{
							if(time2<100)//������ʱ
							{
								time2++;
							}
							else
							{
								time2 = 0;
								task2_num=4;
							}
						}
						break;
						case 4:
						{
							if(time2<100){  //2s
								time2++;
							x=xyPos_set(base_link.pose_x_cm,40);
							y=xyPos_set(base_link.pose_y_cm,0);
							z=height_set(ano_of.of_alt_cm,100);
						//	yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
							tar_setdata(x,y, z,yaw);

							}
							else{
								tar_setdata(0,0,0,0);
								led_flag=1;
								task2_num=5;
								time2 = 0;
							}
						}	
						break;
						case 5:
						{
							if(time2<650){  //13s
								time2++;
							x=xyPos_set(base_link.pose_x_cm,40);
							y=xyPos_set(base_link.pose_y_cm,350);
							z=height_set(ano_of.of_alt_cm,100);
						//	yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
							tar_setdata(x,y, z,yaw);

							}
							else{
								tar_setdata(0,0,0,0);
								led_flag=1;
								task2_num=6;
								time2 = 0;
							}
						}	
						break;
						case 6:
						{
							if(time2<500){  //10s
								time2++;
							x=xyPos_set(base_link.pose_x_cm,-250);
							y=xyPos_set(base_link.pose_y_cm,350);
							z=height_set(ano_of.of_alt_cm,100);
						//	yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
							tar_setdata(x,y, z,yaw);
								if(task2==21){
									if(base_link.pose_x_cm<=-60&&base_link.pose_x_cm>=-80)
									 led_flag=8;
								}
								else if(task2==20){
									if(base_link.pose_x_cm<=-110&&base_link.pose_x_cm>=-130)
									 led_flag=8;
								}
								else if(task2==19){
									if(base_link.pose_x_cm<=-160&&base_link.pose_x_cm>=-180)
									 led_flag=8;
								}
							}
							else{
								tar_setdata(0,0,0,0);
								task2_num=7;
								time2 = 0;
							}
						}	
						break;
					case 7:
						{
							if(time2<100){  //2s
								time2++;
							x=xyPos_set(base_link.pose_x_cm,-250);
							y=xyPos_set(base_link.pose_y_cm,350);
							z=height_set(ano_of.of_alt_cm,50);
						//	yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
							tar_setdata(x,y, z,yaw);

							}
							else{
								tar_setdata(0,0,0,0);
								task2_num=8;
								time2 = 0;
							}
						}	
						break;
						case 8://����
						{
							OneKey_Land();
							task2_num=9;
						}
						break;
						case 9://5s
						{
							if(time2<land_time)
							{
								time2++;
							}
							else
							{
								task2_num=10;
								time2 = 0;
							}
						}
						break;
						case 10:
						{
							FC_Lock();
							task_num=11;
						}
						break;
			}
		}
			
		
		else if(task2==8||task2==9||task2==12||task2==13||task2==16||task2==17){
								rc_in.rc_ch.st_data.ch_[ch_5_aux1]=1500;
        	switch(task2_num)
	 				{
						case 0:
						{
							
							//reset
							//rc_in.rc_ch.st_data.ch_[ch_5_aux1]=1500;//����ģʽ
							rc_in.rc_ch.st_data.ch_[ch_5_aux1]=1500;
							PID_init();	
              //TIM_SetCompare4(TIM4,140);							
							task2_num=1;
							
						}
						break;
						case 1://����
						{
							
							if(time2<1000)//20s  50=1s
							{
								time2++;
							}
							else if(time2>=1000&&ok_flag==3)
							{
								time2 = 0;
								task2_num=2;
								FC_Unlock();
							}
						}
						break;
						case 2://���
						{
							if(time2<100){  //2s
							time2++;
							}
							else{
							time2=0;
							task2_num=3;
							OneKey_Takeoff(145);
							}
						}
						break;
						case 3://��1��
						{
							if(time2<100)//������ʱ
							{
								time2++;
							}
							else
							{
								time2 = 0;
								task2_num=4;
							}
						}
						break;
						case 4:
						{
							if(time2<100){  //2s
								time2++;
							x=xyPos_set(base_link.pose_x_cm,40);
							y=xyPos_set(base_link.pose_y_cm,0);
							z=height_set(ano_of.of_alt_cm,145);
						//	yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
							tar_setdata(x,y, z,yaw);
							/*	if(task2==1){
									if(base_link.pose_x_cm<=-70&&base_link.pose_x_cm>=-80)
									 led_flag=1;
								}
								else if(task2==4){
									if(base_link.pose_x_cm<=-120&&base_link.pose_x_cm>=-130)
									 led_flag=1;
								}
								else if(task2==5){
									if(base_link.pose_x_cm<=-170&&base_link.pose_x_cm>=-180)
									 led_flag=1;
								}*/
							}
							else{
								tar_setdata(0,0,0,0);
								led_flag=1;
								task2_num=5;
								time2 = 0;
							}
						}	
						break;
						case 5:
						{
							if(time2<350){  //7s
								time2++;
							x=xyPos_set(base_link.pose_x_cm,40);
							y=xyPos_set(base_link.pose_y_cm,175);
							z=height_set(ano_of.of_alt_cm,145);
						//	yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
							tar_setdata(x,y, z,yaw);
							/*	if(task2==1){
									if(base_link.pose_x_cm<=-70&&base_link.pose_x_cm>=-80)
									 led_flag=1;
								}
								else if(task2==4){
									if(base_link.pose_x_cm<=-120&&base_link.pose_x_cm>=-130)
									 led_flag=1;
								}
								else if(task2==5){
									if(base_link.pose_x_cm<=-170&&base_link.pose_x_cm>=-180)
									 led_flag=1;
								}*/
							}
							else{
								tar_setdata(0,0,0,0);
								led_flag=1;
								task2_num=6;
								time2 = 0;
							}
						}	
						break;
						case 6:
						{
							if(time2<500){  //10s
								time2++;
							x=xyPos_set(base_link.pose_x_cm,-270);
							y=xyPos_set(base_link.pose_y_cm,175);
							z=height_set(ano_of.of_alt_cm,145);
						//	yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
							tar_setdata(x,y, z,yaw);
										if(task2==12){
									if(base_link.pose_x_cm<=-60&&base_link.pose_x_cm>=-80)
									 led_flag=8;
								}
								else if(task2==9){
									if(base_link.pose_x_cm<=-110&&base_link.pose_x_cm>=-130)
									 led_flag=8;
								}
								else if(task2==8){
									if(base_link.pose_x_cm<=-160&&base_link.pose_x_cm>=-180)
									 led_flag=8;
								}
								else if(task2==13){
									if(base_link.pose_x_cm<=-60&&base_link.pose_x_cm>=-80)
									 led_flag=5;
								}
								else if(task2==16){
									if(base_link.pose_x_cm<=-110&&base_link.pose_x_cm>=-130)
									 led_flag=5;
								}
								else if(task2==17){
									if(base_link.pose_x_cm<=-160&&base_link.pose_x_cm>=-180)
									 led_flag=5;
								}
							}
							else{
								tar_setdata(0,0,0,0);
								task2_num=7;
								time2 = 0;
							}
						}	
						break;
					case 7:
						{
							if(time2<350){  //7s
								time2++;
							x=xyPos_set(base_link.pose_x_cm,-270);
							y=xyPos_set(base_link.pose_y_cm,350);
							z=height_set(ano_of.of_alt_cm,145);
						//	yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
							tar_setdata(x,y, z,yaw);

							}
							else{
								tar_setdata(0,0,0,0);
								task2_num=8;
								time2 = 0;
							}
						}	
						break;
					case 8:
						{
							if(time2<150){  //3s
								time2++;
							x=xyPos_set(base_link.pose_x_cm,-250);
							y=xyPos_set(base_link.pose_y_cm,350);
							z=height_set(ano_of.of_alt_cm,50);
						//	yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
							tar_setdata(x,y, z,yaw);

							}
							else{
								tar_setdata(0,0,0,0);
								task2_num=9;
								time2 = 0;
							}
						}	
						break;
					  case 9:
						{
							if(time2<150){  //3s
								time2++;
							x=xyPos_set(base_link.pose_x_cm,-250);
							y=xyPos_set(base_link.pose_y_cm,350);
							z=height_set(ano_of.of_alt_cm,50);
						//	yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
							tar_setdata(x,y, z,yaw);

							}
							else{
								tar_setdata(0,0,0,0);
								task2_num=10;
								time2 = 0;
							}
						}	
						break;
						case 10://����
						{
							OneKey_Land();
							task2_num=11;
						}
						break;
						case 11://5s
						{
							if(time2<land_time)
							{
								time2++;
							}
							else
							{
								task2_num=12;
								time2 = 0;
							}
						}
						break;
						case 12:
						{
							FC_Lock();
							task_num=13;
						}
						break;
			}
		}
		
		
		
		
		else if(task2==7||task2==10||task2==11||task2==14||task2==15||task2==18){
								rc_in.rc_ch.st_data.ch_[ch_5_aux1]=1500;
        	switch(task2_num)
	 				{
						case 0:
						{
							
							//reset
							//rc_in.rc_ch.st_data.ch_[ch_5_aux1]=1500;//����ģʽ
							rc_in.rc_ch.st_data.ch_[ch_5_aux1]=1500;
							PID_init();	
              //TIM_SetCompare4(TIM4,140);							
							task2_num=1;
							
						}
						break;
						case 1://����
						{
							
							if(time2<1000)//20s  50=1s
							{
								time2++;
							}
							else if(time2>=1000&&ok_flag==3)
							{
								time2 = 0;
								task2_num=2;
								FC_Unlock();
							}
						}
						break;
						case 2://���
						{
							if(time2<100){  //2s
							time2++;
							}
							else{
							time2=0;
							task2_num=3;
							OneKey_Takeoff(100);
							}
						}
						break;
						case 3://��1��
						{
							if(time2<100)//������ʱ
							{
								time2++;
							}
							else
							{
								time2 = 0;
								task2_num=4;
							}
						}
						break;
						case 4:
						{
							if(time2<100){  //2s
								time2++;
							x=xyPos_set(base_link.pose_x_cm,40);
							y=xyPos_set(base_link.pose_y_cm,0);
							z=height_set(ano_of.of_alt_cm,100);
						//	yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
							tar_setdata(x,y, z,yaw);
							/*	if(task2==1){
									if(base_link.pose_x_cm<=-70&&base_link.pose_x_cm>=-80)
									 led_flag=1;
								}
								else if(task2==4){
									if(base_link.pose_x_cm<=-120&&base_link.pose_x_cm>=-130)
									 led_flag=1;
								}
								else if(task2==5){
									if(base_link.pose_x_cm<=-170&&base_link.pose_x_cm>=-180)
									 led_flag=1;
								}*/
							}
							else{
								tar_setdata(0,0,0,0);
								led_flag=0;
								task2_num=5;
								time2 = 0;
							}
						}	
						break;
						case 5:
						{
							if(time2<350){  //7s
								time2++;
							x=xyPos_set(base_link.pose_x_cm,40);
							y=xyPos_set(base_link.pose_y_cm,175);
							z=height_set(ano_of.of_alt_cm,100);
						//	yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
							tar_setdata(x,y, z,yaw);
							/*	if(task2==1){
									if(base_link.pose_x_cm<=-70&&base_link.pose_x_cm>=-80)
									 led_flag=1;
								}
								else if(task2==4){
									if(base_link.pose_x_cm<=-120&&base_link.pose_x_cm>=-130)
									 led_flag=1;
								}
								else if(task2==5){
									if(base_link.pose_x_cm<=-170&&base_link.pose_x_cm>=-180)
									 led_flag=1;
								}*/
							}
							else{
								tar_setdata(0,0,0,0);
								led_flag=0;
								task2_num=6;
								time2 = 0;
							}
						}	
						break;
						case 6:
						{
							if(time2<500){  //10s
								time2++;
							x=xyPos_set(base_link.pose_x_cm,-270);
							y=xyPos_set(base_link.pose_y_cm,175);
							z=height_set(ano_of.of_alt_cm,100);
						//	yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
							tar_setdata(x,y, z,yaw);
										if(task2==7){
									if(base_link.pose_x_cm<=-60&&base_link.pose_x_cm>=-80)
									 led_flag=8;
								}
								else if(task2==10){
									if(base_link.pose_x_cm<=-110&&base_link.pose_x_cm>=-130)
									 led_flag=8;
								}
								else if(task2==11){
									if(base_link.pose_x_cm<=-160&&base_link.pose_x_cm>=-180)
									 led_flag=8;
								}
								else if(task2==14){
									if(base_link.pose_x_cm<=-60&&base_link.pose_x_cm>=-80)
									 led_flag=5;
								}
								else if(task2==15){
									if(base_link.pose_x_cm<=-110&&base_link.pose_x_cm>=-130)
									 led_flag=5;
								}
								else if(task2==18){
									if(base_link.pose_x_cm<=-160&&base_link.pose_x_cm>=-180)
									 led_flag=5;
								}
							}
							else{
								tar_setdata(0,0,0,0);
								task2_num=7;
								time2 = 0;
							}
						}	
						break;
					case 7:
						{
							if(time2<350){  //7s
								time2++;
							x=xyPos_set(base_link.pose_x_cm,-270);
							y=xyPos_set(base_link.pose_y_cm,350);
							z=height_set(ano_of.of_alt_cm,100);
						//	yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
							tar_setdata(x,y, z,yaw);

							}
							else{
								tar_setdata(0,0,0,0);
								task2_num=7;
								time2 = 0;
							}
						}	
						break;
					case 8:
						{
							if(time2<150){  //3s
								time2++;
							x=xyPos_set(base_link.pose_x_cm,-250);
							y=xyPos_set(base_link.pose_y_cm,350);
							z=height_set(ano_of.of_alt_cm,50);
						//	yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
							tar_setdata(x,y, z,yaw);

							}
							else{
								tar_setdata(0,0,0,0);
								task2_num=9;
								time2 = 0;
							}
						}	
						break;
					  case 9:
						{
							if(time2<150){  //3s
								time2++;
							x=xyPos_set(base_link.pose_x_cm,-250);
							y=xyPos_set(base_link.pose_y_cm,350);
							z=height_set(ano_of.of_alt_cm,50);
						//	yaw=Yaw_set(fc_att.st_data.yaw_x100/100+360,my_yaw);
							tar_setdata(x,y, z,yaw);

							}
							else{
								tar_setdata(0,0,0,0);
								task2_num=10;
								time2 = 0;
							}
						}	
						break;
						case 10://����
						{
							OneKey_Land();
							task2_num=11;
						}
						break;
						case 11://5s
						{
							if(time2<land_time)
							{
								time2++;
							}
							else
							{
								task2_num=12;
								time2 = 0;
							}
						}
						break;
						case 12:
						{
							FC_Lock();
							task_num=13;
						}
						break;
			}
		}
		//test1();//ֱ�ӷ��ٵ�ͷ
	//	jiguang_test();
		}
	
	//else if(fire_flag==0&&(fire_task==0||fire_task==2))
//	test();
//	else if(fire_flag==1||fire_task==1)
//	fire_find();
	

					
	//////////////////////////////////////////////////////////////////////

}
}

static void Loop_20Hz(void) //50msִ��һ��
{
	//////////////////////////////////////////////////////////////////////
	lanya_send();

	//////////////////////////////////////////////////////////////////////
}

static void Loop_2Hz(void) //500msִ��һ��
{
	//rc_in.rc_ch.st_data.ch_[ch_5_aux1]=1500;
	//lanya_send();
	//	TIM_SetCompare4(TIM3,150);
//	TIM_SetCompare1(TIM4,5);
	
}
//////////////////////////////////////////////////////////////////////
//��������ʼ��
//////////////////////////////////////////////////////////////////////
//ϵͳ�������ã�������ִͬ��Ƶ�ʵġ��̡߳�
static sched_task_t sched_tasks[] =
	{
		{Loop_1000Hz, 1000, 0, 0},
		{Loop_500Hz, 500, 0, 0},
		{Loop_200Hz, 200, 0, 0},
		{Loop_100Hz, 100, 0, 0},
		{Loop_50Hz, 50, 0, 0},
		{Loop_20Hz, 20, 0, 0},
		{Loop_2Hz, 2, 0, 0},
};
//�������鳤�ȣ��ж��߳�����
#define TASK_NUM (sizeof(sched_tasks) / sizeof(sched_task_t))

void Scheduler_Setup(void)
{
	uint8_t index = 0;
	//��ʼ�������
	for (index = 0; index < TASK_NUM; index++)
	{
		//����ÿ���������ʱ������
		sched_tasks[index].interval_ticks = TICK_PER_SECOND / sched_tasks[index].rate_hz;
		//�������Ϊ1��Ҳ����1ms
		if (sched_tasks[index].interval_ticks < 1)
		{
			sched_tasks[index].interval_ticks = 1;
		}
	}
}
//��������ŵ�main������while(1)�У���ͣ�ж��Ƿ����߳�Ӧ��ִ��
void Scheduler_Run(void)
{
	uint8_t index = 0;
	//ѭ���ж������̣߳��Ƿ�Ӧ��ִ��

	for (index = 0; index < TASK_NUM; index++)
	{
		//��ȡϵͳ��ǰʱ�䣬��λMS
		uint32_t tnow = GetSysRunTimeMs();
		//�����жϣ������ǰʱ���ȥ��һ��ִ�е�ʱ�䣬���ڵ��ڸ��̵߳�ִ�����ڣ���ִ���߳�
		if (tnow - sched_tasks[index].last_run >= sched_tasks[index].interval_ticks)
		{

			//�����̵߳�ִ��ʱ�䣬������һ���ж�
			sched_tasks[index].last_run = tnow;
			//ִ���̺߳�����ʹ�õ��Ǻ���ָ��
			sched_tasks[index].task_func();
		}
	}
}

/******************* (C) COPYRIGHT 2014 ANO TECH *****END OF FILE************/

