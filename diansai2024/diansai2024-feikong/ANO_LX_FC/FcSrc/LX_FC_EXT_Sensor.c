/*==========================================================================
 * ����    �������ɿ����ô���������
 * ����ʱ�䣺2020-02-06 
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
#include "LX_FC_EXT_Sensor.h"
#include "Drv_AnoOf.h"
#include "ANO_DT_LX.h"
#include "raspi.h"

_fc_ext_sensor_st ext_sens;

//����ѹ������ݴ����ͨ���ٶȴ���������
static inline void General_Velocity_Data_Handle()
{
	static u8 of_update_cnt, of_alt_update_cnt;
	static u8 dT_ms = 0;
	//ÿһ����dT_ms+1�������ж��Ƿ�ʱ��������
	if (dT_ms != 255)
	{
		dT_ms++;
	}
	//���OF�����Ƿ����
	if (of_update_cnt != ano_of.of_update_cnt)
	{
		of_update_cnt = ano_of.of_update_cnt;
		//XY_VEL
		if (ano_of.of1_sta && ano_of.work_sta) //������Ч
		{
			ext_sens.gen_vel.st_data.hca_velocity_cmps[0] = ano_of.of1_dx;
			ext_sens.gen_vel.st_data.hca_velocity_cmps[1] = ano_of.of1_dy;
		}
		else //��Ч
		{
			ext_sens.gen_vel.st_data.hca_velocity_cmps[0] = 0x8000;
			ext_sens.gen_vel.st_data.hca_velocity_cmps[1] = 0x8000;
		}
	}
	if (of_alt_update_cnt != ano_of.alt_update_cnt)
	{
		//
		of_alt_update_cnt = ano_of.alt_update_cnt;
		//������z���ٶȣ���z�ٶȸ�ֵΪ��Ч
		ext_sens.gen_vel.st_data.hca_velocity_cmps[2] = 0x8000;
		//��������
		dt.fun[0x33].WTS = 1;
		//reset
		dT_ms = 0;
	}
}

static inline void General_Distance_Data_Handle()
{
	static u8 of_alt_update_cnt;
	if (of_alt_update_cnt != ano_of.alt_update_cnt)
	{
		//
		of_alt_update_cnt = ano_of.alt_update_cnt;
		//
		ext_sens.gen_dis.st_data.direction = 0;
		ext_sens.gen_dis.st_data.angle_100 = 270;
		ext_sens.gen_dis.st_data.distance_cm = ano_of.of_alt_cm;
		//��������
		dt.fun[0x34].WTS = 1;
	}
}

static inline void mid360_Velocity_Data_Handle()
{
	static u8 of_update_cnt;
	static u8 dT_ms = 0;
	//ÿһ����dT_ms+1�������ж��Ƿ�ʱ��������
	if (dT_ms != 255)
	{
		dT_ms++;
	}
	//���OF�����Ƿ����
	if (of_update_cnt != base_link.of_rasypi_update_cnt)
	{
		of_update_cnt = base_link.of_rasypi_update_cnt;
		//XY_VEL
		if (base_link.rasypi_work_sta) //t265��Ч
		{
			ext_sens.gen_vel.st_data.hca_velocity_cmps[0] = (s16)(base_link.speed_x_cms);
			ext_sens.gen_vel.st_data.hca_velocity_cmps[1] = (s16)(base_link.speed_y_cms);
		}
		else //��Ч
		{
			ext_sens.gen_vel.st_data.hca_velocity_cmps[0] = 0x8000;
			ext_sens.gen_vel.st_data.hca_velocity_cmps[1] = 0x8000;
			
			
		}
		ext_sens.gen_vel.st_data.hca_velocity_cmps[2] = 0x8000;
		dt.fun[0x33].WTS = 1;
		//reset
		dT_ms = 0;
	}
}

static inline void General_Pos_Data_Handle()
{
	static u8 of_update_cnt;
	static u8 dT_ms = 0;
	//ÿһ����dT_ms+1�������ж��Ƿ�ʱ��������
	if (dT_ms != 255)
	{
		dT_ms++;
	}
	//���OF�����Ƿ����
	if (of_update_cnt != base_link.pos_rasypi_update_cnt)
	{
		of_update_cnt = base_link.pos_rasypi_update_cnt;
		//XY_VEL
		if (base_link.rasypi_work_sta) //t265��Ч
		{
			ext_sens.gen_pos.st_data.ulhca_pos_cm[0] = (s32)(base_link.pose_x_cm);
			ext_sens.gen_pos.st_data.ulhca_pos_cm[1] = (s32)(base_link.pose_y_cm);
		}
		else //��Ч
		{
			ext_sens.gen_pos.st_data.ulhca_pos_cm[0] = 0x80000000;
			ext_sens.gen_pos.st_data.ulhca_pos_cm[1] = 0x80000000;
		}
		ext_sens.gen_pos.st_data.ulhca_pos_cm[2] = 0x80000000;
		
		dt.fun[0x32].WTS = 1;
		//reset
		dT_ms = 0;
	}
}

void LX_FC_EXT_Sensor_Task(float dT_s) //1ms
{
	//
	General_Velocity_Data_Handle(); //����
  	//mid360_Velocity_Data_Handle();  //�״��ٶ�
	//
	//	General_Velocity_Data_Handle();//�����ٶ�

	
	General_Distance_Data_Handle();

	//	General_Pos_Data_Handle();
}
