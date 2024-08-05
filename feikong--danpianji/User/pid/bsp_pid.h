#ifndef __BSP_PID_H
#define	__BSP_PID_H
#include "stm32f1xx.h"
#include "usart.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>





typedef struct
{
    float target_val;           //Ŀ��ֵ
    float actual_val;        		//ʵ��ֵ
    float err;             			//����ƫ��ֵ
    float err_last;          		//������һ��ƫ��ֵ
    float Kp,Ki,Kd;          		//������������֡�΢��ϵ��
    float integral;          		//�������ֵ
}_pid;

  
extern _pid pid_location,pid_location2;
extern _pid pid_speed, pid_speed2,pid_speed3,pid_speed4;    
extern _pid pid_angel;  


 void PID_param_init(void);
 void set_pid_target(_pid *pid, float temp_val);
 float get_pid_target(_pid *pid);
 void set_p_i_d(_pid *pid, float p, float i, float d);


float location_pid_realize(_pid *pid, float actual_val);
float speed_pid_realize(_pid *pid, float actual_val);
float angel_pid(_pid *pid,float actual_val);

#endif
