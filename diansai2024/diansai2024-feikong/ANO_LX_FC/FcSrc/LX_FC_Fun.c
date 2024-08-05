#include "LX_FC_Fun.h"
#include "LX_FC_State.h"
#include "ANO_DT_LX.h"
#include "Ano_Scheduler.h"
/*==========================================================================
 * 描述    ：凌霄飞控基本功能主程序
 * 更新时间：2020-03-31 
 * 作者		 ：匿名科创-Jyoun
 * 官网    ：www.anotc.com
 * 淘宝    ：anotc.taobao.com
 * 技术Q群 ：190169595
 * 项目合作：18084888982，18061373080
============================================================================
 * 匿名科创团队感谢大家的支持，欢迎大家进群互相交流、讨论、学习。
 * 若您觉得匿名有不好的地方，欢迎您拍砖提意见。
 * 若您觉得匿名好，请多多帮我们推荐，支持我们。
 * 匿名开源程序代码欢迎您的引用、延伸和拓展，不过在希望您在使用时能注明出处。
 * 君子坦荡荡，小人常戚戚，匿名坚决不会请水军、请喷子，也从未有过抹黑同行的行为。  
 * 开源不易，生活更不容易，希望大家互相尊重、互帮互助，共同进步。
 * 只有您的支持，匿名才能做得更好。  
===========================================================================*/
s16 Vx,Vy,Vz,Yaw;
//////////////////////////////////////////////////////////////////////
//基本功能函数
//////////////////////////////////////////////////////////////////////
//
u8 FC_Unlock()
{
	//
	fc_sta.unlock_cmd = 1; //解锁
	//按协议发送指令
	if (dt.wait_ck == 0) //没有其他等待校验的CMD时才发送本CMD
	{
		dt.cmd_send.CID = 0x10;
		dt.cmd_send.CMD[0] = 0x00;
		dt.cmd_send.CMD[1] = 0x01;
		CMD_Send(0XFF, &dt.cmd_send);
		return 1;
	}
	else
	{
		return 0;
	}
}
//
u8 FC_Lock()
{
	//
	fc_sta.unlock_cmd = 0; //上锁
	//按协议发送指令
	if (dt.wait_ck == 0) //没有其他等待校验的CMD时才发送本CMD
	{
		dt.cmd_send.CID = 0x10;
		dt.cmd_send.CMD[0] = 0x00;
		dt.cmd_send.CMD[1] = 0x02;
		CMD_Send(0XFF, &dt.cmd_send);
		return 1;
	}
	else
	{
		return 0;
	}
}

