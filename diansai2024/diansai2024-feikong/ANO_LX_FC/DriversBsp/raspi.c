#include "raspi.h"


DATA_Flight base_link;    //飞控数据

rasypi_delta get_base_link;
rasypi_dist  get_base_link_rpldar;
int num=0;
s32 err_x,err_y;
//static s32 rasypi_data_old[4];
#define T265_hz 100
#define Rader_hz 10
//协议0xAA 0xFF Vx Vy X Y 0xCD 0XDC

/*只有T265*/
void T265_data(u8 data){
	static u8 DataReceive[30];
	static u8 my_flag = 0;
	static u8 _datalen = 0;		//数据长度
	static u8 Radar_cnt = 0,Rader_up = T265_hz / Rader_hz;
	
	static u8 DataReceive_dist[30];
	static u8 my_flag_dist = 0;
	static u8 _datalen_dist = 0;		//数据长度
	
	switch (my_flag)
	{
		case 0:
		{
			if(data == 0xAA)
				my_flag++;
			_datalen = 0;
		}
		break;
		case 1:
		{
			if(data == 0xFF)
				my_flag++;
			else
				my_flag = 0;
		}
		break;		
		case 2:
		{	
			DataReceive[_datalen++] = data;
			if(_datalen == 17)
				my_flag++;
		}
		break;
		case 3:
		{
			if(data == 0xCD)
				my_flag++;
			else
				my_flag = 0;
		}
		break;
		case 4:
		{
			if(data == 0xDC )
			{
				
				base_link.rasypi_mod = DataReceive[0];
				for(u8 i = 0;i < 16;i++)
					get_base_link.byet[i] = DataReceive[i+1];
				
//				for(u8 i = 0;i < 4;i++)
//					if(ABS(float_get_base_link.Data[i] - rasypi_data_old[i]) > 200)
//						float_get_base_link.Data[i] = rasypi_data_old[i];
				
				if(ABS(get_base_link.Data[0]) < 20000)
					S_LPF_1(0.5f,get_base_link.Data[0] ,base_link.speed_x_cms); 
				if(ABS(get_base_link.Data[1]) < 20000)
					S_LPF_1(0.5f,get_base_link.Data[1] ,base_link.speed_y_cms);				
				base_link.of_rasypi_update_cnt++;
				
			//	Radar_cnt++;
				//Radar_cnt %= Rader_up;					
			//	if(Radar_cnt == 0)
			//	{
					base_link.pose_x_cm =  get_base_link.Data[2] ;
					base_link.pose_y_cm =  get_base_link.Data[3] ;
					base_link.pos_rasypi_update_cnt++;
			//	}
											
				base_link.rasypi_work_sta = 1;
			}
			my_flag = 0;
		}
		break;
		default:break;
	}
}


