#ifndef _LANYA_H_
#define _LANYA_H_

#include "SysConfig.h"

void Lanya_Receive_Data(uint8_t com_data);
void Openmv_Receive_Data(uint8_t com_data);
void lanya_send();
extern u8 ok_flag;
extern u8 fire_flag;
#endif
