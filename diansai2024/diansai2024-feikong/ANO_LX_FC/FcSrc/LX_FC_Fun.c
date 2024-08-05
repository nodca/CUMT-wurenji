#include "LX_FC_Fun.h"
#include "LX_FC_State.h"
#include "ANO_DT_LX.h"
#include "Ano_Scheduler.h"
/*==========================================================================
 * ����    �������ɿػ�������������
 * ����ʱ�䣺2020-03-31 
 * ����		 �������ƴ�-Jyoun
 * ����    ��www.anotc.com
 * �Ա�    ��anotc.taobao.com
 * ����QȺ ��190169595
 * ��Ŀ������18084888982��18061373080
============================================================================
 * �����ƴ��ŶӸ�л��ҵ�֧�֣���ӭ��ҽ�Ⱥ���ཻ�������ۡ�ѧϰ��
 * �������������в��õĵط�����ӭ����ש�������
 * �������������ã�����������Ƽ���֧�����ǡ�
 * ������Դ������뻶ӭ�������á��������չ��������ϣ������ʹ��ʱ��ע��������
 * ����̹������С�˳����ݣ��������������ˮ���������ӣ�Ҳ��δ�й�Ĩ��ͬ�е���Ϊ��  
 * ��Դ���ף�����������ף�ϣ����һ������ء����ﻥ������ͬ������
 * ֻ������֧�֣������������ø��á�  
===========================================================================*/
s16 Vx,Vy,Vz,Yaw;
//////////////////////////////////////////////////////////////////////
//�������ܺ���
//////////////////////////////////////////////////////////////////////
//
u8 FC_Unlock()
{
	//
	fc_sta.unlock_cmd = 1; //����
	//��Э�鷢��ָ��
	if (dt.wait_ck == 0) //û�������ȴ�У���CMDʱ�ŷ��ͱ�CMD
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
	fc_sta.unlock_cmd = 0; //����
	//��Э�鷢��ָ��
	if (dt.wait_ck == 0) //û�������ȴ�У���CMDʱ�ŷ��ͱ�CMD
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

//�ı�ɿ�ģʽ(ģʽ0-3)
u8 LX_Change_Mode(u8 new_mode)
{
	static u8 old_mode;
	if (old_mode != new_mode)
	{
		//
		if (dt.wait_ck == 0) //û�������ȴ�У���CMDʱ�ŷ��ͱ�CMD
		{
			old_mode = fc_sta.fc_mode_cmd = new_mode;
			//��Э�鷢��ָ��
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
	else //�Ѿ��ڵ�ǰģʽ
	{
		return 1;
	}
}

//һ������
//��Ҫע�⣬�̿�ģʽ�²���ִ�з���
u8 OneKey_Return_Home()
{
	//
	if (dt.wait_ck == 0) //û�������ȴ�У���CMDʱ�ŷ��ͱ�CMD
	{
		//��Э�鷢��ָ��
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

//һ�����(�߶�cm)
u8 OneKey_Takeoff(u16 height_cm)
{
	//
	if (dt.wait_ck == 0) //û�������ȴ�У���CMDʱ�ŷ��ͱ�CMD
	{
		//��Э�鷢��ָ��
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
//һ������
u8 OneKey_Land()
{
	//
	if (dt.wait_ck == 0) //û�������ȴ�У���CMDʱ�ŷ��ͱ�CMD
	{
		//��Э�鷢��ָ��
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
//ƽ��(����cm���ٶ�cmps������Ƕ�0-360��)
u8 Horizontal_Move(u16 distance_cm, u16 velocity_cmps, u16 dir_angle_0_360)
{
	//
	if (dt.wait_ck == 0) //û�������ȴ�У���CMDʱ�ŷ��ͱ�CMD
	{
		//��Э�鷢��ָ��
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

u8 Angel_Move_Left(u16 deg,u16 deg_s){  //������
	//
	if (dt.wait_ck == 0) //û�������ȴ�У���CMDʱ�ŷ��ͱ�CMD
	{
		//��Э�鷢��ָ��
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

u8 Angel_Move_Right(u16 deg,u16 deg_s){  //������   0-359 deg   5-90 deg/s
	//
	if (dt.wait_ck == 0) //û�������ȴ�У���CMDʱ�ŷ��ͱ�CMD
	{
		//��Э�鷢��ָ��
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

//ˮƽУ׼
u8 Horizontal_Calibrate()
{
	//
	if (dt.wait_ck == 0) //û�������ȴ�У���CMDʱ�ŷ��ͱ�CMD
	{
		//��Э�鷢��ָ��
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

//������У׼
u8 Mag_Calibrate()
{
	//
	if (dt.wait_ck == 0) //û�������ȴ�У���CMDʱ�ŷ��ͱ�CMD
	{
		//��Э�鷢��ָ��
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

//6����ٶ�У׼
u8 ACC_Calibrate()
{
	//
	if (dt.wait_ck == 0) //û�������ȴ�У���CMDʱ�ŷ��ͱ�CMD
	{
		//��Э�鷢��ָ��
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

//������У׼
u8 GYR_Calibrate()
{
	//
	if (dt.wait_ck == 0) //û�������ȴ�У���CMDʱ�ŷ��ͱ�CMD
	{
		//��Э�鷢��ָ��
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

void tar_setdata(s16 x,s16 y, s16 z,s16 yaw)//ʵʱ����֡�����ٶ�
{
	
	Vx=x,Vy=y,Vz=z,Yaw=yaw;
	//	rt_tar.st_data.vel_x=x;//ͷ���ٶ� ��λcm/s
	//	rt_tar.st_data.vel_y=y;//�����Ҹ�
	//	rt_tar.st_data.vel_z=z;//�����ٶ�
	//	rt_tar.st_data.yaw_dps=yaw;//��ʱ��Ϊ�� ��λ��/��
	//	dt.fun[0x41].WTS = 1;
} 

u8 Height_Move(u16 high,u16 high_s){  //����
	//
	if (dt.wait_ck == 0) //û�������ȴ�У���CMDʱ�ŷ��ͱ�CMD
	{
		//��Э�鷢��ָ��
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

u8 Low_Move(u16 high,u16 high_s){  //�½�
	//
	if (dt.wait_ck == 0) //û�������ȴ�У���CMDʱ�ŷ��ͱ�CMD
	{
		//��Э�鷢��ָ��
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

u8 Yaw_get(){  //��ȡŷ����
	//
	if (dt.wait_ck == 0) //û�������ȴ�У���CMDʱ�ŷ��ͱ�CMD
	{
		//��Э�鷢��ָ��
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
/*֮���*/
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