//改变飞控模式(模式0-3)
u8 LX_Change_Mode(u8 new_mode)
{
	static u8 old_mode;
	if (old_mode != new_mode)
	{
		//
		if (dt.wait_ck == 0) //没有其他等待校验的CMD时才发送本CMD
		{
			old_mode = fc_sta.fc_mode_cmd = new_mode;
			//按协议发送指令
			dt.cmd_send.CID = 0X01;
			dt.cmd_send.CMD[0] = 0X01;
			dt.cmd_send.CMD[1] = 0X01;
			dt.cmd_send.CMD[2] = fc_sta.fc_mode_cmd;
			CMD_Send(0xff, &dt.cmd_send);
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else //已经在当前模式
	{
		return 1;
	}
}

//一键返航
//需要注意，程控模式下才能执行返航
u8 OneKey_Return_Home()
{
	//
	if (dt.wait_ck == 0) //没有其他等待校验的CMD时才发送本CMD
	{
		//按协议发送指令
		dt.cmd_send.CID = 0X10;
		dt.cmd_send.CMD[0] = 0X00;
		dt.cmd_send.CMD[1] = 0X07;
		CMD_Send(0xff, &dt.cmd_send);
		return 1;
	}
	else
	{
		return 0;
	}
}

//一键起飞(高度cm)
u8 OneKey_Takeoff(u16 height_cm)
{
	//
	if (dt.wait_ck == 0) //没有其他等待校验的CMD时才发送本CMD
	{
		//按协议发送指令
		dt.cmd_send.CID = 0X10;
		dt.cmd_send.CMD[0] = 0X00;
		dt.cmd_send.CMD[1] = 0X05;
		dt.cmd_send.CMD[2] = BYTE0(height_cm);
		dt.cmd_send.CMD[3] = BYTE1(height_cm);
		CMD_Send(0xff, &dt.cmd_send);
		return 1;
	}
	else
	{
		return 0;
	}
}
//一键降落
u8 OneKey_Land()
{
	//
	if (dt.wait_ck == 0) //没有其他等待校验的CMD时才发送本CMD
	{
		//按协议发送指令
		dt.cmd_send.CID = 0X10;
		dt.cmd_send.CMD[0] = 0X00;
		dt.cmd_send.CMD[1] = 0X06;
		CMD_Send(0xff, &dt.cmd_send);
		return 1;
	}
	else
	{
		return 0;
	}
}
//平移(距离cm，速度cmps，方向角度0-360度)
u8 Horizontal_Move(u16 distance_cm, u16 velocity_cmps, u16 dir_angle_0_360)
{
	//
	if (dt.wait_ck == 0) //没有其他等待校验的CMD时才发送本CMD
	{
		//按协议发送指令
		dt.cmd_send.CID = 0X10;
		dt.cmd_send.CMD[0] = 0X02;
		dt.cmd_send.CMD[1] = 0X03;
		//
		dt.cmd_send.CMD[2] = BYTE0(distance_cm);
		dt.cmd_send.CMD[3] = BYTE1(distance_cm);
		dt.cmd_send.CMD[4] = BYTE0(velocity_cmps);
		dt.cmd_send.CMD[5] = BYTE1(velocity_cmps);
		dt.cmd_send.CMD[6] = BYTE0(dir_angle_0_360);
		dt.cmd_send.CMD[7] = BYTE1(dir_angle_0_360);
		//
		CMD_Send(0xff, &dt.cmd_send);
		return 1;
	}
	else
	{
		return 0;
	}
}

u8 Angel_Move_Left(u16 deg,u16 deg_s){  //左自旋
	//
	if (dt.wait_ck == 0) //没有其他等待校验的CMD时才发送本CMD
	{
		//按协议发送指令
		dt.cmd_send.CID = 0X10;
		dt.cmd_send.CMD[0] = 0X02;
		dt.cmd_send.CMD[1] = 0X07;
		//
		dt.cmd_send.CMD[2] = BYTE0(deg);
		dt.cmd_send.CMD[3] = BYTE1(deg);
		dt.cmd_send.CMD[4] = BYTE0(deg_s);
		dt.cmd_send.CMD[5] = BYTE1(deg_s);

		//
		CMD_Send(0xff, &dt.cmd_send);
		return 1;
	}
	else
	{
		return 0;
	}
}

u8 Angel_Move_Right(u16 deg,u16 deg_s){  //右自旋   0-359 deg   5-90 deg/s
	//
	if (dt.wait_ck == 0) //没有其他等待校验的CMD时才发送本CMD
	{
		//按协议发送指令
		dt.cmd_send.CID = 0X10;
		dt.cmd_send.CMD[0] = 0X02;
		dt.cmd_send.CMD[1] = 0X08;
		//
		dt.cmd_send.CMD[2] = BYTE0(deg);
		dt.cmd_send.CMD[3] = BYTE1(deg);
		dt.cmd_send.CMD[4] = BYTE0(deg_s);
		dt.cmd_send.CMD[5] = BYTE1(deg_s);

		//
		CMD_Send(0xff, &dt.cmd_send);
		return 1;
	}
	else
	{
		return 0;
	}
}

//水平校准
u8 Horizontal_Calibrate()
{
	//
	if (dt.wait_ck == 0) //没有其他等待校验的CMD时才发送本CMD
	{
		//按协议发送指令
		dt.cmd_send.CID = 0X01;
		dt.cmd_send.CMD[0] = 0X00;
		dt.cmd_send.CMD[1] = 0X03;
		CMD_Send(0xff, &dt.cmd_send);
		return 1;
	}
	else
	{
		return 0;
	}
}

//磁力计校准
u8 Mag_Calibrate()
{
	//
	if (dt.wait_ck == 0) //没有其他等待校验的CMD时才发送本CMD
	{
		//按协议发送指令
		dt.cmd_send.CID = 0X01;
		dt.cmd_send.CMD[0] = 0X00;
		dt.cmd_send.CMD[1] = 0X04;
		CMD_Send(0xff, &dt.cmd_send);
		return 1;
	}
	else
	{
		return 0;
	}
}

//6面加速度校准
u8 ACC_Calibrate()
{
	//
	if (dt.wait_ck == 0) //没有其他等待校验的CMD时才发送本CMD
	{
		//按协议发送指令
		dt.cmd_send.CID = 0X01;
		dt.cmd_send.CMD[0] = 0X00;
		dt.cmd_send.CMD[1] = 0X05;
		CMD_Send(0xff, &dt.cmd_send);
		return 1;
	}
	else
	{
		return 0;
	}
}

//陀螺仪校准
u8 GYR_Calibrate()
{
	//
	if (dt.wait_ck == 0) //没有其他等待校验的CMD时才发送本CMD
	{
		//按协议发送指令
		dt.cmd_send.CID = 0X01;
		dt.cmd_send.CMD[0] = 0X00;
		dt.cmd_send.CMD[1] = 0X02;
		CMD_Send(0xff, &dt.cmd_send);
		return 1;
	}
	else
	{
		return 0;
	}
}

void tar_setdata(s16 x,s16 y, s16 z,s16 yaw)//实时控制帧发送速度
{
	
	Vx=x,Vy=y,Vz=z,Yaw=yaw;
	//	rt_tar.st_data.vel_x=x;//头向速度 单位cm/s
	//	rt_tar.st_data.vel_y=y;//左正右负
	//	rt_tar.st_data.vel_z=z;//上下速度
	//	rt_tar.st_data.yaw_dps=yaw;//逆时针为正 单位度/秒
	//	dt.fun[0x41].WTS = 1;
} 

u8 Height_Move(u16 high,u16 high_s){  //上升
	//
	if (dt.wait_ck == 0) //没有其他等待校验的CMD时才发送本CMD
	{
		//按协议发送指令
		dt.cmd_send.CID = 0X10;
		dt.cmd_send.CMD[0] = 0X02;
		dt.cmd_send.CMD[1] = 0X01;
		//
		dt.cmd_send.CMD[2] = BYTE0(high);
		dt.cmd_send.CMD[3] = BYTE1(high);
		dt.cmd_send.CMD[4] = BYTE0(high_s);
		dt.cmd_send.CMD[5] = BYTE1(high_s);

		//
		CMD_Send(0xff, &dt.cmd_send);
		return 1;
	}
	else
	{
		return 0;
	}
}

u8 Low_Move(u16 high,u16 high_s){  //下降
	//
	if (dt.wait_ck == 0) //没有其他等待校验的CMD时才发送本CMD
	{
		//按协议发送指令
		dt.cmd_send.CID = 0X10;
		dt.cmd_send.CMD[0] = 0X02;
		dt.cmd_send.CMD[1] = 0X02;
		//
		dt.cmd_send.CMD[2] = BYTE0(high);
		dt.cmd_send.CMD[3] = BYTE1(high);
		dt.cmd_send.CMD[4] = BYTE0(high_s);
		dt.cmd_send.CMD[5] = BYTE1(high_s);

		//
		CMD_Send(0xff, &dt.cmd_send);
		return 1;
	}
	else
	{
		return 0;
	}
}

u8 Yaw_get(){  //读取欧拉角
	//
	if (dt.wait_ck == 0) //没有其他等待校验的CMD时才发送本CMD
	{
		//按协议发送指令
		dt.cmd_send.CID = 0XAA;
		dt.cmd_send.CMD[0] = 0XE1;
		dt.cmd_send.CMD[1] = 0X03;

		//
		CMD_Send(0xff, &dt.cmd_send);
		return 1;
	}
	else
	{
		return 0;
	}
}
/*PID*/
struct PID_inc height_PID;
struct PID_inc xy_PID;
void PID_init(void)
{
	height_PID.p=1.9;
	height_PID.i=1.3;
	height_PID.d=0.7;
	height_PID.actual=0;
	height_PID.target=0;
	height_PID.err_current=0;
	height_PID.err_last=0;
	height_PID.err_previous=0;
	xy_PID.p=0.5;
	xy_PID.i=1.5;
	xy_PID.d=0.3;
	xy_PID.actual=0;
	xy_PID.target=0;
	xy_PID.err_current=0;
	xy_PID.err_last=0;
	xy_PID.err_previous=0;
}

s16 height_set(u32 height,u16 height_set)
{
	s16 output=0;
	height_PID.actual=height;
	height_PID.target=height_set; 
	height_PID.err_current=height_PID.target-height_PID.actual;
	output=height_PID.p*(height_PID.err_current-height_PID.err_last)+height_PID.i*height_PID.err_current+height_PID.d*(height_PID.err_current-2*height_PID.err_last+height_PID.err_previous);
	height_PID.err_previous=height_PID.err_last;
	height_PID.err_last=height_PID.err_current;
	if (output>20 ) output=20;
	else if(output<-20) output=-20;
	return output;
}
/*之后改*/
s16 xypid_set(s32 xy,s16 xy_set,u16 speedmax)
{
	s16 output=0;
	xy_PID.actual=xy;
	xy_PID.target=xy_set; 
	xy_PID.err_current=xy_PID.target-xy_PID.actual;
	output=xy_PID.p*(xy_PID.err_current-xy_PID.err_last)+xy_PID.i*xy_PID.err_current+xy_PID.d*(xy_PID.err_current-2*xy_PID.err_last+xy_PID.err_previous);
	xy_PID.err_previous=xy_PID.err_last;
	xy_PID.err_last=xy_PID.err_current;
	if (output>speedmax ) output=speedmax;
	else if(output<-speedmax) output=-speedmax;
	return output;
}

s16 xyPos_set(s32 xy,s16 xy_set){
	if(xy_set-xy>30) return 25;
	else if(xy_set-xy<-30) return -25;
	else if(xy_set-xy<=25&&xy_set-xy>4) return (0.8*(xy_set-xy));
	else if(xy_set-xy>=-25&&xy_set-xy<-4) return (0.8*(xy_set-xy));
	//else if(xy_set-xy<=15&&xy_set-xy>5) return 10;
	//else if(xy_set-xy<-5&&xy_set-xy>=-15) return -10;
	else return 0;
	
}

s16 Yaw_set(s16 yaw_val,s16 tar_yaw){
	if(tar_yaw-yaw_val>=4&&tar_yaw-yaw_val<100) return 2;
	else if(tar_yaw-yaw_val<=-4&&(tar_yaw-yaw_val>-100) )return -2;
	else if(tar_yaw-yaw_val>=100) return -2;
	else if(tar_yaw-yaw_val<=-100) return 2;


	else return 0;
	
}
