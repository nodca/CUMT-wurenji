#ifndef __SHIJUE_H
#define __SHIJUE_H


#include "bsp_sys.h"

extern u8 str_buff_FPX[64];


void  lanya_Receive_Data(uint8_t data);
void BlueToothTransmitData(void);
void HMI_Receive_Data(uint8_t com_data);
void lanya_rec(uint8_t com_data);
typedef union{
	
	u8 U8_BUFF[2];
	s16 S16;
}UNION;
extern UNION Pos_x,Pos_y;
extern u8 huowu_num[25],state,huowu_last_num,num,liang_flag,last_liang_flag;
#endif
