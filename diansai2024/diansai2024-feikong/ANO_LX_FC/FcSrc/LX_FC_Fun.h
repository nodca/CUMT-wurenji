#ifndef __LX_FC_FUN_H
#define __LX_FC_FUN_H

//==引用
#include "SysConfig.h"
typedef uint8_t u8;
typedef uint16_t u16;
//==定义/声明
struct PID_inc
{
	s16 target;
	s16 actual;
	float p;
	float i;
	float d;
	s32 err_current;
	s32 err_last;
	s32 err_previous;
};

//==数据声明


//==函数声明
//static

//public
u8 FC_Unlock(void);
u8 FC_Lock(void);
u8 LX_Change_Mode(u8 new_mode);
u8 OneKey_Takeoff(u16 height_cm);
u8 OneKey_Land(void);
u8 OneKey_Flip(void);
u8 OneKey_Return_Home(void);
u8 Horizontal_Calibrate(void);
u8 Horizontal_Move(u16 distance_cm, u16 velocity_cmps, u16 dir_angle_0_360);
u8 Angel_Move_Left(u16 deg,u16 deg_s);
u8 Angel_Move_Right(u16 deg,u16 deg_s);
u8 Mag_Calibrate(void);
u8 ACC_Calibrate(void);
u8 GYR_Calibrate(void);
u8 Height_Move(u16 high,u16 high_s);
u8 Low_Move(u16 high,u16 high_s);
void tar_setdata(s16 x,s16 y, s16 z,s16 yaw);
void PID_init(void);
s16 height_set(u32 height,u16 height_set);
s16 xypid_set(s32 xy,s16 xy_set,u16 speedmax );
s16 xyPos_set(s32 xy,s16 xy_set);
u8 Yaw_get();
s16 Yaw_set(s16 tar_yaw,s16 yaw_val);

extern s16 Vx,Vy,Vz,Yaw;
#endif
