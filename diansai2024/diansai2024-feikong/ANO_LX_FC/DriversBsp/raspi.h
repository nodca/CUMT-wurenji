#ifndef  __RASPI_H
#define  __RASPI_H


#include "SysConfig.h"
#include "Ano_Math.h"

typedef struct
{
	u8 rasypi_mod; // 0 ʲô��û�� 1 ֻ��t265 2ֻ���״� 3ȫ������
	u8 of_rasypi_update_cnt;  //T265�ٶȵ����ݸ��¼�����
	u8 pos_rasypi_update_cnt;
	//  
	u8 rasypi_link_sta; //����״̬��0��δ���ӡ�1�������ӡ�
	u8 rasypi_work_sta; //����״̬��0���쳣��1������
	
	u8 t265_of_quality;
	//t265���ٶ���Ϣ����λcm
	float speed_x_cms;
	float speed_y_cms;
	u32 alt_cm;
	//��̼���Ϣ
	float pose_x_cm;
	float pose_y_cm;
	
	s32 dist_1;   
	s32 dist_2;
	s32 dist_3;
	s32 dist_4;
	s32 dist_5;
	
}DATA_Flight;


typedef union
{
	s32 Data[4];
	u8 byet[16];
}rasypi_delta;



typedef union
{
	s32 Data[5];
	u8 byet[20];
}rasypi_dist;

typedef union
{
	float Data[5];
	u8 byet[20];
}float_u8_opmv;

extern DATA_Flight base_link;
void Openmv_GetOneByte(uint8_t data);
void rasypi_GetOneByte_delta(uint8_t data);
void rastpi_GetOneByte_dist(uint8_t date);
void T265_data(uint8_t date);


#endif
