#ifndef __SHIJUE_H
#define __SHIJUE_H


#include "bsp_sys.h"

extern u8 str_buff_FPX[64];


void Lanya_Receive_Data(uint8_t com_data);
void LanYaTransmitData(void);
void FeiKongTransmitData(void);
typedef union{
	
	u8 U8_BUFF[2];
	s16 S16;
}UNION;
extern UNION Pos_x,Pos_y;
void feikong_Receive_Data(uint8_t com_data);
void shijue_Receive_Data(uint8_t com_data);
void shijue2_Receive_Data(uint8_t com_data);
#endif
